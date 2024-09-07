#include "task/param_test.h"

#include <mutex>

#include "task/probe.h"

std::mutex results_mtx;

ParamTest::ParamTest(const Target& target, const std::string& param,
                     const Probe& probe, std::vector<std::string>& results)
    : target_{target}, param_{param}, probe_{probe}, results_{results} {};

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
  return probe_.AreParametersReflected() ? CheckForReflectedParams()
                                         : CompareWithProbeResponseLen();
}

bool ParamTest::CompareWithProbeResponseLen() {
  const string_map_t query{{param_, "testvalue"}};
  auto res{client_.Get(target_, query)};
  return res.DownloadedBytes() != probe_.OriginalResponseLen();
}

std::string ParamTest::CreateMalformedParamOfTheSameLen() {
  return std::string(param_.length(), 'X');
}

bool ParamTest::CheckForReflectedParams() {
  auto param_response{client_.Get(target_, CreateQuery(param_))};
  auto malformed_param_response{
      client_.Get(target_, CreateQuery(CreateMalformedParamOfTheSameLen()))};
  return param_response.DownloadedBytes() !=
         malformed_param_response.DownloadedBytes();
}