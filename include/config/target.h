#pragma once

#include <string>
#include <vector>

using string_vec_t = std::vector<std::string>;

class Config;

class Target {
 private:
  std::string url_{};
  string_vec_t headers_{};
  Target() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] string_vec_t Headers() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};