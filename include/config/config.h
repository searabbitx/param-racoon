#pragma once

#include <boost/program_options.hpp>
#include <string>

#include "config/target.h"

constexpr short kDefaultThreads{10};
constexpr short kDefaultDiff{1};

namespace po = boost::program_options;

class Config {
 private:
  Target target_;
  std::string wordlist_path_{};
  std::string match_{};
  std::string filter_{};
  short threads_{kDefaultThreads};
  short min_diff_{kDefaultDiff};
  explicit Config(Target target);

 public:
  [[nodiscard]] const Target &ATarget() const;
  [[nodiscard]] std::string WordlistPath() const;
  [[nodiscard]] std::string Match() const;
  [[nodiscard]] std::string Filter() const;
  [[nodiscard]] short Threads() const;
  [[nodiscard]] short MinDiff() const;
  friend Config CreateConfigFromCliArgs(int argc, char **argv);
  friend void Validate(const Config &config,
                       const po::options_description &odesc);
};

Config CreateConfigFromCliArgs(int argc, char **argv);