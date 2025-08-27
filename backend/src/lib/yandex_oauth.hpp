#pragma once

#include <chrono>
#include <iomanip>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <string_view>

#include <userver/clients/http/client.hpp>
#include <userver/http/url.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include "../models/user_data.hpp"

namespace yandex_auth {

namespace lib {

class YandexAuthService {
 public:
  YandexAuthService(const userver::storages::postgres::ClusterPtr& pg_cluster,
                    userver::clients::http::Client& http_client,
                    const std::string& client_id,
                    const std::string& client_secret,
                    const std::string& redirect_uri);

  std::string GenerateAuthUrl();
  std::optional<yandex_auth::models::TUserData> HandleCallback(
      const std::string& code);

 private:
  std::string GenerateState();
  std::optional<std::string> GetTokenForCode(const std::string& code);
  std::optional<yandex_auth::models::TUserData> GetUserInfo(
      const std::string& token);

  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::clients::http::Client& http_client_;
  std::string client_id_;
  std::string client_secret_;
  std::string redirect_uri_;
  std::mt19937 rng_;

  static constexpr std::string_view YANDEX_AUTH_URL =
      "https://oauth.yandex.ru/authorize";
  static constexpr std::string_view YANDEX_TOKEN_URL =
      "https://oauth.yandex.ru/token";
  static constexpr std::string_view YANDEX_INFO_URL =
      "https://login.yandex.ru/info";
};

}  // namespace lib

}  // namespace yandex_auth