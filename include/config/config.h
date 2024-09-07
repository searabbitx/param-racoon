#pragma once

#include <string>

constexpr short kDefaultThreads{10};

class Config {
 private:
  std::string url_{};
  std::string wordlist_path_{};
  short threads_{kDefaultThreads};
  Config() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] std::string WordlistPath() const;
  [[nodiscard]] short Threads() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};

Config CreateConfigFromCliArgs(int argc, char **argv);