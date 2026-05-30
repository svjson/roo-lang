#ifndef ROO__HOST__SCHEMA_H
#define ROO__HOST__SCHEMA_H

#include <optional>
#include <unordered_map>

#include <roo/form.h>
#include <roo/host/object.h>
#include <roo/host/transform.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  template <typename T> struct ValueReader
  {
    static T read(const sptr_val& value) { return Roo::obj<T>(*value); }
  };

  template <> struct ValueReader<sptr_val>
  {
    static sptr_val read(const sptr_val& value) { return value; }
  };

#define ROO_VALUE_READER(TYPE)                                        \
  template <>                                                            \
  struct ValueReader<TYPE>                                               \
  {                                                                      \
    static TYPE read(const sptr_val& value)                              \
    {                                                                    \
      return Roo::rtval_to<TYPE>(value);                              \
    }                                                                    \
  }

  ROO_VALUE_READER(bool);
  ROO_VALUE_READER(char);
  ROO_VALUE_READER(signed char);
  ROO_VALUE_READER(unsigned char);
  ROO_VALUE_READER(int);
  ROO_VALUE_READER(unsigned int);
  ROO_VALUE_READER(unsigned short);
  ROO_VALUE_READER(long);
  ROO_VALUE_READER(float);
  ROO_VALUE_READER(double);
  ROO_VALUE_READER(std::string);

#undef ROO_VALUE_READER

  class MapSchema
  {
    using KVEntryMap = std::unordered_map<std::string, const Roo::TypeRef*>;

    KVEntryMap required;
    KVEntryMap optional;

   public:
    class Inspector
    {
      const MapSchema& schema;
      Value& source;
      std::unordered_map<std::string, sptr_val> override_map;

      Inspector(const MapSchema& schema,
                Value& map,
                std::unordered_map<std::string, sptr_val> overrides);

      sptr_val _get_value(const std::string& key, Value::Type type) const;
      sptr_val _get_value(const std::string& key) const;
      sptr_val _get_value_or_throw(const std::string& key, Value::Type type) const;
      sptr_val _get_value_or_throw(const std::string& key) const;

     public:
      bool contains(const std::string& key) const;

      sptr_val val(const std::string& key) const;
      bool boolean(const std::string& key) const;
      bool boolean(const std::string& key, bool default_value) const;
      uint8_t ui8(const std::string& key) const;
      uint8_t ui8(const std::string& key, uint8_t default_value) const;
      const Value::Number& num(const std::string& key) const;
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
          return Roo::obj<T>(*override_map.at(key));
        }

        return Roo::obj<T>(*Roo::Dict::get_property(source, key));
      }

      template <typename T> T& obj(const std::string& key, T& default_value) const
      {
        if (override_map.count(key))
        {
          return Roo::obj<T>(*override_map.at(key));
        }

        sptr_val obj_val = Roo::Dict::get_property(source, key);
        if (obj_val && (obj_val->type == Value::Type::OBJECT ||
                        obj_val->type == Value::Type::NATIVE_OBJECT))
        {
          return Roo::obj<T>(*Roo::Dict::get_property(source, key));
        }

        return default_value;
      }

      template <typename T>
      const T& obj(const std::string& key, const T& default_value) const
      {
        if (override_map.count(key))
        {
          return Roo::obj<T>(*override_map.at(key));
        }

        sptr_val obj_val = Roo::Dict::get_property(source, key);
        if (obj_val && (obj_val->type == Value::Type::OBJECT ||
                        obj_val->type == Value::Type::NATIVE_OBJECT))
        {
          return Roo::obj<T>(*Roo::Dict::get_property(source, key));
        }

        return default_value;
      }

      template <typename T> std::optional<T> optional_obj(const std::string& key) const
      {
        if (override_map.count(key))
        {
          return Roo::obj<T>(*override_map.at(key));
        }

        sptr_val obj_val = Roo::Dict::get_property(source, key);
        if (obj_val && (obj_val->type == Value::Type::OBJECT ||
                        obj_val->type == Value::Type::NATIVE_OBJECT))
        {
          return Roo::obj<T>(*Roo::Dict::get_property(source, key));
        }

        return std::nullopt;
      }

      template <typename T> std::optional<T> optional(const std::string& key) const
      {
        sptr_val value = _get_value(key);
        if (value)
        {
          return ValueReader<T>::read(value);
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

    Inspector bind(Context& ctx, Value& map);
  };
} // namespace Roo

#endif /* ROO__HOST__SCHEMA_H */
