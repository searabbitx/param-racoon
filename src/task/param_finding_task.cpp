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

std::mutex pending_tasks_mtx;

ParamFindingTask::ParamFindingTask(const Config& config, Wordlist& wordlist)
    : config_{config},
      wordlist_{wordlist},
      probe_{CreateProbe(config.ATarget())},
      io_{},
      work_{io_} {}

std::vector<std::string> ParamFindingTask::Run() {
  results_ = std::vector<std::string>();
  auto pending_tasks{wordlist_.Total()};
  auto progress{logging::Progress(pending_tasks)};
  progress.Start();

  boost::thread_group threads;
  CreateThreads(threads);

  PostTests(pending_tasks, progress);

  threads.join_all();
  return results_;
}

void ParamFindingTask::CreateThreads(boost::thread_group& threads) {
  for (short i = 0; i < config_.Threads(); ++i) {
    threads.create_thread([this] { io_.run(); });
  }
}

void ParamFindingTask::PostTests(long& pending_tasks,
                                 logging::Progress& progress) {
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    io_.post(CreateParamTestFunction(param, pending_tasks, progress));
  }
}

handler_t ParamFindingTask::CreateParamTestFunction(
    const std::string& param, long& pending_tasks,
    logging::Progress& progress) {
  return [=, &pending_tasks, &progress]() {
    ParamTest(config_, param, probe_, results_).Run();

    std::lock_guard<std::mutex> guard{pending_tasks_mtx};
    --pending_tasks;
    progress.Advance();
    if (pending_tasks == 0) {
      io_.stop();
    }
  };
}