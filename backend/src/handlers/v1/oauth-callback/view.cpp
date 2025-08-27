#include "view.hpp"

namespace yandex_auth {

class YandexOauthCallback final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-oauth-callback";

  YandexOauthCallback(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()),
        http_client_(
            component_context.FindComponent<userver::components::HttpClient>()
                .GetHttpClient()),
        service_(pg_cluster_, http_client_,
                 config["client_id"].As<std::string>(),
                 config["client_secret"].As<std::string>(),
                 config["redirect_uri"].As<std::string>()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    lib::add_headers(request);
    auto json_body = userver::formats::json::FromString(request.RequestBody());

    if (!json_body.HasMember("code")) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return R"({"error": "code is required"})";
    }

    auto code = json_body["code"].As<std::string>();
    auto callback_result = service_.HandleCallback(code);

    if (!callback_result) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return R"({"error": "failed to fetch user data"})";
    }

    return userver::formats::json::ToString(
        userver::formats::json::ValueBuilder{*callback_result}.ExtractValue());
  }

  static userver::yaml_config::Schema GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<
        userver::server::handlers::HttpHandlerBase>(R"(
            type: object
            description: OAuth initialization handler config
            additionalProperties: false
            properties:
                client_id:
                    type: string
                    description: YandexID OAuth client ID
                client_secret:
                    type: string
                    description: YandexID OAuth client secret
                redirect_uri:
                    type: string
                    description: OAuth redirect URI registered with YandexID
            )");
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::clients::http::Client& http_client_;
  mutable lib::YandexAuthService service_;
};

void AppendAuthCallback(userver::components::ComponentList& component_list) {
  component_list.Append<YandexOauthCallback>();
}

}  // namespace yandex_auth