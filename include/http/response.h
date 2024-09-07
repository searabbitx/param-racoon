#pragma once

class Response {
 private:
  const long code_;
  const long downloaded_bytes_;

 public:
  Response(long code, long downloaded_bytes);
  [[nodiscard]] long Code() const;
  [[nodiscard]] long DownloadedBytes() const;
};