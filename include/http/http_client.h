#pragma once

#include <curl/curl.h>

#include <string>

#include "http/response.h"

class HttpClient {
 private:
  CURL* curl_;

 public:
  HttpClient();
  ~HttpClient();
  Response Get(const std::string& url);
};