#pragma once

#include <Windows.h>

#include <memory>
#include <string>

class IWinApiFunctions;

class AdminAccessRunner {
 public:
  virtual HANDLE run_admin_access(const std::wstring& pipe_sender_name,
                                  const std::wstring& pipe_receiver_name) = 0;

  static std::unique_ptr<AdminAccessRunner> make_default(
      std::wstring admin_runner_exe_path);
  virtual ~AdminAccessRunner();
};

class SaveAsAdminImpl {
 public:
  SaveAsAdminImpl(AdminAccessRunner& admin_access_runner);
  ~SaveAsAdminImpl();

  void allow_process_file();
  void cancel_process_file();

 private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
