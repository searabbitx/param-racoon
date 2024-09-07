#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <vector>

using string_vec_t = std::vector<std::string>;

class Config;

namespace po = boost::program_options;

class Target {
 private:
  std::string url_{};
  std::string method_;
  string_vec_t headers_{};
  std::string cookies_{};
  std::string data_{};
  Target() = default;

 public:
  [[nodiscard]] std::string Url() const;
  [[nodiscard]] std::string Method() const;
  [[nodiscard]] string_vec_t Headers() const;
  [[nodiscard]] std::string Cookies() const;
  [[nodiscard]] std::string Data() const;
  friend Config CreateConfigFromCliArgs(int argc, char** argv);
  friend void Validate(const Config& config,
                       const po::options_description& odesc);
};