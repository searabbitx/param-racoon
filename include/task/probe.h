#pragma once

#include <string>

#include "config/target.h"
#include "http/response.h"

class Probe {
 private:
  long original_response_len_;
  bool are_parameters_reflected_;
  Probe() = default;

 public:
  [[nodiscard]] long OriginalResponseLen() const;
  [[nodiscard]] bool AreParametersReflected() const;
  friend Probe CreateProbe(const Target& target);
};

Probe CreateProbe(const Target& target);
