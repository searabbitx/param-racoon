#pragma once

#include <string>
#include <vector>

using string_vec_t = std::vector<std::string>;

class Config;

class Target {
 private:
  std::string url_{};
  std::string method_;
  string_vec_t headers_{};
  std::string cookies_{};
  Target() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] std::string Method() const;
  [[nodiscard]] string_vec_t Headers() const;
  [[nodiscard]] std::string Cookies() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};