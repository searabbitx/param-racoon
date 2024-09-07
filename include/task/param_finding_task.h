#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>

#include "task/probe.h"
#include "wordlist/wordlist.h"

using handler_t = std::function<void()>;

class ParamFindingTask {
 private:
  const std::string url_;
  Wordlist& wordlist_;
  short threads_;
  Probe probe_;
  boost::asio::io_service io_;
  boost::asio::io_service::work work_;
  std::vector<std::string> results_{};

 public:
  ParamFindingTask(const std::string& url, Wordlist& wordlist, short threads);
  std::vector<std::string> Run();

 private:
  void CreateThreads(boost::thread_group& threads);
  void PostTests(long& pending_tasks);
  handler_t CreateParamTestFunction(const std::string& param,
                                    long& pending_tasks);
};