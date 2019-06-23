#pragma once

#include <Shlwapi.h>
#include <iostream>

#include <atomic>
#include <functional>
#include <memory>
#include <sstream>

const std::uint32_t MaxModuleNameLen = 128;
const std::uint32_t MaxFunctionNameLen = 64;

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

inline bool name_is_suitable(const char* desired_module_name,
                             HMODULE module_handle,
                             PIMAGE_IMPORT_DESCRIPTOR import_description) {
  return !desired_module_name ||
         0 == StrCmpIA((char*)((PBYTE)module_handle + import_description->Name),
                       desired_module_name);
}

template <class Proc>
struct InjectionResult {
  PROC* old_function_position = nullptr;
  Proc old_function = nullptr;
};

template <class Proc>
InjectionResult<Proc> process_injection(const char* module_name,
                                        const char* function_name,
                                        Proc ptr_to_new_fun) {
  const size_t name_length = strnlen(function_name, MaxFunctionNameLen);
  HMODULE module_handle = GetModuleHandle(NULL);

  if (!module_handle)
    throw std::logic_error("Failed to retrieve module handle");

  ULONG description_size = 0;

  PIMAGE_IMPORT_DESCRIPTOR import_description =
      (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
          module_handle, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &description_size);

  for (; import_description && import_description->Name; ++import_description) {
    if (name_is_suitable(module_name, module_handle, import_description))
      break;
  }

  for (; import_description && import_description->Name; ++import_description) {
    if (!name_is_suitable(module_name, module_handle, import_description))
      break;
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
        InjectionResult<Proc> result;
        result.old_function_position = ppfn;
        result.old_function = (Proc)(PROC)*ppfn;
        append_new_ptr(ppfn, ptr_to_new_fun);

        return result;
      }

      thunk++;
      thunk_orig++;
    }
  }
  std::stringstream error_info;
  error_info << "Injected function '" << function_name << "' ";
  if (module_name) {
    error_info << "in module '" << module_name << "' ";
  }
  error_info << "was not found";
  throw std::logic_error(error_info.str());
}

template <class Ret, class... Args>
class ScopedInjector {
  typedef Ret(WINAPI* WinapiFunctionPointer)(Args...);

  struct Data {
    InjectionResult<WinapiFunctionPointer> injection_result;
    std::function<Ret(Args... args)> callback;
  };

  Data* m_data = nullptr;

  template <class Unique>
  struct StaticFunctionWrapper {
    static Data data;
    static Ret WINAPI injected_function(Args... args) {
      if (data.callback) {
        return data.callback(args...);
      }
      return data.injection_result.old_function(args...);
    }
  };

 public:
  template <class Unique>
  ScopedInjector(Unique,
                 const char* module,
                 const char* function,
                 std::function<Ret(Args...)> callback) {
    static StaticFunctionWrapper<Unique> static_function;
    if (static_function.data.callback) {
      std::stringstream descr;
      descr << "You can't use same injection macro to ";
      descr << " inject same function more than once.";
      if (function)
        descr << " Function:" << function;
      if (module)
        descr << " in module: " << module;
      throw std::logic_error(descr.str());
    }
    static_function.data.callback = callback;
    m_data = &static_function.data;
    try {
      static_function.data.injection_result = process_injection(
          module, function, static_function.injected_function);
    } catch (...) {
      static_function.data.callback = nullptr;
      throw;
    }
  }

  ~ScopedInjector() {
    m_data->callback = nullptr;
    append_new_ptr(m_data->injection_result.old_function_position,
                   m_data->injection_result.old_function);
  }

  Ret call_original(Args... args) {
    return m_data->injection_result.old_function(args...);
  }

  using Pointer = std::unique_ptr<ScopedInjector<Ret, Args...>>;
};

template <class Ret, class... Args>
template <class Unique>
typename ScopedInjector<Ret, Args...>::Data
    ScopedInjector<Ret, Args...>::StaticFunctionWrapper<Unique>::data;

template <class Unique, class Ret, class... Args>
auto make_inject(const char* module,
                 const char* function,
                 Ret(WINAPI* ptr_to_original_fun)(Args...),
                 std::function<Ret(Args...)> new_function) {
  Unique unique;
  (void)ptr_to_original_fun;
  return std::make_unique<ScopedInjector<Ret, Args...>>(unique, module,
                                                        function, new_function);
}

template <class Unique, class CallbackType, class Ret, class... Args>
auto make_inject(const char* module,
                 const char* function,
                 Ret(WINAPI* ptr_to_original_fun)(Args...),
                 CallbackType new_function) {
  std::function<Ret(Args...)> function_wrapper = [=](Args... args) {
    return new_function(args...);
  };
  return make_inject<Unique>(module, function, ptr_to_original_fun,
                             function_wrapper);
}

template <class Ret, class... Args>
auto injection_type_helper(Ret(WINAPI*)(Args...)) {
  return ScopedInjector<Ret, Args...>::Pointer(nullptr);
}

#define injection_stringize_helper(s) #s
#define injection_stringize(s) injection_stringize_helper(s)

#define injection_ptr_type(win_api_function) \
  decltype(injection_type_helper(win_api_function))

#define inject(win_api_function, new_function) \
  inject_in_module(nullptr, win_api_function, new_function)

#define inject_in_module(module_name, win_api_function, new_function) \
  [](auto module_name_var, auto&& new_function_var) {                 \
    struct Unique {};                                                 \
    return make_inject<Unique>(module_name_var,                       \
                               injection_stringize(win_api_function), \
                               win_api_function, new_function_var);   \
  }(module_name, new_function);

template <class ClassObj, class ClassWithMemeber, class Rest, class... Args>
auto make_injection_callback(ClassObj& object,
                             Rest (ClassWithMemeber::*member)(Args...)) {
  ClassObj* object_ptr = &object;
  return [=](Args... args) { return (object_ptr->*member)(args...); };
}
