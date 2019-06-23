#pragma once

#include <gmock/gmock.h>

#include "CommandManager.hpp"
#include "CommandProcessor.hpp"
#include "Pipe.hpp"
#include "plugin/FuncExecutor.hpp"
#include "plugin/SaveAsAdminImpl.hpp"

#include <future>
#include <memory>
#include <thread>

struct ExecutionThread {
  std::unique_ptr<Pipe> pipe_sender;
  std::unique_ptr<Pipe> pipe_receiver;
  std::thread thread;
  int execution_result = 0;
  CommandManager command_manager;

  ExecutionThread(bool auto_start = true) {
    if (auto_start)
      start();
  }

  ~ExecutionThread() { stop(true); }

  void start() {
    pipe_sender = Pipe::create_unique();
    pipe_receiver = Pipe::create_unique();
    start(pipe_sender->get_name(), pipe_receiver->get_name());
    pipe_sender->wait();
    pipe_receiver->wait();
    SetLastError(ERROR_SUCCESS);
  }

  void start(const std::wstring& pipe_sender_name,
             const std::wstring& pipe_receiver_name) {
    thread = std::thread([this, pipe_sender_name = pipe_sender_name,
                          pipe_receiver_name = pipe_receiver_name]() {
      execution_result = process_commands(command_manager, pipe_sender_name,
                                          pipe_receiver_name);
      EXPECT_NE(FailedToOpenPipe, execution_result);
      if (FailedToOpenPipe == execution_result) {
        std::terminate();
      }
    });
  }

  void stop(bool call_execute_exit) {
    if (thread.joinable()) {
      auto future = std::async(std::launch::async, &std::thread::join, &thread);
      if (call_execute_exit) {
        EXPECT_NE(pipe_sender, nullptr);
        execute_exit(*pipe_sender);
      }
      EXPECT_NE(future.wait_for(std::chrono::seconds(5)),
                std::future_status::timeout);
      if (thread.joinable()) {
        std::terminate();
      }
    }
  }
  int wait() {
    stop(false);
    return execution_result;
  }
};
