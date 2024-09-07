#pragma once

#include <string>
#include <vector>

#include "http/http_client.h"
#include "http/response.h"

class ParamTest {
 private:
  HttpClient client_{};
  const std::string& url_;
  const std::string& param_;
  const Response& probe_;
  std::vector<std::string>& results_;

 public:
  ParamTest(const std::string& url, const std::string& param,
            const Response& probe, std::vector<std::string>& results);
  void Run();
};