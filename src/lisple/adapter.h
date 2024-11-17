
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "form.h"
#include "host.h"
#include "impl.h"
#include "type.h"

namespace Lisple
{
  class Context;

  template <typename K, typename V, class A1=K, class A2=V>
  class StdMapAdapter : public HostObject<std::map<K, V>>
  {
    const TypeRef* key_type;
    const TypeRef* value_type;

   public:
    StdMapAdapter(const HostTypeRef* type,
                  std::map<K, V>& map,
                  const TypeRef* key_type,
                  const TypeRef* value_type)
      : HostObject<std::map<K, V>>(type, map)
      , key_type(key_type)
      , value_type(value_type)
    {
    }

    void set_property(const Object& key, sptr_sobject& value) override
    {
      this->set_property(nullptr, key, value);
    }

    void set_property(Context*, const Lisple::Object& key, sptr_sobject& value) override
    {
      if (*value != *NIL &&
          key != *NIL &&
          key_type->is_type_of(key) &&
          value_type->is_type_of(*value))
      {
        if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
          {
            this->get_object().insert_or_assign(unwrap_primitive<K>(key),
                                                unwrap_primitive<V>(*value));
          }
          else if constexpr (std::is_arithmetic<V>::value || std::is_same<std::remove_const_t<V>, std::string>::value)
          {
            this->get_object().emplace(unwrap_primitive<K>(key),
                                       unwrap_primitive<std::remove_const_t<V>>(*value));
          }
          else
          {
            if (this->has_key(key))
            {
              this->get_object().erase(unwrap_primitive<K>(key));
            }
            this->get_object().emplace(unwrap_primitive<K>(key),
                                       value->as<HostObject<std::remove_const_t<V>>>().get_object());
          }
        }
        else
        {
          if constexpr ((!std::is_const<V>::value && std::is_arithmetic<V>::value) ||
                        std::is_same<V, std::string>::value)
          {
            this->get_object().insert_or_assign(key.as<HostObject<std::remove_const_t<K>>>().get_object(),
                                                unwrap_primitive<V>(*value));
          }
          else if constexpr (std::is_arithmetic<V>::value || std::is_same<std::remove_const_t<V>, std::string>::value)
          {
            this->get_object().emplace(key.as<HostObject<std::remove_const_t<K>>>().get_object(),
                                       unwrap_primitive<std::remove_const_t<V>>(*value));
          }
          else
          {
            if (this->has_key(key))
            {
              this->get_object().erase(key.as<HostObject<K>>().get_object());
            }
            this->get_object().emplace(key.as<HostObject<K>>().get_object(),
                                       value->as<HostObject<std::remove_const_t<V>>>().get_object());
          }
        }
      }
    }

    sptr_sobject get_sptr_property(const Lisple::Object& key) const override
    {
      if (this->has_key(key))
      {
        if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<std::remove_const_t<V>, std::string>::value)
          {
            return wrap_primitive(this->get_object().at(unwrap_primitive<K>(key)));
          }
          else
          {
            return A1::make_ref(this->get_object().at(unwrap_primitive<K>(key)));
          }
        }
        else
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
          {
            return wrap_primitive(this->get_object().at(key.as<HostObject<K>>().get_object()));
          }
          else
          {
            return A2::make_ref(this->get_object().at(key.as<HostObject<K>>().get_object()));
          }
        }
      }
      return Lisple::NIL;
    }

    bool has_key(const Lisple::Object& key) const override
    {
      if (key != *Lisple::NIL &&
          key_type->is_type_of(key))
      {
        if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
        {
          return this->get_object().count(unwrap_primitive<K>(key));
        }
        else
        {
          return this->get_object().count(key.as<HostObject<K>>().get_object());
        }
      }
      return false;
    }

    const Lisple::sptr_sobject_v keys() const override
    {
      Lisple::sptr_sobject_v keys;
      keys.reserve(this->get_object().size());

      if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
      {
        for (auto& [k, v] : this->get_object()) keys.push_back(wrap_primitive(k));
      }
      else
      {
        for (auto& [k, v] : this->get_object()) keys.push_back(A1::make_ref(k));
      }

      return keys;
    }

    unsigned int size() const override
    {
      return this->get_object().size();
    }
  };
}
