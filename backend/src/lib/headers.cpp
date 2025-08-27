#include "headers.hpp"
#include <cstdlib>
#include <string>

namespace yandex_auth {

namespace lib {

void add_headers(const userver::server::http::HttpRequest& request) {
  auto& response = request.GetHttpResponse();
  response.SetContentType("application/json");

  response.SetHeader(std::string("Access-Control-Allow-Origin"), "*");
  response.SetHeader(std::string("Access-Control-Allow-Methods"),
                     "GET, POST, PUT, DELETE, OPTIONS");
  response.SetHeader(
      std::string("Access-Control-Allow-Headers"),
      "Content-Type, Authorization, X-Requested-With, x-client-id");
  response.SetHeader(std::string("Access-Control-Allow-Credentials"), "true");
}

}  // namespace lib

}  // namespace yandex_auth