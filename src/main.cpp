#include <curl/curl.h>

#include <iostream>
#include <map>
#include <string>

#include "http/http_client.h"
#include "http/response.h"

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  std::cout << "Hello." << std::endl;

  HttpClient client{};
  const std::map<std::string, std::string> query{{"foo", "bar"}};
  const Response response{
      client.Get("http://localhost:8888/test_params.php", query)};
  std::cout << "Response code: " << response.Code() << std::endl;
  std::cout << "Response length: " << response.DownloadedBytes() << std::endl;
}