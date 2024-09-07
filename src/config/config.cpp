#include "config/config.h"

#include <curl/curl.h>

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

#include "config/target.h"

Config::Config(Target target) : target_{std::move(target)} {}

const Target& Config::ATarget() const { return target_; }

std::string Config::WordlistPath() const { return wordlist_path_; }

std::string Config::Match() const { return match_; }

std::string Config::Filter() const { return filter_; }

short Config::Threads() const { return threads_; }

short Config::MinDiff() const { return min_diff_; }

bool Config::Quiet() const { return quiet_; }

static void Err(const std::string& error,
                const po::options_description& odesc) {
  std::cout << "Error:\n  " << error << "\n\n" << odesc << '\n';
  std::exit(1);
}

template <typename T>
void SetValue(T& dest, const std::string& name, po::variables_map& vm) {
  if (vm.count(name) != 0U) {
    dest = vm[name].as<T>();
  }
}

void SetValue(bool& dest, const std::string& name, po::variables_map& vm) {
  if (vm.count(name) != 0U) {
    dest = true;
  }
}

template <typename T>
void SetRequiredValue(T& dest, const std::string& name, po::variables_map& vm,
                      const po::options_description& odesc) {
  if (vm.count(name) != 0U) {
    dest = vm[name].as<T>();
  } else {
    Err(name + " is missing", odesc);
  }
}

template <typename T>
void SetValue(T& dest, const std::string& name, po::variables_map& vm,
              T default_val) {
  dest = default_val;
  SetValue<T>(dest, name, vm);
}

static bool IsUrlValid(const std::string& url) {
  CURLU* h{curl_url()};
  CURLUcode uc{curl_url_set(h, CURLUPART_URL, url.c_str(), 0)};
  curl_url_cleanup(h);
  return uc == CURLUcode::CURLUE_OK;
}

void Validate(const Config& config, const po::options_description& odesc) {
  if (!IsUrlValid(config.target_.url_)) {
    Err("invalid url", odesc);
  }
  if (!std::filesystem::exists(config.wordlist_path_)) {
    Err("specified wordlist file ('" + config.wordlist_path_ +
            "') does not exist.",
        odesc);
  }
  if (!std::filesystem::is_regular_file(config.wordlist_path_) &&
      !std::filesystem::is_symlink(config.wordlist_path_)) {
    Err("specified wordlist ('" + config.wordlist_path_ + "') is not a file.",
        odesc);
  }
  if (!config.match_.empty() && !config.filter_.empty()) {
    Err("match and filter options are mutually exlusive!", odesc);
  }
}

Config CreateConfigFromCliArgs(int argc, char** argv) {
  const std::string usage_head{
      "Usage:\n"
      "  param-racoon [options] url\n"
      "\n"
      "Allowed options"};
  po::options_description odesc(usage_head);

  odesc.add_options()("help,h", "produce help message")(
      "threads,t", po::value<short>(), "set number of threads (default: 10)")(
      "wordlist,w", po::value<std::string>(), "set path to the wordlist file")(
      "header,H", po::value<std::vector<std::string>>(), "set a header")(
      "agent,a", po::value<std::string>(), "set user agent")(
      "cookies,c", po::value<std::string>(),
      "set cookies (in \"NAME1=VAL1; NAME2=VAL2\" format)")(
      "method,X", po::value<std::string>(),
      "set request method (default: GET)")(
      "data,d", po::value<std::string>(),
      "set data to be sent in request body")(
      "match,m", po::value<std::string>(),
      "match a string in response to recognize valid params")(
      "filter,f", po::value<std::string>(),
      "filter out responses with a string to recognize valid params")(
      "min-diff,D", po::value<short>(),
      "minimal difference in response length that is required to consider "
      "response 'different' (default: 1)")(
      "quiet,q", "do not print the banner and configuration table")(
      "url", po::value<std::string>(), "set target url");
  po::positional_options_description pdesc;
  pdesc.add("url", 1);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv)
                  .options(odesc)
                  .positional(pdesc)
                  .run(),
              vm);
    po::notify(vm);
  } catch (std::exception& e) {
    Err(e.what(), odesc);
  }

  if (vm.count("help") != 0U) {
    std::cout << odesc << "\n";
    std::exit(0);
  }

  Config config{Target()};

  SetRequiredValue<std::string>(config.target_.url_, "url", vm, odesc);
  SetRequiredValue<std::string>(config.wordlist_path_, "wordlist", vm, odesc);
  SetValue<short>(config.threads_, "threads", vm);
  SetValue<short>(config.min_diff_, "min-diff", vm);
  SetValue<std::vector<std::string>>(config.target_.headers_, "header", vm);
  SetValue<std::string>(config.target_.agent_, "agent", vm);
  SetValue<std::string>(config.target_.cookies_, "cookies", vm);
  SetValue<std::string>(config.target_.data_, "data", vm);
  SetValue<std::string>(config.match_, "match", vm);
  SetValue<std::string>(config.filter_, "filter", vm);
  SetValue<std::string>(config.target_.method_, "method", vm, "GET");
  SetValue(config.quiet_, "quiet", vm);

  Validate(config, odesc);

  return config;
}