#include "log/banner.h"

#include <iostream>
#include <string>

constexpr short kColLen{10};
const auto kEmptyCol{std::string(kColLen + 4, ' ')};

static void PrintHead() {
  std::cout << "🦝 Hello.\n";
  std::cout << "🦝🦝🦝🦝\n";
}

static std::string CreatePadding(std::size_t name_length) {
  auto padding_len{kColLen - name_length};
  return std::string(padding_len, ' ');
}

static void PrintBannerTableEntry(std::string_view name,
                                  std::string_view value) {
  if (value.empty()) {
    return;
  }
  std::cout << "🦝 " << name << ":" << CreatePadding(name.length()) << value
            << '\n';
}

static void PrintBannerTableEntry(std::string_view name, long value) {
  PrintBannerTableEntry(name, std::to_string(value));
}

static void PrintBannerTableEntry(std::string_view name,
                                  const string_vec_t& vec) {
  if (vec.empty()) {
    return;
  }
  std::cout << "🦝 " << name << ":" << CreatePadding(name.length());
  for (const auto& value : vec) {
    std::cout << value << "\n" << kEmptyCol;
  }
}

void logging::PrintBanner(const Config& config, const Wordlist& wordlist) {
  PrintHead();

  PrintBannerTableEntry("url", config.ATarget().Url());
  PrintBannerTableEntry("method", config.ATarget().Method());
  PrintBannerTableEntry("wordlist", config.WordlistPath());
  PrintBannerTableEntry("words", wordlist.Total());
  PrintBannerTableEntry("threads", config.Threads());
  PrintBannerTableEntry("headers", config.ATarget().Headers());
  PrintBannerTableEntry("cookies", config.ATarget().Cookies());
  PrintBannerTableEntry("match", config.Match());
  PrintBannerTableEntry("filter", config.Filter());

  std::cout << '\n';
}