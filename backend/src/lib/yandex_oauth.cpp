#include "yandex_oauth.hpp"

namespace yandex_auth {

namespace lib {

YandexAuthService::YandexAuthService(
    const userver::storages::postgres::ClusterPtr& pg_cluster,
    userver::clients::http::Client& http_client, const std::string& client_id,
    const std::string& client_secret, const std::string& redirect_uri)
    : pg_cluster_(pg_cluster),
      http_client_(http_client),
      client_id_(client_id),
      client_secret_(client_secret),
      redirect_uri_(redirect_uri),
      rng_(std::random_device{}()) {}

std::string YandexAuthService::GenerateState() {
  std::uniform_int_distribution<> dis(0, 16);
  std::ostringstream state;

  for (int i = 0; i < 32; ++i) {
    state << std::hex << dis(rng_);
  }

  return state.str();
}

std::string YandexAuthService::GenerateAuthUrl() {
  std::ostringstream url;

  url << YANDEX_AUTH_URL << "?response_type=code"
      << "&client_id=" << userver::http::UrlEncode(client_id_)
      << "&redirect_uri=" << userver::http::UrlEncode(redirect_uri_);

  return url.str();
}

std::optional<yandex_auth::models::TUserData> YandexAuthService::HandleCallback(
    const std::string& code) {
  auto user_token = GetTokenForCode(code);

  if (!user_token) {
    return std::nullopt;
  }

  auto user_data = GetUserInfo(*user_token);

  if (!user_data) {
    return std::nullopt;
  }

  return user_data;
}

std::optional<std::string> YandexAuthService::GetTokenForCode(
    const std::string& code) {
  try {
    std::ostringstream form_data;
    form_data << "grant_type=authorization_code"
              << "&code=" << userver::http::UrlEncode(code)
              << "&client_id=" << userver::http::UrlEncode(client_id_)
              << "&client_secret=" << userver::http::UrlEncode(client_secret_);

    auto request = http_client_.CreateRequest();
    request.url(std::string(YANDEX_TOKEN_URL));
    request.method(userver::clients::http::HttpMethod::kPost);
    request.headers({{"Content-Type", "application/x-www-form-urlencoded"}});
    request.data(form_data.str());
    request.timeout(std::chrono::seconds(1));

    auto response = request.perform();

    auto body = response->body();
    LOG_INFO() << "Token response: " << body;
    response->raise_for_status();

    auto json = userver::formats::json::FromString(response->body());

    if (json.HasMember("access_token")) {
      return json["access_token"].As<std::string>();
    }

    return std::nullopt;
  } catch (const std::exception& e) {
    LOG_ERROR() << "Failed to get user info: " << e.what();
    return std::nullopt;
  }
}

std::optional<yandex_auth::models::TUserData> YandexAuthService::GetUserInfo(
    const std::string& token) {
  try {
    auto request = http_client_.CreateRequest();
    request.url(std::string(YANDEX_INFO_URL));
    request.method(userver::clients::http::HttpMethod::kGet);
    request.headers({{"Authorization", "OAuth " + token}});
    request.timeout(std::chrono::seconds(1));

    auto response = request.perform();

    auto body = response->body();
    LOG_INFO() << "Token response: " << body;

    response->raise_for_status();

    auto json = userver::formats::json::FromString(response->body());
    return json.As<yandex_auth::models::TUserData>();
  } catch (const std::exception& e) {
    LOG_ERROR() << "Failed to get user info: " << e.what();
    return std::nullopt;
  }
}

}  // namespace lib

}  // namespace yandex_auth
