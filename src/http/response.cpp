#include "http/response.h"

Response::Response(long code) : code_(code) {}

long Response::Code() const { return code_; }