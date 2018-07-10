#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include <vector>

class Pipe final {
  const HANDLE m_pipe;
  const std::wstring m_name;

  Pipe(HANDLE pipe, std::wstring name);
  static std::unique_ptr<Pipe> create(HANDLE pipe, std::wstring name);

 public:
  ~Pipe();

  static std::unique_ptr<Pipe> create(const std::wstring& name);
  static std::unique_ptr<Pipe> create_unique();
  static std::unique_ptr<Pipe> open(const std::wstring& name);

  bool read(std::vector<char>& data);
  bool write(const std::vector<char>& data);
  const std::wstring& get_name() const;
  bool wait() const;
};
