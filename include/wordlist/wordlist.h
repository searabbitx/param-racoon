#pragma once

#include <fstream>
#include <string>

class Wordlist {
 private:
  std::ifstream wordlist_file_;

 public:
  explicit Wordlist(const std::string& path);
  std::string NextWord();
  bool HasMore();
};