#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>

#include "config/config.h"
#include "log/progress.h"
#include "task/probe.h"
#include "wordlist/wordlist.h"

using handler_t = std::function<void()>;

class ParamFindingTask {
 private:
  const Config& config_;
  Wordlist& wordlist_;
  Probe probe_;
  boost::asio::io_service io_;
  boost::asio::io_service::work work_;
  std::vector<std::string> results_{};

 public:
  ParamFindingTask(const Config& config, Wordlist& wordlist);
  std::vector<std::string> Run();

 private:
  void CreateThreads(boost::thread_group& threads);
  void PostTests(logging::Progress& progress);
  handler_t CreateParamTestFunction(const std::string& param,
                                    logging::Progress& progress);
};