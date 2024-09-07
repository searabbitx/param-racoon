#include "task/probe.h"

#include <string>

#include "http/http_client.h"
#include "http/response.h"

using sv = std::string_view;

long Probe::OriginalResponseLen() const { return original_response_len_; };
bool Probe::AreParametersReflected() const {
  return are_parameters_reflected_;
};

const std::string kParamNameCanary{"abcdefghijklmn123"};
const std::string kParamValueCanary{"pqrstuvwyz321"};

bool AreParametersReflected(HttpClient& client, const std::string& url) {
  string_map_t params{{kParamNameCanary, kParamValueCanary}};
  auto res{client.Get(url, params)};
  auto content{res.Content()};
  return sv::npos != content.find(kParamNameCanary) &&
         sv::npos != content.find(kParamValueCanary);
}

Probe CreateProbe(const std::string& url) {
  auto client{HttpClient()};
  auto probe{Probe()};
  probe.original_response_len_ = client.Get(url).DownloadedBytes();
  probe.are_parameters_reflected_ = AreParametersReflected(client, url);
  return probe;
}