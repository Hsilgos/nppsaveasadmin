#pragma once

#include <Windows.h>

class Pipe;

HANDLE execute_create_file_w(Pipe& pipe_sender,
                             Pipe& pipe_receiver,
                             LPCWSTR file_name,
                             DWORD desired_access,
                             DWORD share_mode,
                             DWORD creation_disposition,
                             DWORD flags_and_attributes);

BOOL execute_write_file(Pipe& pipe_sender,
                        Pipe& pipe_receiver,
                        HANDLE orig_handle,
                        LPCVOID buffer,
                        DWORD number_of_bytes_to_write,
                        LPDWORD number_of_bytes_written);
DWORD execute_get_file_type(Pipe& pipe_sender,
                            Pipe& pipe_receiver,
                            HANDLE handle);

BOOL execute_close_handle(Pipe& pipe_sender,
                          Pipe& pipe_receiver,
                          HANDLE handle);
void execute_exit(Pipe& pipe_sender);
