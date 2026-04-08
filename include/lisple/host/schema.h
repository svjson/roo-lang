#ifndef LISPLE__HOST__SCHEMA_H
#define LISPLE__HOST__SCHEMA_H

#include <optional>
#include <unordered_map>

#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  class MapSchema
  {
    using KVEntryMap = std::unordered_map<std::string, const Lisple::TypeRef*>;

    KVEntryMap required;
    KVEntryMap optional;

   public:
    class Inspector
    {
      const MapSchema& schema;
      RTValue& source;
      std::unordered_map<std::string, sptr_rtval> override_map;

      Inspector(const MapSchema& schema,
                RTValue& map,
                std::unordered_map<std::string, sptr_rtval> overrides);

      sptr_rtval _get_value(const std::string& key, RTValue::Type type) const;
      sptr_rtval _get_value(const std::string& key) const;
      sptr_rtval _get_value_or_throw(const std::string& key, RTValue::Type type) const;
      sptr_rtval _get_value_or_throw(const std::string& key) const;

     public:
      bool contains(const std::string& key) const;

      sptr_rtval val(const std::string& key) const;
      bool boolean(const std::string& key) const;
      bool boolean(const std::string& key, bool default_value) const;
      uint8_t ui8(const std::string& key) const;
      uint8_t ui8(const std::string& key, uint8_t default_value) const;
      int i32(const std::string& key) const;
      int i32(const std::string& key, int default_value) const;
      long i64(const std::string& key) const;
      long i64(const std::string& key, long default_value) const;
      float f32(const std::string& key) const;
      float f32(const std::string& key, float default_value) const;
      double f64(const std::string& key) const;
      double f64(const std::string& key, double default_value) const;
      const std::string& str(const std::string& key) const;
      const std::string& str(const std::string& key, const std::string& default_value) const;

      template <typename T> T& obj(const std::string& key) const
      {
        if (override_map.count(key))
        {
          return Lisple::obj<T>(*override_map.at(key));
        }

        return Lisple::obj<T>(*Lisple::Dict::get_property(source, key));
      }

      template <typename T> T& obj(const std::string& key, T& default_value) const
      {
        if (override_map.count(key))
        {
          return Lisple::obj<T>(*override_map.at(key));
        }

        sptr_rtval obj_val = Lisple::Dict::get_property(source, key);
        if (obj_val && obj_val->type == RTValue::Type::OBJECT)
        {
          return Lisple::obj<T>(*Lisple::Dict::get_property(source, key));
        }

        return default_value;
      }

      template <typename T>
      const T& obj(const std::string& key, const T& default_value) const
      {
        if (override_map.count(key))
        {
          return Lisple::obj<T>(*override_map.at(key));
        }

        sptr_rtval obj_val = Lisple::Dict::get_property(source, key);
        if (obj_val && obj_val->type == RTValue::Type::OBJECT)
        {
          return Lisple::obj<T>(*Lisple::Dict::get_property(source, key));
        }

        return default_value;
      }

      template <typename T> std::optional<T> optional_obj(const std::string& key) const
      {
        if (override_map.count(key))
        {
          return Lisple::obj<T>(*override_map.at(key));
        }

        sptr_rtval obj_val = Lisple::Dict::get_property(source, key);
        if (obj_val && obj_val->type == RTValue::Type::OBJECT)
        {
          return Lisple::obj<T>(*Lisple::Dict::get_property(source, key));
        }

        return std::nullopt;
      }

      friend class MapSchema;
    };
    MapSchema(const KVEntryMap& required_keys, const KVEntryMap& optional_keys = {});

    Inspector bind(Context& ctx, RTValue& map);
  };
} // namespace Lisple

#endif /* LISPLE__HOST__SCHEMA_H */
