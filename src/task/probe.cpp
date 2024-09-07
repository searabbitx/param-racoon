#include "task/probe.h"

#include <string>

#include "config/target.h"
#include "http/http_client.h"
#include "http/response.h"

using sv = std::string_view;

long Probe::OriginalResponseLen() const { return original_response_len_; };
long Probe::OriginalResponseCode() const { return original_response_code_; };
bool Probe::AreParametersReflected() const {
  return are_parameters_reflected_;
};

const std::string kParamNameCanary{"abcdefghijklmn123"};
const std::string kParamValueCanary{"pqrstuvwyz321"};

static bool AreParametersReflected(HttpClient& client, const Target& target) {
  string_map_t params{{kParamNameCanary, kParamValueCanary}};
  auto res{client.MakeRequest(target, params)};
  auto content{res.Content()};
  return sv::npos != content.find(kParamNameCanary) &&
         sv::npos != content.find(kParamValueCanary);
}

Probe CreateProbe(const Target& target) {
  auto client{HttpClient()};
  auto probe{Probe()};
  auto res{client.MakeRequest(target)};
  probe.original_response_len_ = res.DownloadedBytes();
  probe.original_response_code_ = res.Code();
  probe.are_parameters_reflected_ = AreParametersReflected(client, target);
  return probe;
}