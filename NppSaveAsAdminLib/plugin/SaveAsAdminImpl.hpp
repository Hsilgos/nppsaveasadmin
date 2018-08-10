#pragma once

#include <Windows.h>

#include <memory>
#include <string>

class IWinApiFunctions;

class AdminAccessRunner {
 public:
  virtual HANDLE run_admin_access(IWinApiFunctions& original_functions,
                                  const std::wstring& pipe_sender_name,
                                  const std::wstring& pipe_receiver_name) = 0;
  virtual ~AdminAccessRunner();
};

class SaveAsAdminImpl {
 public:
  SaveAsAdminImpl();
  SaveAsAdminImpl(AdminAccessRunner& admin_access_runner);
  ~SaveAsAdminImpl();

  void allow_process_file(const std::wstring& filename);
  void cancel_process_file(const std::wstring& filename);

 private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
