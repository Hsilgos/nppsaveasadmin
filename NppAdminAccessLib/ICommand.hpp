#pragma once

#include <memory>
#include <vector>
#include "Common.hpp"

class IWinApiFunctions;

class ICommand {
 public:
  virtual bool execute(const std::vector<char>& data,
                       std::vector<char>& ret_data) = 0;
  ICommand(IWinApiFunctions& original_functions);
  virtual ~ICommand();

 protected:
  IWinApiFunctions& m_winapi;
};

template <class T>
const T* get_command_data_from_vector(const std::vector<char>& data) {
  if (data.size() != sizeof(PacketWithCommand<T>))
    return nullptr;
  const auto* packet_with_command =
      reinterpret_cast<const PacketWithCommand<T>*>(data.data());
  return &packet_with_command->commandData;
}
