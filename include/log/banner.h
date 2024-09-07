#pragma once

#include "config/config.h"
#include "wordlist/wordlist.h"

namespace logging {

void PrintBanner(const Config& config, const Wordlist& wordlist);

}