#pragma once

#include <string>

#include "config/target.h"

constexpr short kDefaultThreads{10};

class Config {
 private:
  Target target_;
  std::string wordlist_path_{};
  std::string match_{};
  short threads_{kDefaultThreads};
  explicit Config(Target target);

 public:
  [[nodiscard]] const Target &ATarget() const;
  [[nodiscard]] std::string WordlistPath() const;
  [[nodiscard]] std::string Match() const;
  [[nodiscard]] short Threads() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};

Config CreateConfigFromCliArgs(int argc, char **argv);