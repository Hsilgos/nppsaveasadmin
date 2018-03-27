#include "CommandProcessor.h"

#include "CommandManager.h"
#include "Common.h"
#include "ICommand.h"
#include "Pipe.h"

#include <string>
#include <vector>

Commands get_command(const std::vector<char>& buffer) {
  if (buffer.size() < sizeof(Commands))
    return UnknownCmd;

  const Commands* command = reinterpret_cast<const Commands*>(buffer.data());

  if (*command <= UnknownCmd || *command >= MaxCmd)
    return UnknownCmd;

  return *command;
}

int process_commands(CommandManager& cmd_manager,
                     const std::wstring& pipe_name_to_read,
                     const std::wstring& pipe_name_to_write) {
  auto pipe_read = Pipe::open(pipe_name_to_read);
  auto pipe_write = Pipe::open(pipe_name_to_write);

  if (!pipe_read || !pipe_write)
    return FailedToOpenPipe;

  // The read operation will block until there is data to read
  std::vector<char> buffer;
  std::vector<char> result_buffer;

  while (pipe_read->read(buffer)) {
    if (!buffer.empty()) {
      const Commands cmd_num = get_command(buffer);
      if (ExitCmd == cmd_num)
        break;

      ICommand* const command = cmd_manager.find_commad(cmd_num);
      if (!command)
        return UnknownCommand;

      if (!command->execute(buffer, result_buffer))
        return BadDataInBuffer;

      if (result_buffer.empty())
        return ReturnBufferHasNoData;

      if (!pipe_write->write(result_buffer))
        return FailedToWritePipe;
    }
  }

  return NoError;
}
