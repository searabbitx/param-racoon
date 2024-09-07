#include <curl/curl.h>

#include <iostream>
#include <string>

#include "task/param_finding_task.h"
#include "wordlist/wordlist.h"

int main() {
  curl_global_init(CURL_GLOBAL_ALL);

  std::cout << "🦝 Hello.\n\n";

  const std::string url{"http://localhost:8888/test_params.php"};
  Wordlist wordlist{"test/wordlist/test.txt"};
  HttpClient client{};
  ParamFindingTask task{url, wordlist, client};

  const auto params{task.Run()};

  for (const auto& param : params) {
    std::cout << "🦝🦝 Found param: " << param << '\n';
  }

  curl_global_cleanup();
}