#pragma once

#include <Windows.h>

#include <set>
#include <string>

class NppSaveAsAdminBehaviour {
 public:
  virtual bool process_file(const std::wstring& filename) = 0;
  virtual std::wstring convert_path(const std::wstring& filename) = 0;
  virtual HANDLE run_admin_access(const std::wstring& pipe_sender_name,
                                  const std::wstring& pipe_receiver_name) = 0;
};

class DefaultNppSaveAsAdminBehaviour : public NppSaveAsAdminBehaviour {
  std::set<std::wstring> m_allowed_files;

 public:
  bool process_file(const std::wstring& filename) override;
  std::wstring convert_path(const std::wstring& filename) override;
  HANDLE run_admin_access(const std::wstring& pipe_sender_name,
                          const std::wstring& pipe_receiver_name) override;

  void allow_process_file(const std::wstring& filename);
  void cancel_process_file(const std::wstring& filename);

  static DefaultNppSaveAsAdminBehaviour& instance();
};

void set_save_as_admin_behaviour(NppSaveAsAdminBehaviour& behviour);
