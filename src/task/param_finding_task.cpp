#include "task/param_finding_task.h"

#include <string>

ParamFindingTask::ParamFindingTask(const std::string& url, Wordlist& wordlist,
                                   HttpClient& http_client)
    : url_(url), wordlist_(wordlist), http_client_(http_client) {}

void ParamFindingTask::Run() {
  while (wordlist_.HasMore()) {
    const string_map_t query{{wordlist_.NextWord(), "testvalue"}};
    http_client_.Get(url_, query);
  }
}