#pragma once

#include <string>

class Config;

class Target {
 private:
  std::string url_{};
  Target() = default;

 public:
  [[nodiscard]] std::string Url() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};