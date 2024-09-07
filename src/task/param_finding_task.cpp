#include "task/param_finding_task.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "http/http_client.h"
#include "task/param_test.h"

constexpr int kThreadCount{10};

std::mutex pending_tasks_mtx;

ParamFindingTask::ParamFindingTask(const std::string& url, Wordlist& wordlist)
    : url_(url), wordlist_(wordlist), io_{}, work_{io_} {}

std::vector<std::string> ParamFindingTask::Run() {
  Response probe{http_client_.Get(url_)};
  results_ = std::vector<std::string>();

  boost::thread_group threads;
  CreateThreads(threads);

  long pending_tasks{wordlist_.Total()};
  PostTests(probe, pending_tasks);

  threads.join_all();
  return results_;
}

void ParamFindingTask::CreateThreads(boost::thread_group& threads) {
  for (std::size_t i = 0; i < kThreadCount; ++i) {
    auto& io{io_};
    threads.create_thread([&io] { io.run(); });
  }
}

void ParamFindingTask::PostTests(Response& probe, long& pending_tasks) {
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    io_.post(CreateParamTestFunction(param, probe, pending_tasks));
  }
}

handler_t ParamFindingTask::CreateParamTestFunction(const std::string& param,
                                                    const Response& probe,
                                                    long& pending_tasks) {
  return [=, &pending_tasks]() {
    ParamTest(url_, param, probe, results_).Run();

    std::lock_guard<std::mutex> guard{pending_tasks_mtx};
    --pending_tasks;
    if (pending_tasks == 0) {
      io_.stop();
    }
  };
}