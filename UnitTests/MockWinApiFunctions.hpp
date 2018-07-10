#pragma once

#include <gmock/gmock.h>

#include "IWinApiFunctions.hpp"

class MockWinApiFunctions : public IWinApiFunctions {
 public:
  MOCK_METHOD5(write_file, BOOL(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED));
  MOCK_METHOD7(create_file_a,
               HANDLE(LPCSTR,
                      DWORD,
                      DWORD,
                      LPSECURITY_ATTRIBUTES,
                      DWORD,
                      DWORD,
                      HANDLE));
  MOCK_METHOD7(create_file_w,
               HANDLE(LPCWSTR,
                      DWORD,
                      DWORD,
                      LPSECURITY_ATTRIBUTES,
                      DWORD,
                      DWORD,
                      HANDLE));
  MOCK_METHOD1(close_handle, BOOL(HANDLE));
  MOCK_METHOD1(get_file_type, DWORD(HANDLE));
};
