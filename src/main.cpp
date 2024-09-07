#include <curl/curl.h>

#include <iostream>
#include <string>

#include "task/param_finding_task.h"
#include "wordlist/wordlist.h"

constexpr short kThreads{10};

int main() {
  curl_global_init(CURL_GLOBAL_ALL);

  std::cout << "🦝 Hello.\n\n";

  const std::string url{"http://localhost:8888/test_params.php"};
  Wordlist wordlist{"test/wordlist/test.txt"};
  ParamFindingTask task{url, wordlist, kThreads};

  const auto params{task.Run()};

  for (const auto& param : params) {
    std::cout << "🦝🦝 Found param: " << param << '\n';
  }

  curl_global_cleanup();
}