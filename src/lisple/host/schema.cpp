
#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/host/schema.h>
#include <lisple/runtime/dict.h>

namespace Lisple
{
  MapSchema::MapSchema(const KVEntryMap& required_keys, const KVEntryMap& optional_keys)
    : required(required_keys)
    , optional(optional_keys)
  {
  }

  MapSchema::Inspector MapSchema::bind(Context& ctx, RTValue& map)
  {
    std::unordered_set<std::string> map_keys = Lisple::Dict::map_string_keys(map);
    std::unordered_map<std::string, sptr_rtval> transformed;

    for (auto& [k, type] : required)
    {
      if (map_keys.count(k))
      {
        sptr_rtval val = Lisple::Dict::get_property(map, k);
        if (!type->is_type_of(*val) && val->type != RTValue::Type::NIL)
        {
          auto coercion = type->coerce(ctx, val);
          if (coercion.success)
          {
            transformed.emplace(k, coercion.result);
          }
          else
          {
            throw Lisple::TypeError("Invalid type of value " + val->to_string() +
                                    " for key :" + k + " in " + map.to_string() +
                                    ". Expected: " + type->to_string());
          }
        }
      }
      else
      {
        throw Lisple::TypeError("Map " + map.to_string() + " is missing required key :" + k +
                                " of type " + type->to_string() + ".");
      }
    }

    for (auto& [k, type] : optional)
    {
      if (map_keys.count(k))
      {
        sptr_rtval val = Lisple::Dict::get_property(map, k);
        if (!type->is_type_of(*val) && val->type != RTValue::Type::NIL)
        {
          auto coercion = type->coerce(ctx, val);
          if (coercion.success)
          {
            transformed.emplace(k, coercion.result);
          }
          else
          {
            throw Lisple::TypeError("Invalid type of value " + val->to_string() +
                                    " for key :" + k + " in " + map.to_string() +
                                    ". Expected: " + type->to_string());
          }
        }
      }
    }

    return MapSchema::Inspector(*this, map, transformed);
  }

  MapSchema::Inspector::Inspector(const MapSchema& schema,
                                  RTValue& map,
                                  std::unordered_map<std::string, sptr_rtval> overrides)
    : schema(schema)
    , source(map)
    , override_map(overrides)
  {
  }

  sptr_rtval MapSchema::Inspector::_get_value(const std::string& key,
                                              RTValue::Type type) const
  {
    sptr_rtval value;
    if (override_map.count(key))
    {
      value = override_map.at(key);
    }
    else
    {
      value = Dict::get_property(source, key);
    }

    if (value->type == RTValue::Type::NIL)
    {
      return nullptr;
    }
    if (value->type != type)
    {
      throw Lisple::TypeError("Incorrect type of value for :" + key + " in " +
                              source.to_string());
    }

    return value;
  }

  sptr_rtval MapSchema::Inspector::_get_value(const std::string& key) const
  {
    sptr_rtval value;
    if (override_map.count(key))
    {
      value = override_map.at(key);
    }
    else
    {
      value = Dict::get_property(source, key);
    }

    if (value->type == RTValue::Type::NIL)
    {
      return nullptr;
    }

    return value;
  }

  sptr_rtval MapSchema::Inspector::_get_value_or_throw(const std::string& key) const
  {
    sptr_rtval value;
    if (override_map.count(key))
    {
      value = override_map.at(key);
    }
    else
    {
      value = Dict::get_property_or_throw(source, key);
    }

    return value;
  }

  sptr_rtval MapSchema::Inspector::_get_value_or_throw(const std::string& key,
                                                       RTValue::Type type) const
  {
    sptr_rtval value;
    if (override_map.count(key))
    {
      value = override_map.at(key);
    }
    else
    {
      value = Dict::get_property_or_throw(source, key);
    }

    if (value->type != type)
    {
      throw Lisple::TypeError("Incorrect type of value for :" + key + " in " +
                              source.to_string());
    }

    return value;
  }

  bool MapSchema::Inspector::contains(const std::string& key) const
  {
    return override_map.count(key) || Dict::contains_key(source, key);
  }

  sptr_rtval MapSchema::Inspector::val(const std::string& key) const
  {
    if (override_map.count(key)) return override_map.at(key);
    return Dict::get_property(source, key);
  }

  bool MapSchema::Inspector::boolean(const std::string& key) const
  {
    return std::get<bool>(_get_value_or_throw(key, RTValue::Type::BOOL)->value);
  }

  bool MapSchema::Inspector::boolean(const std::string& key, bool default_value) const
  {
    sptr_rtval value = _get_value(key, RTValue::Type::BOOL);
    if (value)
    {
      return std::get<bool>(value->value);
    }
    return default_value;
  }

  uint8_t MapSchema::Inspector::ui8(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value).get_int();
  }

  uint8_t MapSchema::Inspector::ui8(const std::string& key, uint8_t default_value) const
  {
    sptr_rtval value = _get_value(key, RTValue::Type::NUMBER);
    if (value)
    {
      return std::get<const RTValue::Number>(value->value).get_int();
    }
    return default_value;
  }

  const RTValue::Number& MapSchema::Inspector::num(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value);
  }

  int MapSchema::Inspector::i32(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value).get_int();
  }
  long MapSchema::Inspector::i64(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value).get_long();
  }
  float MapSchema::Inspector::f32(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value).get_float();
  }
  float MapSchema::Inspector::f32(const std::string& key, float default_value) const
  {
    if (sptr_rtval value = _get_value(key, RTValue::Type::NUMBER))
    {
      return std::get<const RTValue::Number>(value->value).get_float();
    }
    return default_value;
  }
  double MapSchema::Inspector::f64(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key, RTValue::Type::NUMBER);
    return std::get<const RTValue::Number>(value->value).get_double();
  }

  const std::string& MapSchema::Inspector::str(const std::string& key) const
  {
    sptr_rtval value = _get_value_or_throw(key);

    if (std::string* str = std::get_if<std::string>(&value->value))
    {
      return *str;
    }

    throw TypeError("Value for key :" + key + " is not string-like");
  }

  const std::string& MapSchema::Inspector::str(const std::string& key,
                                               const std::string& default_value) const
  {
    sptr_rtval value = _get_value(key);

    if (std::string* str = std::get_if<std::string>(&value->value))
    {
      return *str;
    }

    return default_value;
  }

} // namespace Lisple
