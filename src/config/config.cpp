#include "config/config.h"

#include <curl/curl.h>

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

#include "config/target.h"

namespace po = boost::program_options;

Config::Config(Target target) : target_{std::move(target)} {}

const Target& Config::ATarget() const { return target_; }

std::string Config::WordlistPath() const { return wordlist_path_; }

short Config::Threads() const { return threads_; }

static void Err(const std::string& error,
                const po::options_description& odesc) {
  std::cout << "Error:\n  " << error << "\n\n" << odesc << '\n';
  std::exit(1);
}

static bool IsUrlValid(const std::string& url) {
  CURLU* h{curl_url()};
  CURLUcode uc{curl_url_set(h, CURLUPART_URL, url.c_str(), 0)};
  curl_url_cleanup(h);
  return uc == CURLUcode::CURLUE_OK;
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
      "cookies,c", po::value<std::string>(),
      "set cookies (in \"NAME1=VAL1; NAME2=VAL2\" format)")(
      "match,m", po::value<std::string>(),
      "match a string in resopnse to recognize valid params")(
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

  if (vm.count("url") != 0U) {
    config.target_.url_ = vm["url"].as<std::string>();
    if (!IsUrlValid(config.target_.url_)) {
      Err("invalid url", odesc);
    }
  } else {
    Err("url is missing", odesc);
  }

  if (vm.count("wordlist") != 0U) {
    config.wordlist_path_ = vm["wordlist"].as<std::string>();
    if (!std::filesystem::exists(config.wordlist_path_)) {
      Err("specified wordlist file ('" + config.wordlist_path_ +
              ")' does not exist.",
          odesc);
    }
  } else {
    Err("wordlist is missing", odesc);
  }

  if (vm.count("threads") != 0U) {
    config.threads_ = vm["threads"].as<short>();
  }

  if (vm.count("header") != 0U) {
    config.target_.headers_ = vm["header"].as<std::vector<std::string>>();
  }

  if (vm.count("cookies") != 0U) {
    config.target_.cookies_ = vm["cookies"].as<std::string>();
  }

  return config;
}