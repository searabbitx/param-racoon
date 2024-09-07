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

static string_map_t CreateParams(const std::string& param) {
  string_map_t params{{param, "testvalue"}};
  return params;
}

bool ParamTest::CheckParam() {
  if (!config_.Match().empty()) {
    return CheckForMatchString();
  }
  if (!config_.Filter().empty()) {
    return CheckForFilterString();
  }
  if (probe_.AreParametersReflected()) {
    return CheckForReflectedParams();
  }
  return CompareWithProbeResponseLen();
}

static bool matches(std::string_view str, std::string_view match) {
  return sv::npos != str.find(match);
}

bool ParamTest::CheckForMatchString() {
  auto res{MakeRequest(CreateParams(param_))};
  return matches(res.Content(), config_.Match());
}

bool ParamTest::CheckForFilterString() {
  auto res{MakeRequest(CreateParams(param_))};
  return !matches(res.Content(), config_.Filter());
}

bool ParamTest::CompareWithProbeResponseLen() {
  auto res{MakeRequest(CreateParams(param_))};
  return res.DownloadedBytes() != probe_.OriginalResponseLen();
}

std::string ParamTest::CreateMalformedParamOfTheSameLen() {
  return std::string(param_.length(), 'X');
}

bool ParamTest::CheckForReflectedParams() {
  auto target{config_.ATarget()};
  auto param_response{MakeRequest(CreateParams(param_))};
  auto malformed_param_response{
      MakeRequest(CreateParams(CreateMalformedParamOfTheSameLen()))};
  return param_response.DownloadedBytes() !=
         malformed_param_response.DownloadedBytes();
}

Response ParamTest::MakeRequest(const string_map_t& params) {
  return client_.MakeRequest(config_.ATarget(), params);
}