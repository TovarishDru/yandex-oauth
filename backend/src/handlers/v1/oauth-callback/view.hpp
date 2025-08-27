#pragma once
#include <format>

#include <userver/clients/http/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/component_list.hpp>
#include <userver/formats/json.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "../../../lib/headers.hpp"
#include "../../../lib/yandex_oauth.hpp"
#include "../../../models/user_data.hpp"

namespace yandex_auth {

void AppendAuthCallback(userver::components::ComponentList& component_list);

}  // namespace yandex_auth