#pragma once

#include <string>

#include "config/config.h"

std::string ParseRequest(const std::string& request_file_path, Config& config);