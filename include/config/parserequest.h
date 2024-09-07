#pragma once

#include <string>

#include "config/config.h"

void ParseRequest(std::string_view request_file, Config& config);