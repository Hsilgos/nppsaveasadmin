#include "Pipe.h"

#include <set>
//////////////////////////////////////////////////////////////////////////

namespace {
std::wstring generate_random_string(int length) {
  static bool is_initialized = false;
  if (!is_initialized)
    srand(GetTickCount());

  const wchar_t random_map[] = L"1234567890";
  const size_t map_size = sizeof(random_map) / sizeof(random_map[0]);

  std::wstring result;
  result.reserve(length);

  for (int i = 0; i < length; ++i)
    result.append(1, random_map[rand() * (map_size - 1) / RAND_MAX]);

  return result;
}

const int MaxBufferSize = 1024 * 4;
}  // namespace

//////////////////////////////////////////////////////////////////////////

Pipe::Pipe(HANDLE pipe, std::wstring name)
    : m_pipe(pipe), m_name(std::move(name)) {}

Pipe::~Pipe() {
  if (m_pipe && m_pipe != INVALID_HANDLE_VALUE) {
    CloseHandle(m_pipe);
  }
}

std::unique_ptr<Pipe> Pipe::create(HANDLE pipe, std::wstring name) {
  return std::unique_ptr<Pipe>(new Pipe(pipe, name));
}

std::unique_ptr<Pipe> Pipe::create(const std::wstring& name) {
  HANDLE pipe = CreateNamedPipe(
      name.c_str(),                                  // pipe's name
      PIPE_ACCESS_DUPLEX /*|FILE_FLAG_OVERLAPPED*/,  //
      PIPE_TYPE_BYTE, 1, MaxBufferSize, MaxBufferSize, 10000, NULL);

  if (pipe == INVALID_HANDLE_VALUE)
    return nullptr;

  return create(pipe, name);
}

std::unique_ptr<Pipe> Pipe::create_unique() {
  std::unique_ptr<Pipe> result;
  do {
    std::wstring name = L"\\\\.\\pipe\\npp_";
    name += generate_random_string(5);
    result = create(name);
  } while (!result);

  return result;
}

std::unique_ptr<Pipe> Pipe::open(const std::wstring& name) {
  HANDLE pipe =
      CreateFile(name.c_str(),                  // pipe's name
                 GENERIC_READ | GENERIC_WRITE,  // only need read access
                 FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                 FILE_ATTRIBUTE_NORMAL, NULL);

  if (pipe == INVALID_HANDLE_VALUE)
    return nullptr;

  return create(pipe, name);
}

bool Pipe::read(std::vector<char>& data) {
  DWORD read_size = 0;
  data.resize(MaxBufferSize);
  const DWORD buffer_size = static_cast<DWORD>(data.size());
  const bool result =
      TRUE == ReadFile(m_pipe, data.data(), buffer_size, &read_size, NULL);
  data.resize(result ? read_size : 0);
  return result;
}

bool Pipe::write(const std::vector<char>& data) {
  DWORD written = 0;
  const DWORD buffer_size = static_cast<DWORD>(data.size());
  const BOOL result =
      WriteFile(m_pipe, data.data(), buffer_size, &written, NULL);
  return TRUE == result && written == data.size();
}

const std::wstring& Pipe::get_name() const {
  return m_name;
}

bool Pipe::wait() const {
  return TRUE == ConnectNamedPipe(m_pipe, NULL);
}
