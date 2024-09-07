#include "task/param_finding_task.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "http/http_client.h"
#include "task/param_test.h"

std::mutex pending_tasks_mtx;

ParamFindingTask::ParamFindingTask(const std::string& url, Wordlist& wordlist,
                                   short threads)
    : url_{url},
      wordlist_{wordlist},
      threads_{threads},
      probe_{HttpClient().Get(url)},
      io_{},
      work_{io_} {}

std::vector<std::string> ParamFindingTask::Run() {
  results_ = std::vector<std::string>();

  boost::thread_group threads;
  CreateThreads(threads);

  long pending_tasks{wordlist_.Total()};
  PostTests(pending_tasks);

  threads.join_all();
  return results_;
}

void ParamFindingTask::CreateThreads(boost::thread_group& threads) {
  for (short i = 0; i < threads_; ++i) {
    threads.create_thread([this] { io_.run(); });
  }
}

void ParamFindingTask::PostTests(long& pending_tasks) {
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    io_.post(CreateParamTestFunction(param, pending_tasks));
  }
}

handler_t ParamFindingTask::CreateParamTestFunction(const std::string& param,
                                                    long& pending_tasks) {
  return [=, &pending_tasks]() {
    ParamTest(url_, param, probe_, results_).Run();

    std::lock_guard<std::mutex> guard{pending_tasks_mtx};
    --pending_tasks;
    if (pending_tasks == 0) {
      io_.stop();
    }
  };
}