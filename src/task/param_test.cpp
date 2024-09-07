#include "task/param_test.h"

#include <mutex>

#include "task/probe.h"

std::mutex results_mtx;

ParamTest::ParamTest(const std::string& url, const std::string& param,
                     const Probe& probe, std::vector<std::string>& results)
    : url_(url), param_(param), probe_(probe), results_(results){};

void ParamTest::Run() {
  if (CheckParam()) {
    std::lock_guard<std::mutex> guard{results_mtx};
    results_.push_back(param_);
  }
}

bool ParamTest::CheckParam() {
  const string_map_t query{{param_, "testvalue"}};
  auto res{client_.Get(url_, query)};
  return res.DownloadedBytes() != probe_.OriginalResponseLen();
}