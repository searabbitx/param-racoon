#pragma once

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

 private:
  void PrintProgress() const;
};

}  // namespace logging