#include "config/config.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

std::string Config::Url() const { return url_; }

short Config::Threads() const { return threads_; }

Config CreateConfigFromCliArgs(int argc, char **argv) {
  const std::string usage_head{
      "Usage:\n"
      "  param-racoon [options] url\n"
      "\n"
      "Allowed options"};
  po::options_description odesc(usage_head);

  odesc.add_options()("help,h", "produce help message")(
      "threads,t", po::value<short>(), "set number of threads (default: 10)")(
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
    std::cout << "Error:\n  url is missing\n\n" << odesc << '\n';
    std::exit(1);
  }

  if (vm.count("threads") != 0U) {
    config.threads_ = vm["threads"].as<short>();
  }

  return config;
}