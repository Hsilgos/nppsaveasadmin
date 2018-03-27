#pragma once

#include <vector>
#include "Common.h"

class ICommand {
 public:
  virtual bool execute(const std::vector<char>& data,
                       std::vector<char>& ret_data) = 0;
  ICommand();
  virtual ~ICommand();
};

template <class T>
const T* get_command_data_from_vector(const std::vector<char>& data) {
  if (data.size() != sizeof(PacketWithCommand<T>))
    return nullptr;
  const auto* packet_with_command =
      reinterpret_cast<const PacketWithCommand<T>*>(data.data());
  return &packet_with_command->commandData;
}
