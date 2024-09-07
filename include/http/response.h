#pragma once

#include <string>

class Response {
 private:
  const long code_;
  const long downloaded_bytes_;
  const std::string content_;

 public:
  Response(long code, long downloaded_bytes, std::string content);
  Response(const Response& other) = delete;
  Response& operator=(const Response& other) = delete;
  [[nodiscard]] long Code() const;
  [[nodiscard]] long DownloadedBytes() const;
  [[nodiscard]] std::string_view Content() const;
};