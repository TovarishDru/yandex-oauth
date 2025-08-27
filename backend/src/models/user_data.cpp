#include "user_data.hpp"

namespace yandex_auth {

namespace models {

TUserData Parse(const userver::formats::json::Value& value,
                userver::formats::parse::To<TUserData>) {
  TUserData data;
  data.first_name = value["first_name"].As<std::string>("");
  data.last_name = value["last_name"].As<std::string>("");
  data.display_name = value["display_name"].As<std::string>("");
  data.real_name = value["real_name"].As<std::string>("");
  data.login = value["login"].As<std::string>("");
  data.sex = value["sex"].As<std::string>("");
  data.id = value["id"].As<std::string>("");
  data.client_id = value["client_id"].As<std::string>("");
  data.psuid = value["psuid"].As<std::string>("");
  return data;
}

userver::formats::json::Value Serialize(
    const TUserData& data,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder item;
  item["first_name"] = data.first_name;
  item["last_name"] = data.last_name;
  item["display_name"] = data.display_name;
  item["real_name"] = data.real_name;
  item["login"] = data.login;
  item["sex"] = data.sex;
  item["id"] = data.id;
  item["client_id"] = data.client_id;
  item["psuid"] = data.psuid;
  return item.ExtractValue();
}

}  // namespace models

}  // namespace yandex_auth