#pragma once

#include <string>
#include <vector>

#include "http/http_client.h"
#include "wordlist/wordlist.h"

class ParamFindingTask {
 private:
  const std::string& url_;
  Wordlist& wordlist_;
  HttpClient& http_client_;

 public:
  ParamFindingTask(const std::string& url, Wordlist& wordlist,
                   HttpClient& http_client);
  std::vector<std::string> Run();

 private:
  void testParam(const std::string& param, const Response& probe,
                 std::vector<std::string>& results);
};