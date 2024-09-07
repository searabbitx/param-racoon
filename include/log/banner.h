#pragma once

#include "config/config.h"
#include "wordlist/wordlist.h"

namespace logging {

void printBanner(const Config& config, const Wordlist& wordlist);

}