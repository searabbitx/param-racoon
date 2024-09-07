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

Response HttpClient::MakeRequest(const Target& target,
                                 const string_map_t& params) {
  if (target.Method() == "GET") {
    return MakeRequest(target.Url(), params, {{}}, target.Headers(),
                       target.Agent(), target.Cookies(), target.Method(),
                       target.Data(), target.Proxy(), target.AbsoluteUri());
  } else {
    return MakeRequest(target.Url(), {{}}, params, target.Headers(),
                       target.Agent(), target.Cookies(), target.Method(),
                       target.Data(), target.Proxy(), target.AbsoluteUri());
  }
}

static std::string CreateBodyString(const std::string& data,
                                    const string_map_t& additional_params) {
  if (additional_params.empty()) {
    return data;
  }
  std::string result{};
  if (!data.empty()) {
    result += data;
    result += '&';
  }
  for (const auto& [key, value] : additional_params) {
    result += key;
    result += '=';
    result += value;
    result += '&';
  }
  result.pop_back();
  return result;
}

Response HttpClient::MakeRequest(
    const std::string& host, const string_map_t& query,
    const string_map_t& body_params, const string_vec_t& headers,
    const std::string& agent, const std::string& cookies,
    const std::string& method, const std::string& data,
    const std::string& proxy, const std::string& absolute_uri) {
  curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, method.c_str());
  curl_easy_setopt(curl_, CURLOPT_URL,
                   CreateFullUrl(host, absolute_uri, query).c_str());
  if (!proxy.empty()) {
    curl_easy_setopt(curl_, CURLOPT_PROXY, proxy.c_str());
  }
  curl_easy_setopt(curl_, CURLOPT_USERAGENT, agent.c_str());

  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
  std::string content{};
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &content);

  std::string body{CreateBodyString(data, body_params)};
  if (!body.empty()) {
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());
  }

  curl_slist* list{nullptr};
  SetHeaders(list, headers);

  SetCookies(cookies);

  PerformRequest();

  curl_slist_free_all(list);
  return CreateResponse(std::move(content));
}

std::string HttpClient::CreateFullUrl(const std::string& host,
                                      const std::string& absolute_uri,
                                      const string_map_t& query) {
  CURLU* url{curl_url()};
  curl_url_set(url, CURLUPART_URL, host.c_str(), 0);

  SetAbsoluteUri(url, absolute_uri);

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

void HttpClient::SetAbsoluteUri(CURLU* url, const std::string& absolute_uri) {
  if (absolute_uri.empty()) {
    return;
  }

  auto query_delim{absolute_uri.find('?')};
  if (query_delim != std::string::npos) {
    auto path{absolute_uri.substr(0, query_delim)};
    auto query{absolute_uri.substr(query_delim + 1)};
    curl_url_set(url, CURLUPART_PATH, path.c_str(), 0);
    curl_url_set(url, CURLUPART_QUERY, query.c_str(), 0);
  } else {
    curl_url_set(url, CURLUPART_PATH, absolute_uri.c_str(), 0);
  }
}

void HttpClient::SetHeaders(curl_slist* list, const string_vec_t& headers) {
  for (const auto& header : headers) {
    list = curl_slist_append(list, header.c_str());
  }
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, list);
}

void HttpClient::SetCookies(const std::string& cookies) {
  curl_easy_setopt(curl_, CURLOPT_COOKIE, cookies.c_str());
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