#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include <vector>

#include "UniqueHandle.hpp"

class Pipe final {
  const UniqueHandle<> m_pipe;
  const std::wstring m_name;

  Pipe(UniqueHandle<> pipe, std::wstring name);
  static std::unique_ptr<Pipe> create(UniqueHandle<> pipe, std::wstring name);

 public:

  static std::unique_ptr<Pipe> create(const std::wstring& name);
  static std::unique_ptr<Pipe> create_unique();
  static std::unique_ptr<Pipe> open(const std::wstring& name);

  bool read(std::vector<char>& data);
  bool write(const std::vector<char>& data);
  const std::wstring& get_name() const;
  bool wait() const;
};
