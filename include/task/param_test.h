#pragma once

#include <string>
#include <vector>

#include "config/target.h"
#include "http/http_client.h"
#include "task/probe.h"

class ParamTest {
 private:
  HttpClient client_{};
  const Target& target_;
  const std::string& param_;
  const Probe& probe_;
  std::vector<std::string>& results_;

 public:
  ParamTest(const Target& target, const std::string& param, const Probe& probe,
            std::vector<std::string>& results);
  void Run();

 private:
  bool CheckParam();
  bool CompareWithProbeResponseLen();
  bool CheckForReflectedParams();
  std::string CreateMalformedParamOfTheSameLen();
};