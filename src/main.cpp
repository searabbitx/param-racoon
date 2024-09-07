#include <curl/curl.h>

#include <iostream>
#include <string>

#include "config/config.h"
#include "task/param_finding_task.h"
#include "wordlist/wordlist.h"

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  const Config config{CreateConfigFromCliArgs(argc, argv)};

  std::cout << "🦝 Hello.\n";
  std::cout << "🦝🦝🦝🦝\n";
  std::cout << "🦝 url: " << config.Url() << '\n';
  std::cout << "🦝 threads: " << config.Threads() << "\n\n";

  const std::string url{config.Url()};
  Wordlist wordlist{"test/wordlist/test.txt"};
  ParamFindingTask task{url, wordlist, config.Threads()};

  const auto params{task.Run()};

  for (const auto& param : params) {
    std::cout << "🦝🦝 Found param: " << param << '\n';
  }

  curl_global_cleanup();
}