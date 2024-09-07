#include "http/http_client.h"

#include <curl/curl.h>

#include <iostream>
#include <string>

#include "http/response.h"

HttpClient::HttpClient() : curl_(curl_easy_init()) {}

HttpClient::~HttpClient() { curl_easy_cleanup(curl_); }

size_t WriteCallback(const char *contents, size_t size, size_t nmemb,
                     void *userp) {
  (void)contents;
  (void)userp;
  return size * nmemb;
}

Response HttpClient::Get(const std::string &url) {
  CURLcode res;

  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);

  res = curl_easy_perform(curl_);

  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
              << '\n';
  }

  long response_code{0};
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);

  curl_off_t downloaded_bytes{0};
  res = curl_easy_getinfo(curl_, CURLINFO_SIZE_DOWNLOAD_T, &downloaded_bytes);

  return Response(response_code, static_cast<long>(downloaded_bytes));
}