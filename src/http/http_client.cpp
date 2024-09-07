#include "http/http_client.h"

#include <curl/curl.h>

#include <iostream>
#include <map>
#include <string>

#include "http/response.h"

HttpClient::HttpClient() : curl_(curl_easy_init()) {}

HttpClient::~HttpClient() { curl_easy_cleanup(curl_); }

size_t WriteCallback(const char* contents, size_t size, size_t nmemb,
                     void* userp) {
  static_cast<std::string*>(userp)->append(contents, size * nmemb);
  return size * nmemb;
}

Response HttpClient::Get(const std::string& host, const string_map_t& query) {
  curl_easy_setopt(curl_, CURLOPT_URL, CreateFullUrl(host, query).c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
  std::string content{};
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &content);

  curl_slist* list{nullptr};
  list = curl_slist_append(list, "X-Required-Header: foo");
  list = curl_slist_append(list, "X-Other-Required-Header: foo");
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, list);

  PerformRequest();

  curl_slist_free_all(list);
  return CreateResponse(std::move(content));
}

std::string HttpClient::CreateFullUrl(const std::string& host,
                                      const string_map_t& query) {
  CURLU* url{curl_url()};
  curl_url_set(url, CURLUPART_URL, host.c_str(), 0);

  std::string query_entry;
  for (const auto& [key, value] : query) {
    query_entry = key;
    query_entry += "=";
    query_entry += value;
    curl_url_set(url, CURLUPART_QUERY, query_entry.c_str(), CURLU_APPENDQUERY);
  }

  char* buffer;
  curl_url_get(url, CURLUPART_URL, &buffer, 0);
  std::string result{buffer};
  curl_free(buffer);

  return result;
}

void HttpClient::PerformRequest() {
  auto res{curl_easy_perform(curl_)};
  if (res != CURLE_OK) {
    std::cerr << "Error:\n  Connection failed. " << curl_easy_strerror(res)
              << '\n';
    std::exit(1);
  }
}

Response HttpClient::CreateResponse(std::string content) {
  long response_code{0};
  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);

  curl_off_t downloaded_bytes{0};
  curl_easy_getinfo(curl_, CURLINFO_SIZE_DOWNLOAD_T, &downloaded_bytes);

  return Response(response_code, static_cast<long>(downloaded_bytes),
                  std::move(content));
}