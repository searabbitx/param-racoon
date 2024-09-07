#include <curl/curl.h>

#include <iostream>
#include <string>

#include "http/http_client.h"
#include "http/response.h"
#include "task/param_finding_task.h"
#include "wordlist/wordlist.h"

int main() {
  curl_global_init(CURL_GLOBAL_ALL);

  std::cout << "🦝 Hello." << std::endl;

  Wordlist wordlist{"test/wordlist/test.txt"};
  HttpClient client{};
  const std::string url{"http://localhost:8888/test_params.php"};
  ParamFindingTask task{url, wordlist, client};

  task.Run();

  curl_global_cleanup();
}