#pragma once

#include <string>

#include "http/response.h"

class Probe {
 private:
  long original_response_len_;
  bool are_parameters_reflected_;
  Probe() = default;

 public:
  [[nodiscard]] long OriginalResponseLen() const;
  [[nodiscard]] bool AreParametersReflected() const;
  friend Probe CreateProbe(const std::string& url);
};

Probe CreateProbe(const std::string& url);
