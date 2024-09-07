#pragma once

#include <curl/curl.h>

#include <map>
#include <string>
#include <vector>

#include "config/target.h"
#include "http/response.h"

using string_map_t = std::map<std::string, std::string>;
using string_vec_t = std::vector<std::string>;

class HttpClient {
 private:
  CURL* curl_;

 public:
  HttpClient();
  HttpClient(const HttpClient& other) = delete;
  HttpClient& operator=(const HttpClient& other) = delete;
  ~HttpClient();
  Response Get(const std::string& host,
               const string_map_t& query = string_map_t(),
               const string_vec_t& headers = string_vec_t());

  Response Get(const Target& target,
               const string_map_t& query = string_map_t());

 private:
  static std::string CreateFullUrl(const std::string& host,
                                   const string_map_t& query);
  void PerformRequest();
  Response CreateResponse(std::string content);
};