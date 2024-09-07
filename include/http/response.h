#pragma once

class Response {
 private:
  const long code_;

 public:
  explicit Response(long code);
  [[nodiscard]] long Code() const;
};