#pragma once

#include <Shlwapi.h>
#include <memory>

inline void append_new_ptr_impl(PROC* ptr_to_fun_memory, PROC ptr_to_new_fun) {
  MEMORY_BASIC_INFORMATION mbi = {0};
  VirtualQuery(ptr_to_fun_memory, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
  VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE,
                 &mbi.Protect);

  // Replace the original address of API with the address of corresponding
  // wrapper function
  *ptr_to_fun_memory = *ptr_to_new_fun;

  DWORD old_protect = 0;
  VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &old_protect);
}

template <class Proc>
void append_new_ptr(PROC* ptr_to_fun_memory, Proc ptr_to_new_fun) {
  append_new_ptr_impl(ptr_to_fun_memory,
                      reinterpret_cast<PROC>(ptr_to_new_fun));
};

template <class Proc>
void process_injection(const char* module_name,
                       const char* function_name,
                       Proc& save_old_to_ptr,
                       Proc ptr_to_new_fun) {
  const size_t name_length = strnlen(function_name, 64);
  HMODULE module_handle = GetModuleHandle(NULL);

  if (!module_handle)
    return;

  ULONG description_size = 0;

  PIMAGE_IMPORT_DESCRIPTOR import_description =
      (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
          module_handle, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &description_size);

  while (import_description && import_description->Name) {
    if (0 == StrCmpIA((char*)((PBYTE)module_handle + import_description->Name),
                      module_name))
      break;

    ++import_description;
  }

  while (import_description && import_description->Name) {
    PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)(
        (PBYTE)module_handle + import_description->FirstThunk);
    PIMAGE_THUNK_DATA thunk_orig = (PIMAGE_THUNK_DATA)(
        (PBYTE)module_handle + import_description->OriginalFirstThunk);

    while (thunk->u1.AddressOfData) {
      PROC* ppfn = (PROC*)&thunk->u1.AddressOfData;

      PIMAGE_IMPORT_BY_NAME image_name = (PIMAGE_IMPORT_BY_NAME)(
          (PBYTE)module_handle + thunk_orig->u1.AddressOfData);

      if (0 ==
          strncmp(function_name, (const char*)image_name->Name, name_length)) {
        save_old_to_ptr = (Proc)(PROC)*ppfn;
        append_new_ptr(ppfn, ptr_to_new_fun);

        return;
      }

      thunk++;
      thunk_orig++;
    }

    import_description++;
  }
}

template <class Ret, class... Args>
class ScopedInjector {
  const char* m_module;
  const char* m_function;
  typedef Ret(WINAPI* WinapiFunctionPointer)(Args...);
  WinapiFunctionPointer m_original_function_ptr = nullptr;

 public:
  ScopedInjector(const char* module,
                 const char* function,
                 WinapiFunctionPointer ptr_to_new_fun)
      : m_module(module), m_function(function) {
    process_injection(m_module, m_function, m_original_function_ptr, ptr_to_new_fun);
  }

  ~ScopedInjector() {
    WinapiFunctionPointer dummy;
    process_injection(m_module, m_function, dummy, m_original_function_ptr);
  }

  Ret call_original(Args... args) { return m_original_function_ptr(args...); }

  using Pointer = std::unique_ptr<ScopedInjector<Ret, Args...>>;
};

template <class Ret, class... Args>
auto inject(const char* module,
            const char* function,
            Ret(WINAPI* ptr_to_new_fun)(Args...)) {
  return std::make_unique<ScopedInjector<Ret, Args...>>(module, function,
                                                        ptr_to_new_fun);
}
