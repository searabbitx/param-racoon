#include "task/param_finding_task.h"

#include <string>
#include <vector>

#include "http/http_client.h"

ParamFindingTask::ParamFindingTask(const std::string& url, Wordlist& wordlist,
                                   HttpClient& http_client)
    : url_(url), wordlist_(wordlist), http_client_(http_client) {}

std::vector<std::string> ParamFindingTask::Run() {
  Response probe{http_client_.Get(url_)};

  std::vector<std::string> results{};
  while (wordlist_.HasMore()) {
    auto param{wordlist_.NextWord()};
    if (testParam(param, probe)) {
      results.push_back(param);
    }
  }

  return results;
}

bool ParamFindingTask::testParam(const std::string& param,
                                 const Response& probe) {
  const string_map_t query{{param, "testvalue"}};
  auto res{http_client_.Get(url_, query)};
  return res.DownloadedBytes() != probe.DownloadedBytes();
}
