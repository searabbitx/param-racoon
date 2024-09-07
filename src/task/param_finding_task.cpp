#include "task/param_finding_task.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "http/http_client.h"
#include "task/param_test.h"

constexpr int kThreadCount{4};

ParamFindingTask::ParamFindingTask(const std::string& url, Wordlist& wordlist)
    : url_(url), wordlist_(wordlist), io_{}, work_{io_} {}

std::vector<std::string> ParamFindingTask::Run() {
  Response probe{http_client_.Get(url_)};
  std::vector<std::string> results{};

  boost::thread_group threads;
  CreateThreads(threads);

  PostTests(probe, results);

  threads.join_all();
  return results;
}

void ParamFindingTask::CreateThreads(boost::thread_group& threads) {
  for (std::size_t i = 0; i < kThreadCount; ++i) {
    threads.create_thread([this] { io_.run(); });
  }
}

void ParamFindingTask::PostTests(Response& probe,
                                 std::vector<std::string>& results) {
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    io_.post(CreateParamTestFunction(param, probe, results));
  }
}

std::function<void()> ParamFindingTask::CreateParamTestFunction(
    const std::string& param, const Response& probe,
    std::vector<std::string>& results) {
  return [=, &results]() {
    std::cout << "Running for " << param << '\n';
    ParamTest(url_, param, probe, results).Run();
    if (param == "last10") {
      io_.stop();
    }
  };
}