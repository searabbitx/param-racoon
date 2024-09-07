#include "config/parserequest.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

std::string ParseRequest(const std::string& request_file_path, Config& config) {
  if (!fs::exists(request_file_path)) {
    return "Request file: '" + request_file_path + "' does not exist";
  }

  std::ifstream request_stream{request_file_path};

  std::string request_line{};
  std::getline(request_stream, request_line);
  auto first_space{request_line.find(' ')};
  auto second_space{request_line.find(' ', first_space + 1)};
  config.target_.method_ =
      std::string(&request_line[0], &request_line[first_space]);
  config.target_.absolute_uri_ =
      std::string(&request_line[first_space + 1], &request_line[second_space]);

  std::string header{};
  std::getline(request_stream, header);
  while (!header.empty()) {
    config.target_.headers_.push_back(header);
    std::getline(request_stream, header);
  }

  return "";
}