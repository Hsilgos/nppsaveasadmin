#include "NppSaveAsAdminImpl.h"

#include <map>
#include <string>
#include <vector>

#include "FileOperations.h"
#include "AdminAccess.h"
#include "FuncExecutor.h"

#include "Pipe.h"

namespace
{
  struct MyFileHandle
  {
    std::unique_ptr<Pipe> pipe_sender;
    std::unique_ptr<Pipe> pipe_receiver;
    HANDLE proc_handle;
    HANDLE origin_handle;
  };

  using HandleMap = std::map<HANDLE, std::unique_ptr<MyFileHandle>>;
  HandleMap	g_handles;

  NppSaveAsAdminBehaviour* g_behaviour = &DefaultNppSaveAsAdminBehaviour::instance();
}

FileFunctions &getFileFunctions();

void set_save_as_admin_behaviour(NppSaveAsAdminBehaviour& behaviour)
{
  g_behaviour = &behaviour;
}

bool DefaultNppSaveAsAdminBehaviour::process_file(const std::wstring& filename)
{
  return m_allowed_files.find(filename) != m_allowed_files.end();
}

std::wstring DefaultNppSaveAsAdminBehaviour::convert_path(const std::wstring& filename)
{
  return filename;
}

HANDLE DefaultNppSaveAsAdminBehaviour::run_admin_access(
  const std::wstring & pipe_sender_name,
  const std::wstring & pipe_receiver_name)
{
  return run_admin_access_app(pipe_sender_name, pipe_receiver_name);
}

void DefaultNppSaveAsAdminBehaviour::allow_process_file(const std::wstring& filename)
{
  if (!filename.empty())
    m_allowed_files.insert(filename);
}

void DefaultNppSaveAsAdminBehaviour::cancel_process_file(const std::wstring& filename)
{
  auto it = m_allowed_files.find(filename);
  if (it != m_allowed_files.end())
    m_allowed_files.erase(it);
}

DefaultNppSaveAsAdminBehaviour& DefaultNppSaveAsAdminBehaviour::instance()
{
  static DefaultNppSaveAsAdminBehaviour instance;
  return instance;
}

//////////////////////////////////////////////////////////////////////////

BOOL WINAPI MyWriteFileProc(HANDLE file_handle, LPCVOID buffer,
  DWORD number_of_bytes_to_write, LPDWORD number_of_bytes_written, LPOVERLAPPED overlapped)
{
	HandleMap::iterator it = g_handles.find(file_handle);
	if( it != g_handles.end() )
	{
		return execute_write_file(
      *it->second->pipe_sender,
      *it->second->pipe_receiver,
			it->second->origin_handle, 
			buffer,
			number_of_bytes_to_write, 
			number_of_bytes_written);
	}
	else
	{
		return getFileFunctions().write_file->call_original(
      file_handle,
      buffer,
      number_of_bytes_to_write,
      number_of_bytes_written,
      overlapped);
	}
}

HANDLE WINAPI MyCreateFileA(
							LPCSTR file_name,
							DWORD desired_access,
							DWORD share_mode,
							LPSECURITY_ATTRIBUTES security_attributes,
							DWORD creation_disposition,
							DWORD flags_and_attributes,
							HANDLE template_file)
{
	return getFileFunctions().create_filea->call_original(
		file_name, 
		desired_access,
		share_mode,
		security_attributes,
		creation_disposition,
		flags_and_attributes,
		template_file);
}

HANDLE WINAPI MyCreateFileW(
							 LPCWSTR file_name,
							 DWORD desired_access,
							 DWORD share_mode,
							 LPSECURITY_ATTRIBUTES security_attributes,
							 DWORD creation_disposition,
							 DWORD flags_and_attributes,
							 HANDLE template_file)
{
  HANDLE result = getFileFunctions().create_filew->call_original(
		file_name, 
		desired_access,
		share_mode,
		security_attributes,
		creation_disposition,
		flags_and_attributes,
		template_file);

	if( INVALID_HANDLE_VALUE == result && (desired_access & GENERIC_WRITE) )
	{
		const int error_code = GetLastError();
		if( ERROR_ACCESS_DENIED == error_code && g_behaviour->process_file(file_name) )
		{
			std::unique_ptr<MyFileHandle> new_handle = std::make_unique<MyFileHandle>();
      new_handle->pipe_sender = Pipe::create_unique();
      new_handle->pipe_receiver = Pipe::create_unique();
			if( !new_handle->pipe_sender || !new_handle->pipe_receiver )
			{
				return INVALID_HANDLE_VALUE;
			}

			new_handle->proc_handle = g_behaviour->run_admin_access(
        new_handle->pipe_sender->get_name(),
        new_handle->pipe_receiver->get_name());
			if( NULL == new_handle->proc_handle )
			{
				return INVALID_HANDLE_VALUE;
			}

			new_handle->pipe_sender->wait();
      new_handle->pipe_receiver->wait();

			new_handle->origin_handle = execute_create_file_w(
        *new_handle->pipe_sender,
        *new_handle->pipe_receiver,
				file_name,
				desired_access,
				share_mode, 
				creation_disposition,
				flags_and_attributes);
			if( INVALID_HANDLE_VALUE == new_handle->origin_handle )
			{
				return INVALID_HANDLE_VALUE;
			}

			g_handles[new_handle->proc_handle] = std::move(new_handle);
			result = new_handle->proc_handle;
		}
		
	}

	return result;
}

BOOL WINAPI MyCloseHandle(HANDLE handle)
{
	HandleMap::iterator it = g_handles.find(handle);
	if( it != g_handles.end() )
	{
		BOOL result = execute_close_handle(*it->second->pipe_sender,
      *it->second->pipe_receiver, it->second->origin_handle);
		execute_exit(*it->second->pipe_sender);
		g_handles.erase(it);
		return result;
	}
	else
	{
		return getFileFunctions().close_handle->call_original(handle);
	}
}

DWORD WINAPI MyGetFileType(HANDLE handle)
{
	HandleMap::iterator it = g_handles.find(handle);
	if( it != g_handles.end() )
		return execute_get_file_type(*it->second->pipe_sender, *it->second->pipe_receiver,
      it->second->origin_handle);
	else
		return getFileFunctions().get_file_type->call_original(handle);
}
