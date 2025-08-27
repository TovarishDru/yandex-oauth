#include "view.hpp"

namespace yandex_auth {

class YandexOauthCallbackOptions final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-oauth-callback-options";

  YandexOauthCallbackOptions(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    lib::add_headers(request);
    return {};
  }
};

void AppendAuthCallbackOptions(
    userver::components::ComponentList& component_list) {
  component_list.Append<YandexOauthCallbackOptions>();
}

}  // namespace yandex_auth