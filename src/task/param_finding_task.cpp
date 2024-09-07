#include "task/param_finding_task.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "config/config.h"
#include "log/progress.h"
#include "task/param_test.h"
#include "task/probe.h"

std::mutex progress_mtx;

ParamFindingTask::ParamFindingTask(const Config& config, Wordlist& wordlist)
    : config_{config},
      wordlist_{wordlist},
      probe_{CreateProbe(config.ATarget())},
      io_{},
      work_{io_} {}

std::vector<std::string> ParamFindingTask::Run() {
  results_ = std::vector<std::string>();
  boost::thread_group threads;
  CreateThreads(threads);

  auto progress{logging::Progress(wordlist_.Total())};
  progress.Start();
  PostTests(progress);

  threads.join_all();
  return results_;
}

void ParamFindingTask::CreateThreads(boost::thread_group& threads) {
  for (short i = 0; i < config_.Threads(); ++i) {
    threads.create_thread([this] { io_.run(); });
  }
}

void ParamFindingTask::PostTests(logging::Progress& progress) {
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    io_.post(CreateParamTestFunction(param, progress));
  }
}

handler_t ParamFindingTask::CreateParamTestFunction(
    const std::string& param, logging::Progress& progress) {
  return [=, &progress]() {
    auto found{ParamTest(config_, param, probe_, results_).Run()};
    (void)found;
    std::lock_guard<std::mutex> guard{progress_mtx};
    progress.Advance();
    if (progress.IsDone()) {
      io_.stop();
    }
  };
}