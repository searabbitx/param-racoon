#include <curl/curl.h>

#include <iostream>

#include "http/http_client.h"

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  std::cout << "Hello." << std::endl;

  HttpClient client{};
  long code{client.Get("https://www.google.com/")};
  std::cout << "Response code: " << code << std::endl;
}