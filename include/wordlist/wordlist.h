#pragma once

#include <fstream>
#include <string>

class Wordlist {
 private:
  std::ifstream wordlist_file_;
  long total_{};

 public:
  explicit Wordlist(const std::string& path);
  std::string NextWord();
  bool HasMore();
  long Total() const;

 private:
  static long CalculateTotal(const std::string& path);
};