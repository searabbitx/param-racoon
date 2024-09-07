#pragma once

#include <string>
#include <vector>

#include "config/config.h"
#include "http/http_client.h"
#include "task/probe.h"

class ParamTest {
 private:
  HttpClient client_{};
  const Config& config_;
  const std::string& param_;
  const Probe& probe_;
  std::vector<std::string>& results_;

 public:
  ParamTest(const Config& config, const std::string& param, const Probe& probe,
            std::vector<std::string>& results);
  void Run();

 private:
  bool CheckParam();
  bool CheckForMatchString();
  bool CheckForFilterString();
  bool CompareWithProbeResponseLen();
  bool CheckForReflectedParams();
  std::string CreateMalformedParamOfTheSameLen();
};