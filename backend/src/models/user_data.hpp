#pragma once

#include <optional>
#include <string>

#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace yandex_auth {

namespace models {

struct TUserData {
  std::string first_name;
  std::string last_name;
  std::string display_name;
  std::string real_name;
  std::string login;
  std::string sex;
  std::string id;
  std::string client_id;
  std::string psuid;
};

userver::formats::json::Value Serialize(
    const TUserData& data,
    userver::formats::serialize::To<userver::formats::json::Value>);

TUserData Parse(const userver::formats::json::Value& value,
                userver::formats::parse::To<TUserData>);

}  // namespace models

}  // namespace yandex_auth