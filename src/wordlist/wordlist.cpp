#include "wordlist/wordlist.h"

#include <algorithm>
#include <fstream>
#include <string>

Wordlist::Wordlist(const std::string& path)
    : wordlist_file_{path}, total_{CalculateTotal(path)} {}

std::string Wordlist::NextWord() {
  std::string line;
  std::getline(wordlist_file_, line);
  return line;
}

bool Wordlist::HasMore() { return wordlist_file_.peek() != EOF; }

long Wordlist::Total() const { return total_; }

long Wordlist::CalculateTotal(const std::string& path) {
  long result = 0;
  std::ifstream in{path};
  std::string unused;
  while (std::getline(in, unused)) {
    ++result;
  }
  return result;
}