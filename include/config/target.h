#pragma once

#include <string>
#include <vector>

using string_vec_t = std::vector<std::string>;

class Config;

class Target {
 public:
  enum class Method { kGet, kPost };

 private:
  std::string url_{};
  Method method_;
  string_vec_t headers_{};
  std::string cookies_{};
  Target() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] Method AMethod() const;
  [[nodiscard]] string_vec_t Headers() const;
  [[nodiscard]] std::string Cookies() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
};