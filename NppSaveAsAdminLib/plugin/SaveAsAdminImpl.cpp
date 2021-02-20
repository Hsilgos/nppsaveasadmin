#include "SaveAsAdminImpl.hpp"

#include <map>
#include <set>

#include "AdminAccess.hpp"
#include "FuncExecutor.hpp"
#include "IWinApiFunctions.hpp"
#include "Injection.hpp"

#include "Pipe.hpp"

class SaveAsAdminImpl::Impl {
 public:
  Impl(AdminAccessRunner& admin_access_runner)
      : m_admin_access_runner(admin_access_runner) {
    m_write_file = inject_in_module(
        "Kernel32.dll", WriteFile,
        make_injection_callback(*this, &Impl::write_file_hook));

    m_create_filew = inject_in_module(
        "Kernel32.dll", CreateFileW,
        make_injection_callback(*this, &Impl::create_file_w_hook));
    m_get_file_type = inject_in_module(
        "Kernel32.dll", GetFileType,
        make_injection_callback(*this, &Impl::get_file_type_hook));
    m_close_handle = inject_in_module(
        "Kernel32.dll", CloseHandle,
        make_injection_callback(*this, &Impl::close_handle_hook));
  }

  void allow_process_file() { m_is_process_allowed = true; }

  void cancel_process_file() { m_is_process_allowed = false; }

 private:
  BOOL write_file_hook(HANDLE file_handle,
                       LPCVOID buffer,
                       DWORD number_of_bytes_to_write,
                       LPDWORD number_of_bytes_written,
                       LPOVERLAPPED overlapped) {
    HandleMap::iterator it = m_file_handles.find(file_handle);
    if (it != m_file_handles.end()) {
      return execute_write_file(
          *it->second->pipe_sender, *it->second->pipe_receiver,
          it->second->origin_handle, buffer, number_of_bytes_to_write,
          number_of_bytes_written);
    } else {
      return m_write_file->call_original(file_handle, buffer,
                                         number_of_bytes_to_write,
                                         number_of_bytes_written, overlapped);
    }
  }

  HANDLE create_file_w_hook(LPCWSTR file_name,
                            DWORD desired_access,
                            DWORD share_mode,
                            LPSECURITY_ATTRIBUTES security_attributes,
                            DWORD creation_disposition,
                            DWORD flags_and_attributes,
                            HANDLE template_file) {
    HANDLE result = m_create_filew->call_original(
        file_name, desired_access, share_mode, security_attributes,
        creation_disposition, flags_and_attributes, template_file);

    if (INVALID_HANDLE_VALUE == result && (desired_access & GENERIC_WRITE)) {
      const int error_code = GetLastError();
      if (ERROR_ACCESS_DENIED == error_code && m_is_process_allowed) {
        std::unique_ptr<FileHandle> new_handle = std::make_unique<FileHandle>();
        new_handle->pipe_sender = Pipe::create_unique();
        new_handle->pipe_receiver = Pipe::create_unique();
        if (!new_handle->pipe_sender || !new_handle->pipe_receiver) {
          return INVALID_HANDLE_VALUE;
        }

        new_handle->proc_handle = m_admin_access_runner.run_admin_access(
            new_handle->pipe_sender->get_name(),
            new_handle->pipe_receiver->get_name());
        if (INVALID_HANDLE_VALUE == new_handle->proc_handle) {
          return INVALID_HANDLE_VALUE;
        }

        new_handle->pipe_sender->wait();
        new_handle->pipe_receiver->wait();

        new_handle->origin_handle = execute_create_file_w(
            *new_handle->pipe_sender, *new_handle->pipe_receiver, file_name,
            desired_access, share_mode, creation_disposition,
            flags_and_attributes);
        if (INVALID_HANDLE_VALUE == new_handle->origin_handle) {
          return INVALID_HANDLE_VALUE;
        }

        result = new_handle->proc_handle;
        m_file_handles[new_handle->proc_handle] = std::move(new_handle);
      }
    }

    return result;
  }

  BOOL close_handle_hook(HANDLE handle) {
    HandleMap::iterator it = m_file_handles.find(handle);
    if (it != m_file_handles.end()) {
      BOOL result = execute_close_handle(*it->second->pipe_sender,
                                         *it->second->pipe_receiver,
                                         it->second->origin_handle);
      execute_exit(*it->second->pipe_sender);
      m_file_handles.erase(it);
      return result;
    } else {
      return m_close_handle->call_original(handle);
    }
  }

  DWORD get_file_type_hook(HANDLE handle) {
    HandleMap::iterator it = m_file_handles.find(handle);
    if (it != m_file_handles.end())
      return execute_get_file_type(*it->second->pipe_sender,
                                   *it->second->pipe_receiver,
                                   it->second->origin_handle);
    else
      return m_get_file_type->call_original(handle);
  }

 private:
  AdminAccessRunner& m_admin_access_runner;
  bool m_is_process_allowed = false;

  struct FileHandle {
    std::unique_ptr<Pipe> pipe_sender;
    std::unique_ptr<Pipe> pipe_receiver;
    HANDLE proc_handle;
    HANDLE origin_handle;
  };

  using HandleMap = std::map<HANDLE, std::unique_ptr<FileHandle>>;
  HandleMap m_file_handles;

  injection_ptr_type(WriteFile) m_write_file;
  injection_ptr_type(CreateFileW) m_create_filew;
  injection_ptr_type(GetFileType) m_get_file_type;
  injection_ptr_type(CloseHandle) m_close_handle;
};

namespace {
class DefaultAdminAccessRunner : public AdminAccessRunner {
 public:
  DefaultAdminAccessRunner(std::wstring exe_path)
      : m_exe_path(std::move(exe_path)) {}

 private:
  HANDLE run_admin_access(const std::wstring& pipe_sender_name,
                          const std::wstring& pipe_receiver_name) override {
    return run_admin_access_app(m_exe_path, pipe_sender_name,
                                pipe_receiver_name);
  }

 private:
  std::wstring m_exe_path;
};
}  // namespace

AdminAccessRunner::~AdminAccessRunner() = default;

std::unique_ptr<AdminAccessRunner> AdminAccessRunner::make_default(
    std::wstring admin_runner_exe_path) {
  return std::unique_ptr<AdminAccessRunner>(
      new DefaultAdminAccessRunner(std::move(admin_runner_exe_path)));
}

SaveAsAdminImpl::SaveAsAdminImpl(AdminAccessRunner& admin_access_runner)
    : m_impl(std::make_unique<Impl>(admin_access_runner)) {}

SaveAsAdminImpl::~SaveAsAdminImpl() = default;

void SaveAsAdminImpl::allow_process_file() {
  m_impl->allow_process_file();
}

void SaveAsAdminImpl::cancel_process_file() {
  m_impl->allow_process_file();
}
