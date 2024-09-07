#include "task/param_test.h"

#include <mutex>

#include "task/probe.h"

using sv = std::string_view;

std::mutex results_mtx;

ParamTest::ParamTest(const Config& config, const std::string& param,
                     const Probe& probe, std::vector<std::string>& results)
    : config_{config}, param_{param}, probe_{probe}, results_{results} {};

void ParamTest::Run() {
  if (CheckParam()) {
    std::lock_guard<std::mutex> guard{results_mtx};
    results_.push_back(param_);
  }
}

static string_map_t CreateQuery(const std::string& param) {
  string_map_t query{{param, "testvalue"}};
  return query;
}

bool ParamTest::CheckParam() {
  if (!config_.Match().empty()) {
    return CheckForMatchString();
  }
  if (probe_.AreParametersReflected()) {
    return CheckForReflectedParams();
  }
  return CompareWithProbeResponseLen();
}

bool ParamTest::CheckForMatchString() {
  auto res{client_.Get(config_.ATarget(), CreateQuery(param_))};
  return sv::npos != res.Content().find(config_.Match());
}

bool ParamTest::CompareWithProbeResponseLen() {
  const string_map_t query{{param_, "testvalue"}};
  auto res{client_.Get(config_.ATarget(), query)};
  return res.DownloadedBytes() != probe_.OriginalResponseLen();
}

std::string ParamTest::CreateMalformedParamOfTheSameLen() {
  return std::string(param_.length(), 'X');
}

bool ParamTest::CheckForReflectedParams() {
  auto target{config_.ATarget()};
  auto param_response{client_.Get(target, CreateQuery(param_))};
  auto malformed_param_response{
      client_.Get(target, CreateQuery(CreateMalformedParamOfTheSameLen()))};
  return param_response.DownloadedBytes() !=
         malformed_param_response.DownloadedBytes();
}