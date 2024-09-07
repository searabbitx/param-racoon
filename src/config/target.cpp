#include "config/target.h"

#include <string>

std::string Target::Url() const { return url_; }
string_vec_t Target::Headers() const { return headers_; }
std::string Target::Cookies() const { return cookies_; }
std::string Target::Method() const { return method_; }