#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>

#include "http/http_client.h"
#include "wordlist/wordlist.h"

class ParamFindingTask {
 private:
  const std::string& url_;
  Wordlist& wordlist_;
  HttpClient http_client_{};
  boost::asio::io_service io_;
  boost::asio::io_service::work work_;

 public:
  ParamFindingTask(const std::string& url, Wordlist& wordlist);
  std::vector<std::string> Run();

 private:
  void CreateThreads(boost::thread_group& threads);
  void PostTests(Response& probe, std::vector<std::string>& results);
  std::function<void()> CreateParamTestFunction(
      const std::string& param, const Response& probe,
      std::vector<std::string>& results);
};