#pragma once

#include <Windows.h>
#include <vector>

enum Errors {
  NoError = 0,
  WrongExecute,
  FailedToOpenPipe,
  ErrorCommandExecution,
  UnknownCommand,
  BadDataInBuffer,
  FailedToWritePipe,
  ReturnBufferHasNoData
};

// commands
enum Commands : std::uint8_t {
  UnknownCmd = 0,

  CreateFileACmd,
  CreateFileWCmd,
  WriteFileCmd,
  CloseHandleCmd,
  GetFileTypeCmd,
  ExitCmd,

  MaxCmd
};

// CreateFile
struct CreateFileDataA {
  char filename[1024];
  DWORD desired_access;
  DWORD share_mode;
  DWORD creation_disposition;
  DWORD flags_and_attributes;
};

struct CreateFileDataW {
  wchar_t filename[1024];
  DWORD desired_access;
  DWORD share_mode;
  DWORD creation_disposition;
  DWORD flags_and_attributes;
};

struct CreateFileResult {
  bool success;
  HANDLE handle;
  DWORD last_error;
};

// WriteFile
struct WriteFileData {
  HANDLE handle;
  DWORD num_bytes_to_write;
  enum { MaxBufferSize = 1024 * 3 };
  bool buffer_is_null = false;
  char buffer[MaxBufferSize];
};

struct WriteFileResult {
  bool success;
  DWORD bytes_written;
  DWORD last_error;
};

// CloseHandle
struct CloseHandleData {
  HANDLE handle;
};

struct CloseHandleResult {
  bool success;
  DWORD last_error;
};

// GetFileType
struct GetFileTypeData {
  HANDLE handle;
};

struct GetFileTypeResult {
  DWORD type;
  int last_error;
};

template <class T>
struct PacketWithCommand {
  Commands command;
  T commandData;
};

template <class T>
T* prepareVectorToStoreData(std::vector<char>& data) {
  data.resize(sizeof(T), 0);
  return reinterpret_cast<T*>(data.data());
}

template <class T>
std::vector<char> dataToVector(const T& data) {
  std::vector<char> result;
  T* tBufferPtr = prepareVectorToStoreData<T>(result);
  *tBufferPtr = data;
  return result;
}

template <class T>
bool readDataFromVector(T& out, const std::vector<char>& data) {
  if (data.size() != sizeof(T))
    return false;
  const T* data_ptr = reinterpret_cast<const T*>(data.data());
  out = *data_ptr;
  return true;
}
