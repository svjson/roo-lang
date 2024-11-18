
#include <cstdint>
#include <map>
#include <string>
#include <type_traits>

#include "form.h"
#include "host.h"
#include "impl.h"
#include "type.h"

namespace Lisple
{
  class Context;

  namespace Type
  {
    static const HostTypeRef VECTOR_INT("vector<int>");
    static const HostTypeRef VECTOR_SHORT("vector<short>");
    static const HostTypeRef VECTOR_LONG("vector<long>");
    static const HostTypeRef VECTOR_DOUBLE("vector<double>");
    static const HostTypeRef VECTOR_FLOAT("vector<float>");
    static const HostTypeRef VECTOR_UINT("vector<unsigned int>");
    static const HostTypeRef VECTOR_USHORT("vector<unsigned short>");
    static const HostTypeRef VECTOR_ULONG("vector<unsigned long>");
    static const HostTypeRef VECTOR_INT8("vector<int8_t>");
    static const HostTypeRef VECTOR_UINT8("vector<uint8_t>");
    static const HostTypeRef VECTOR_STRING("vector<string>");
  }

  template <typename V>
  constexpr const HostTypeRef* get_vector_host_type();

  template <>
  constexpr const HostTypeRef* get_vector_host_type<int>()
  {
    return &Type::VECTOR_INT;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<short>()
  {
    return &Type::VECTOR_SHORT;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<long>()
  {
    return &Type::VECTOR_LONG;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<unsigned int>()
  {
    return &Type::VECTOR_UINT;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<unsigned short>()
  {
    return &Type::VECTOR_USHORT;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<unsigned long>()
  {
    return &Type::VECTOR_ULONG;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<double>()
  {
    return &Type::VECTOR_DOUBLE;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<float>()
  {
    return &Type::VECTOR_FLOAT;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<int8_t>()
  {
    return &Type::VECTOR_INT8;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<uint8_t>()
  {
    return &Type::VECTOR_UINT8;
  }
  template <>
  constexpr const HostTypeRef* get_vector_host_type<std::string>()
  {
    return &Type::VECTOR_STRING;
  }

  template <typename V>
  constexpr const TypeRef* get_lisple_type();

  template <>
  constexpr const TypeRef* get_lisple_type<int>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<short>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<long>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<unsigned int>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<unsigned short>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<unsigned long>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<double>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<float>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<int8_t>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<uint8_t>()
  {
    return &Type::NUMBER;
  }

  template <>
  constexpr const TypeRef* get_lisple_type<std::string>()
  {
    return &Type::STRING;
  }

  /*!
   * @brief Holds and std::vector and allows it to be exposed to the Lisple
   * runtime and to be treated much like a Lisple::Array, although some
   * differences do apply.
   *
   * In order for Lisple functions to access the contents of the underlying
   * vector, they must still be transformed on the fly behind and behind the
   * scenes. This means that there is a performance cost to be considered.
   */
  template <typename V, class A=V>
  class StdVectorAdapter : public HostObject<std::vector<V>>
  {
    /*!
     * @brief Lisple Type reference of the vector value type
     */
    const TypeRef* value_type;

   public:
    /*!
     * @brief Verbose constructor that allows specialization on construction
     * "on the fly" and without pre-definition, but requires specifying the
     * host type of the vector adapter self as well as the value
     */
    StdVectorAdapter(const HostTypeRef* type,
                     std::vector<V>& collection,
                     const TypeRef* value_type)
      : HostObject<std::vector<V>>(Form::HOST_SEQ, type, collection)
      , value_type(value_type)
    {
    }

    /*!
     * @brief Constructor for pre-compiled vector types that will determine
     * its type references automatically. Requires pre-defined specializations
     * of @ref get_vector_host_type and @ref get_lisple_type.
     */
    StdVectorAdapter(std::vector<V>& collection)
      : HostObject<std::vector<V>>(Form::HOST_SEQ, get_vector_host_type<V>(), collection)
      , value_type(get_lisple_type<V>())
    {
    }
    sptr_sobject& head() override
    {
      this->sync_children();
      return Seq::head();
    }

    sptr_sobject_v tail() override
    {
      this->sync_children();
      return Seq::tail();
    }

    void append(const sptr_sobject& child) override
    {
      if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
      {
        this->get_object().push_back(unwrap_primitive<V>(*child));
      }
      else
      {
        this->get_object().push_back(child->as<A>().get_object());
      }
    }

    sptr_sobject_v& get_children() override
    {
      this->sync_children();
      return this->children;;
    }

    void replace_children(const sptr_sobject_v& children) override
    {
      std::vector<V>& vec = this->get_object();
      vec.clear();
      vec.reserve(children.size());
      for (auto& c : children)
      {
        if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
        {
          vec.push_back(unwrap_primitive<V>(*c));
        }
        else
        {
          vec.push_back(c->as<A>().get_object());
        }
      }
    }

    unsigned int size() const override
    {
      return this->get_object().size();
    }

    std::string to_string(int depth=-1) const override
    {
      this->sync_children();
      return Seq::to_string(depth);
    }

    const std::string lpar() const override
    {
      return "[";
    }

    const std::string rpar() const override
    {
      return "]";
    }

   protected:
    void sync_children() const override
    {
      this->children.clear();
      this->children.reserve(this->get_object().size());
      for (auto& v : this->get_object())
      {
        if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
        {
          this->children.push_back(wrap_primitive<V>(v));
        }
        else
        {
          this->children.push_back(A::make_ref(v));
        }

      }
    }
  };

  template class StdVectorAdapter<int>;
  template class StdVectorAdapter<short>;
  template class StdVectorAdapter<long>;
  template class StdVectorAdapter<float>;
  template class StdVectorAdapter<double>;
  template class StdVectorAdapter<int8_t>;
  template class StdVectorAdapter<unsigned int>;
  template class StdVectorAdapter<unsigned short>;
  template class StdVectorAdapter<unsigned long>;
  template class StdVectorAdapter<uint8_t>;
  template class StdVectorAdapter<std::string>;

  typedef StdVectorAdapter<int> VectorInt;

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
            return wrap_primitive<std::remove_const_t<V>>(this->get_object().at(unwrap_primitive<K>(key)));
          }
          else
          {
            return A1::make_ref(this->get_object().at(unwrap_primitive<std::remove_const_t<K>>(key)));
          }
        }
        else
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
          {
            return wrap_primitive<std::remove_const_t<V>>(this->get_object().at(key.as<HostObject<K>>().get_object()));
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
        for (auto& [k, v] : this->get_object()) keys.push_back(wrap_primitive<K>(k));
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
