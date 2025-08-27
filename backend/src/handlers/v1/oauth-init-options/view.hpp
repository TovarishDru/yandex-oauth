#pragma once
#include <format>

#include <userver/clients/http/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "../../../lib/headers.hpp"

namespace yandex_auth {

void AppendAuthInitOptions(userver::components::ComponentList& component_list);

}  // namespace yandex_auth