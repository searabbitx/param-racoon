#include "log/banner.h"

#include <iostream>
#include <string>

static void printBannerTableEntry(std::string_view name,
                                  std::string_view value) {
  if (value.empty()) {
    return;
  }
  std::cout << "🦝 " << name << ": " << value << '\n';
}

static void printBannerTableEntry(std::string_view name, long value) {
  printBannerTableEntry(name, std::to_string(value));
}

static void printBannerTableEntry(std::string_view name,
                                  const string_vec_t& vec) {
  if (vec.empty()) {
    return;
  }
  std::cout << "🦝 " << name << ": ";
  for (const auto& value : vec) {
    std::cout << value << "\n            ";
  }
}

void logging::printBanner(const Config& config, const Wordlist& wordlist) {
  std::cout << "🦝 Hello.\n";
  std::cout << "🦝🦝🦝🦝\n";
  printBannerTableEntry("url", config.ATarget().Url());
  printBannerTableEntry("method", config.ATarget().Method());
  printBannerTableEntry("wordlist", config.WordlistPath());
  printBannerTableEntry("words", wordlist.Total());
  printBannerTableEntry("threads", config.Threads());
  printBannerTableEntry("headers", config.ATarget().Headers());
  printBannerTableEntry("cookies", config.ATarget().Cookies());
  printBannerTableEntry("match", config.Match());
  printBannerTableEntry("filter", config.Filter());
  std::cout << '\n';
}