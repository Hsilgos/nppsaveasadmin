#pragma once

// clang-format off
#include <Windows.h>
#include <Dbghelp.h>
#include <Psapi.h>
// clang-format on

class IWinApiFunctions {
 public:
  virtual BOOL write_file(HANDLE file_handle,
                                 LPCVOID buffer,
                                 DWORD number_of_bytes_to_write,
                                 LPDWORD number_of_bytes_written,
                                 LPOVERLAPPED overlapped) = 0;

  virtual HANDLE create_file_a(LPCSTR file_name,
                                     DWORD desired_access,
                                     DWORD share_mode,
                                     LPSECURITY_ATTRIBUTES security_attributes,
                                     DWORD creation_disposition,
                                     DWORD flags_and_attributes,
                                     HANDLE template_file) = 0;

  virtual HANDLE create_file_w(LPCWSTR file_name,
                                     DWORD desired_access,
                                     DWORD share_mode,
                                     LPSECURITY_ATTRIBUTES security_attributes,
                                     DWORD creation_disposition,
                                     DWORD flags_and_attributes,
                                     HANDLE template_file) = 0;

  virtual BOOL close_handle(HANDLE object) = 0;

  virtual DWORD get_file_type(HANDLE file) = 0;
};

class DefaultWinApiFunctions : public IWinApiFunctions {
 public:
  BOOL write_file(HANDLE file_handle,
                         LPCVOID buffer,
                         DWORD number_of_bytes_to_write,
                         LPDWORD number_of_bytes_written,
                         LPOVERLAPPED overlapped) override {
    return WriteFile(file_handle, buffer, number_of_bytes_to_write,
                     number_of_bytes_written, overlapped);
  }

  HANDLE create_file_a(LPCSTR file_name,
                             DWORD desired_access,
                             DWORD share_mode,
                             LPSECURITY_ATTRIBUTES security_attributes,
                             DWORD creation_disposition,
                             DWORD flags_and_attributes,
                             HANDLE template_file) override {
    return CreateFileA(file_name, desired_access, share_mode,
                       security_attributes, creation_disposition,
                       flags_and_attributes, template_file);
  }

  HANDLE create_file_w(LPCWSTR file_name,
                             DWORD desired_access,
                             DWORD share_mode,
                             LPSECURITY_ATTRIBUTES security_attributes,
                             DWORD creation_disposition,
                             DWORD flags_and_attributes,
                             HANDLE template_file) override {
    return CreateFileW(file_name, desired_access, share_mode,
                       security_attributes, creation_disposition,
                       flags_and_attributes, template_file);
  }

  BOOL close_handle(HANDLE object) override {
    return CloseHandle(object);
  }

  DWORD get_file_type(HANDLE file) override { return GetFileType(file); }
};
