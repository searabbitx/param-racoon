#include "http/response.h"

#include <string>

Response::Response(long code, long downloaded_bytes, std::string content)
    : code_{code},
      downloaded_bytes_{downloaded_bytes},
      content_{std::move(content)} {}

long Response::Code() const { return code_; }
long Response::DownloadedBytes() const { return downloaded_bytes_; }
std::string_view Response::Content() const { return content_; }