#pragma once

#include <string>

constexpr short kDefaultThreads{10};

class Config {
 private:
  std::string url_{};
  short threads_{kDefaultThreads};
  Config() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] short Threads() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};

Config CreateConfigFromCliArgs(int argc, char **argv);