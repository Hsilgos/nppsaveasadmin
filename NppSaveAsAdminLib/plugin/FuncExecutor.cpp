#include "FuncExecutor.hpp"

#include "Common.hpp"

#include "Pipe.hpp"

template <class InData, class OutData>
bool execute_function(Pipe& pipe_sender,
                      Pipe& pipe_receiver,
                      const InData& in_data,
                      Commands cmd_num,
                      OutData& out_data) {
  std::vector<char> in_buffer;
  std::vector<char> read_buffer;

  PacketWithCommand<InData>* in_ptr =
      prepare_vector_to_store_data<PacketWithCommand<InData>>(in_buffer);
  in_ptr->command = cmd_num;
  in_ptr->commandData = in_data;

  if (!pipe_sender.write(in_buffer))
    return false;

  const bool read_result = pipe_receiver.read(read_buffer);
  return read_result && read_data_from_vector(out_data, read_buffer);
}

HANDLE execute_create_file_w(Pipe& pipe_sender,
                             Pipe& pipe_receiver,
                             LPCWSTR file_name,
                             DWORD desired_access,
                             DWORD share_mode,
                             DWORD creation_disposition,
                             DWORD flags_and_attributes) {
  CreateFileDataW tcfdw = {0};
  wcscpy_s(tcfdw.filename, file_name);
  tcfdw.desired_access = desired_access;
  tcfdw.share_mode = share_mode;
  tcfdw.flags_and_attributes = flags_and_attributes;
  tcfdw.creation_disposition = creation_disposition;
  CreateFileResult result = {0};
  if (!execute_function(pipe_sender, pipe_receiver, tcfdw, CreateFileWCmd,
                        result))
    return INVALID_HANDLE_VALUE;

  SetLastError(result.last_error);

  return result.handle;
}

inline void set_number_of_bytes_written(LPDWORD result, DWORD written) {
  if (result) {
    *result = written;
  }
}

BOOL execute_write_file(Pipe& pipe_sender,
                        Pipe& pipe_receiver,
                        HANDLE original_handle,
                        LPCVOID buffer_ptr,
                        DWORD number_of_bytes_to_write,
                        LPDWORD number_of_bytes_written) {
  const char* ptr = static_cast<const char*>(buffer_ptr);

  DWORD written = 0;

  do {
    WriteFileData twfd = {0};
    twfd.handle = original_handle;
    twfd.num_bytes_to_write =
        min(WriteFileData::MaxBufferSize, number_of_bytes_to_write);
    if (ptr)
      memcpy(twfd.buffer, ptr, twfd.num_bytes_to_write);
    else
      twfd.buffer_is_null = true;

    WriteFileResult result = {0};
    if (!execute_function(pipe_sender, pipe_receiver, twfd, WriteFileCmd,
                          result)) {
      set_number_of_bytes_written(number_of_bytes_written, written);
      return FALSE;
    }

    written += result.bytes_written;

    if (!result.success || twfd.num_bytes_to_write != result.bytes_written) {
      SetLastError(result.last_error);
      set_number_of_bytes_written(number_of_bytes_written, written);
      return FALSE;
    }

    if (ptr)
      ptr += result.bytes_written;
    number_of_bytes_to_write -= result.bytes_written;
  } while (number_of_bytes_to_write > 0);

  set_number_of_bytes_written(number_of_bytes_written, written);

  return TRUE;
}

DWORD execute_get_file_type(Pipe& pipe_sender,
                            Pipe& pipe_receiver,
                            HANDLE handle) {
  GetFileTypeData tgft = {0};
  tgft.handle = handle;
  GetFileTypeResult result = {0};
  if (!execute_function(pipe_sender, pipe_receiver, tgft, GetFileTypeCmd,
                        result))
    return FALSE;

  SetLastError(result.last_error);

  return result.type;
}

BOOL execute_close_handle(Pipe& pipe_sender,
                          Pipe& pipe_receiver,
                          HANDLE handle) {
  CloseHandleData tchd = {0};
  tchd.handle = handle;
  CloseHandleResult result = {0};
  if (!execute_function(pipe_sender, pipe_receiver, tchd, CloseHandleCmd,
                        result))
    return FALSE;

  SetLastError(result.last_error);

  return result.success;
}

void execute_exit(Pipe& pipe_sender) {
  char code = ExitCmd;
  pipe_sender.write(data_to_vector(code));
}
