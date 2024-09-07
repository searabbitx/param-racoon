#pragma once

#include <string>
#include <vector>

#include "http/http_client.h"
#include "task/probe.h"

class ParamTest {
 private:
  HttpClient client_{};
  const std::string& url_;
  const std::string& param_;
  const Probe& probe_;
  std::vector<std::string>& results_;

 public:
  ParamTest(const std::string& url, const std::string& param,
            const Probe& probe, std::vector<std::string>& results);
  void Run();

 private:
  bool CheckParam();
};