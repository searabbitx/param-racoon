#include "http/response.h"

Response::Response(long code, long downloaded_bytes)
    : code_(code), downloaded_bytes_(downloaded_bytes) {}

long Response::Code() const { return code_; }
long Response::DownloadedBytes() const { return downloaded_bytes_; }