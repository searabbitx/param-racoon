#include <curl/curl.h>

#include <iostream>
#include <string>

#include "config/config.h"
#include "log/banner.h"
#include "task/param_finding_task.h"
#include "wordlist/wordlist.h"

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  const Config config{CreateConfigFromCliArgs(argc, argv)};
  Wordlist wordlist{config.WordlistPath()};

  logging::printBanner(config, wordlist);

  ParamFindingTask task{config, wordlist};

  const auto params{task.Run()};

  for (const auto& param : params) {
    std::cout << "🦝🦝 Found param: " << param << '\n';
  }

  curl_global_cleanup();
}