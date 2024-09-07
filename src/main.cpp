#include <curl/curl.h>

#include <iostream>

int main() {
  curl_global_init(CURL_GLOBAL_ALL);
  std::cout << "Hello." << std::endl;
  std::cout << "It seems to work!" << std::endl;
}