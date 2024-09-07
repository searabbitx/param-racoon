#include <curl/curl.h>

#include <iostream>

#include "http/http_client.h"
#include "http/response.h"

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  std::cout << "Hello." << std::endl;

  HttpClient client{};
  const Response response{client.Get("https://www.google.com/")};
  std::cout << "Response code: " << response.Code() << std::endl;
  std::cout << "Response length: " << response.DownloadedBytes() << std::endl;
}