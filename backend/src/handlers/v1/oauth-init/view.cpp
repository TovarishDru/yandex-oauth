#include "view.hpp"

namespace yandex_auth {

class YandexOauthInit final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-oauth-init";

  YandexOauthInit(
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

    auto auth_url = service_.GenerateAuthUrl();

    std::string loc_header = "Location";
    request.SetResponseStatus(userver::server::http::HttpStatus::kFound);
    request.GetHttpResponse().SetHeader(loc_header, auth_url);
    return {};
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

void AppendAuthInit(userver::components::ComponentList& component_list) {
  component_list.Append<YandexOauthInit>();
}

}  // namespace yandex_auth