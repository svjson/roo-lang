#ifndef LISPLE__HOST__SCHEMA_H
#define LISPLE__HOST__SCHEMA_H

#include <optional>
#include <unordered_map>

#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/host/transform.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  template <typename T> struct RTValueReader
  {
    static T read(const sptr_rtval& value) { return Lisple::obj<T>(*value); }
  };

  template <> struct RTValueReader<sptr_rtval>
  {
    static sptr_rtval read(const sptr_rtval& value) { return value; }
  };

#define LISPLE_RTVALUE_READER(TYPE)                                      \
  template <>                                                            \
  struct RTValueReader<TYPE>                                             \
  {                                                                      \
    static TYPE read(const sptr_rtval& value)                            \
    {                                                                    \
      return Lisple::rtval_to<TYPE>(value);                              \
    }                                                                    \
  }

  LISPLE_RTVALUE_READER(bool);
  LISPLE_RTVALUE_READER(char);
  LISPLE_RTVALUE_READER(signed char);
  LISPLE_RTVALUE_READER(unsigned char);
  LISPLE_RTVALUE_READER(int);
  LISPLE_RTVALUE_READER(unsigned int);
  LISPLE_RTVALUE_READER(unsigned short);
  LISPLE_RTVALUE_READER(long);
  LISPLE_RTVALUE_READER(float);
  LISPLE_RTVALUE_READER(double);
  LISPLE_RTVALUE_READER(std::string);

#undef LISPLE_RTVALUE_READER

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
      const RTValue::Number& num(const std::string& key) const;
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
        if (obj_val && (obj_val->type == RTValue::Type::OBJECT ||
                        obj_val->type == RTValue::Type::NATIVE_OBJECT))
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
        if (obj_val && (obj_val->type == RTValue::Type::OBJECT ||
                        obj_val->type == RTValue::Type::NATIVE_OBJECT))
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
        if (obj_val && (obj_val->type == RTValue::Type::OBJECT ||
                        obj_val->type == RTValue::Type::NATIVE_OBJECT))
        {
          return Lisple::obj<T>(*Lisple::Dict::get_property(source, key));
        }

        return std::nullopt;
      }

      template <typename T> std::optional<T> optional(const std::string& key) const
      {
        sptr_rtval value = _get_value(key);
        if (value)
        {
          return RTValueReader<T>::read(value);
        }
        return std::nullopt;
      }

      template <typename T>
      std::optional<T> optional(const std::string& key, std::optional<T> default_value) const
      {
        std::optional<T> value = optional<T>(key);
        if (value)
        {
          return value;
        }
        return default_value;
      }

      friend class MapSchema;
    };
    MapSchema(const KVEntryMap& required_keys, const KVEntryMap& optional_keys = {});

    Inspector bind(Context& ctx, RTValue& map);
  };
} // namespace Lisple

#endif /* LISPLE__HOST__SCHEMA_H */
