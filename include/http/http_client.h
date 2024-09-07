#pragma once

#include <curl/curl.h>

#include <string>

class HttpClient {
 private:
  CURL* curl_;

 public:
  HttpClient();
  ~HttpClient();
  long Get(const std::string& url);
};