#include "config/config.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

std::string Config::Url() const { return url_; }

std::string Config::WordlistPath() const { return wordlist_path_; }

short Config::Threads() const { return threads_; }

static void Err(const std::string& error,
                const po::options_description& odesc) {
  std::cout << "Error:\n  " << error << "\n\n" << odesc << '\n';
  std::exit(1);
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
      "url", po::value<std::string>(), "set target url");
  po::positional_options_description pdesc;
  pdesc.add("url", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(odesc)
                .positional(pdesc)
                .run(),
            vm);
  po::notify(vm);

  if (vm.count("help") != 0U) {
    std::cout << odesc << "\n";
    std::exit(0);
  }

  Config config{};

  if (vm.count("url") != 0U) {
    config.url_ = vm["url"].as<std::string>();
  } else {
    Err("url is missing", odesc);
  }

  if (vm.count("wordlist") != 0U) {
    config.wordlist_path_ = vm["wordlist"].as<std::string>();
  } else {
    Err("wordlist is missing", odesc);
  }

  if (vm.count("threads") != 0U) {
    config.threads_ = vm["threads"].as<short>();
  }

  return config;
}