#pragma once

#include <string>

namespace logging {

class Progress {
 private:
  long total_{};
  long current_{};
  bool done_{false};

 public:
  explicit Progress(long total);
  void Start();
  void Advance();
  [[nodiscard]] bool IsDone() const;
  void LogParam(std::string_view param);

 private:
  void PrintProgress() const;
};

}  // namespace logging