#pragma once

#include <cstdlib>
#include <userver/server/http/http_request.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace yandex_auth {

namespace lib {

void add_headers(const userver::server::http::HttpRequest& request);

}  // namespace lib

}  // namespace yandex_auth