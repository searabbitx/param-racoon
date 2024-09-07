#include "wordlist/wordlist.h"

#include <fstream>
#include <string>

Wordlist::Wordlist(const std::string& path) : wordlist_file_{path} {}

std::string Wordlist::NextWord() {
  std::string line;
  std::getline(wordlist_file_, line);
  return line;
}

bool Wordlist::HasMore() { return wordlist_file_.peek() != EOF; }