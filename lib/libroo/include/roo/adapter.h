
#ifndef __ADAPTER_H_
#define __ADAPTER_H_

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <roo/host/object.h>
#include <roo/host/transform.h>
#include <roo/runtime/seq.h>
#include <roo/type.h>

/* __TRAITS_NAME_CONCAT_IMPL
 * __TRAITS_NAME_CONCAT
 * __TRAITS_NAME
 *
 * Helper macros for generating a unique name for a constant. Uses
 * __COUNTER__ (available in gcc, clang and MSVC) to ensure uniqueness.
 */
#define __TRAITS_NAME_CONCAT_IMPL(A, B) A##B
#define __TRAITS_NAME_CONCAT(BASE, COUNT) __TRAITS_NAME_CONCAT_IMPL(BASE, COUNT)
#define __TRAITS_NAME(BASE) __TRAITS_NAME_CONCAT(BASE, __COUNTER__)

/*
 * __DEFINE_VECTOR_TYPE
 * DEFINE_VECTOR_TYPE
 * ROO__DEFINE_VECTOR_TYPE
 *
 * Generates necessary boiler-plate for using a Roo::NativeStdVectorAdapter
 * together with an arbitrary type.
 *
 * This consists of a const Roo::StdVectorTraits containing the type
 * information and template specification of get_vector_traits<T> for
 * accessing it.
 *
 * DEFINE_VECTOR_TYPE should be used when definition happens in the
 * global namespace.
 *
 * ROO__DEFINE_VECTOR_TYPE should be used when definition happens from
 * within the Roo namespace.
 */
#define __DEFINE_VECTOR_TYPE(NS_PREFIX, TRAITS_ID, VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF) \
  inline static const Roo::StdVectorTraits TRAITS_ID                 \
  (                                                                     \
    &VEC_TYPE_REF,                                                      \
    &VALUE_TYPE_REF                                                     \
  );                                                                    \
  template <>                                                           \
  constexpr const Roo::StdVectorTraits* NS_PREFIX get_vector_traits<VALUE_TYPE>() \
  {                                                                     \
    return &TRAITS_ID;                                                  \
  };

#define DEFINE_VECTOR_TYPE(VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)   \
  __DEFINE_VECTOR_TYPE(Roo::, __TRAITS_NAME(StdVector__traits_), VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)

#define ROO__DEFINE_VECTOR_TYPE(VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)   \
  __DEFINE_VECTOR_TYPE(, __TRAITS_NAME(StdVector__traits_), VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)

/*
 * __DEFINE_MAP_TYPE
 * DEFINE_MAP_TYPE
 * ROO__DEFINE_MAP_TYPE
 *
 * Generates necessary boiler-plate for using a Roo::NativeStdMapAdapter
 * together with arbitrary key and value types.
 *
 * This consists of a const Roo::StdMapTraits containing the type
 * information and template specification of get_map_traits<T> for
 * accessing it.
 *
 * DEFINE_MAP_TYPE should be used when definition happens in the
 * global namespace.
 *
 * ROO__DEFINE_MAP_TYPE should be used when definition happens from
 * within the Roo namespace.
 */
#define __DEFINE_MAP_TYPE(NS_PREFIX, TRAITS_ID, MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  inline static const Roo::StdMapTraits TRAITS_ID                     \
  {                                                                      \
    &MAP_TYPE_REF,                                                       \
    Roo::get_roo_type<KEY_TYPE>(),                                 \
    Roo::get_roo_type<VALUE_TYPE>()                                \
  };                                                                     \
  template<>                                                             \
  constexpr const Roo::StdMapTraits* NS_PREFIX get_map_traits<KEY_TYPE, VALUE_TYPE>() \
  {                                                                      \
    return &TRAITS_ID;                                                   \
  }

#define DEFINE_MAP_TYPE(MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  __DEFINE_MAP_TYPE(Roo::, __TRAITS_NAME(StdMap__traits_), MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE)

#define ROO__DEFINE_MAP_TYPE(MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  __DEFINE_MAP_TYPE(, __TRAITS_NAME(StdMap__traits_), MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE)

/*
 * __DEFINE_ROO_TYPE__INTERNAL
 * DEFINE_ROO_TYPE
 * ROO__DEFINE_ROO_TYPE
 *
 * Generates necessary boiler-plate for using native types together with
 * NativeStdMapAdapter and NativeStdVectorAdapter.
 *
 * This consists of a template specification of get_roo_type<T> used
 * lookup the type Roo::TypeRef associated with T.
 *
 * DEFINE_ROO_TYPE should be used when definition happens in the
 * global namespace.
 *
 * ROO__DEFINE_ROO_TYPE should be used when definition happens from
 * within the Roo namespace.
 */
#define __DEFINE_ROO_TYPE__INTERNAL(NS_PREFIX, NATIVE_TYPE, ROO_TYPE) \
  template<>                                                            \
  constexpr const Roo::TypeRef* NS_PREFIX get_roo_type<NATIVE_TYPE>() \
  {                                                                       \
    return &ROO_TYPE;                                                  \
  }

#define DEFINE_ROO_TYPE(NATIVE_TYPE, ROO_TYPE)                      \
  __DEFINE_ROO_TYPE__INTERNAL(Roo::, NATIVE_TYPE, ROO_TYPE)

#define ROO__DEFINE_ROO_TYPE(NATIVE_TYPE, ROO_TYPE)              \
  __DEFINE_ROO_TYPE__INTERNAL(, NATIVE_TYPE, ROO_TYPE)

namespace Roo
{
  class Context;

  namespace Type
  {
    /*! @brief Type reference for std::vector<int> in NativeStdVectorAdapter */
    static const HostTypeRef VECTOR_INT("vector<int>");

    /*! @brief Type reference for std::vector<short> in NativeStdVectorAdapter */
    static const HostTypeRef VECTOR_SHORT("vector<short>");

    /*! @brief Type reference for std::vector<long> in NativeStdVectorAdapter */
    static const HostTypeRef VECTOR_LONG("vector<long>");

    /*! @brief Type reference for std::vector<double> in NativeStdVectorAdapter */
    static const HostTypeRef VECTOR_DOUBLE("vector<double>");

    /*! @brief Type reference for std::vector<float> in NativeStdVectorAdapter */
    static const HostTypeRef VECTOR_FLOAT("vector<float>");

    /*!
     * @brief Type reference for std::vector<unsigned int> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_UINT("vector<unsigned int>");

    /*!
     * @brief Type reference for std::vector<unsigned short> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_USHORT("vector<unsigned short>");

    /*!
     * @brief Type reference for std::vector<unsigned long> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_ULONG("vector<unsigned long>");

    /*!
     * @brief Type reference for std::vector<int8_t> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_INT8("vector<int8_t>");

    /*!
     * @brief Type reference for std::vector<uint8_t> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_UINT8("vector<uint8_t>");

    /*!
     * @brief Type reference for std::vector<std::string> in NativeStdVectorAdapter
     */
    static const HostTypeRef VECTOR_STRING("vector<string>");

    /*!
     * @brief Type reference for std::map<int, std::string> in
     * NativeStdMapAdapter
     */
    static const HostTypeRef MAP_INT_TO_STRING("map<int, string>");

    static const HostTypeRef MAP_INT_TO_CONST_STRING("map<int, const string>");

    /*!
     * @brief Type reference for std::map<uint8_t, short> in
     * NativeStdMapAdapter
     */
    static const HostTypeRef MAP_UINT8_TO_SHORT("map<uint8_t, short>");
  } // namespace Type

  struct StdVectorTraits
  {
    const HostTypeRef* type_ref;
    /*!
     * @brief The Roo TypeRef corresponding to the type T of the wrapped
     * std::vector<T>
     */
    const TypeRef* value_type;

    /*!
     * @brief Initializes a new StdVectorTraits instance with the specialized
     * HostTypeRef and TypeRef for the mapped vector type.
     */
    StdVectorTraits(const HostTypeRef* type_ref, const TypeRef* value_type);
  };

  struct StdMapTraits
  {
    const HostTypeRef* type_ref;
    /*!
     * @brief The Roo TypeRef corresponding to the type K of the wrapped
     * std::map<K, V>
     */
    const TypeRef* key_type;
    /*!
     * @brief The Roo TypeRef corresponding to the type V of the wrapped
     * std::map<K, V>
     */
    const TypeRef* value_type;

    /*!
     * @brief Initializes a new StdMapTraits instance with the specialized
     * HostTypeRef and TypeRef pointers for the mapped K and V map types.
     */
    StdMapTraits(const HostTypeRef* type_ref,
                 const TypeRef* key_type,
                 const TypeRef* value_type);
  };

  struct NativeStdVectorTraits : public NativeObjectTraits
  {
    const TypeRef* value_type;

    NativeStdVectorTraits(const StdVectorTraits* traits)
      : NativeObjectTraits(traits->type_ref, NO_N_ACCESSORS)
      , value_type(traits->value_type)
    {
    }
  };

  struct NativeStdMapTraits : public NativeObjectTraits
  {
    const TypeRef* key_type;
    const TypeRef* value_type;

    NativeStdMapTraits(const StdMapTraits* traits)
      : NativeObjectTraits(traits->type_ref, NO_N_ACCESSORS)
      , key_type(traits->key_type)
      , value_type(traits->value_type)
    {
    }
  };

  template <typename T>
  inline constexpr bool is_rt_primitive_v =
    std::is_arithmetic_v<std::remove_const_t<T>> ||
    std::is_same_v<std::remove_const_t<T>, std::string>;

  template <typename T> std::remove_const_t<T> rtval_to_native(const Value& value)
  {
    if constexpr (is_rt_primitive_v<T>)
    {
      return rtval_to<std::remove_const_t<T>>(std::make_shared<Value>(value));
    }
    else
    {
      return obj<std::remove_const_t<T>>(value);
    }
  }

  template <typename T, class Adapter = T> sptr_val native_to_rtval(const T& value)
  {
    if constexpr (is_rt_primitive_v<T>)
    {
      return rtval_from(static_cast<std::remove_const_t<T>>(value));
    }
    else
    {
      return Adapter::make_ref(value);
    }
  }

  /*!
   * @brief Template method that must be specialized for all types that are
   * to be used with wrapped native collection types.
   *
   * Roo provides implementations for common native types, such as
   * integrals, floating point types and std::string.
   */
  template <typename V> constexpr const TypeRef* get_roo_type();

  /*!
   * get_roo_type<V> template specialization for int
   */
  ROO__DEFINE_ROO_TYPE(int, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for const int
   */
  ROO__DEFINE_ROO_TYPE(const int, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for short
   */
  ROO__DEFINE_ROO_TYPE(short, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for long
   */
  ROO__DEFINE_ROO_TYPE(long, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for unsigned int
   */
  ROO__DEFINE_ROO_TYPE(unsigned int, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for unsigned short
   */
  ROO__DEFINE_ROO_TYPE(unsigned short, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for unsigned long
   */
  ROO__DEFINE_ROO_TYPE(unsigned long, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for double
   */
  ROO__DEFINE_ROO_TYPE(double, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for float
   */
  ROO__DEFINE_ROO_TYPE(float, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for int8_t
   */
  ROO__DEFINE_ROO_TYPE(int8_t, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for uint8_t
   */
  ROO__DEFINE_ROO_TYPE(uint8_t, Type::NUMBER);
  /*!
   * get_roo_type<V> template specialization for std::string
   */
  ROO__DEFINE_ROO_TYPE(std::string, Type::STRING);
  /*!
   * get_roo_type<V> template specialization for const std::string
   */
  ROO__DEFINE_ROO_TYPE(const std::string, Type::STRING);

  /*!
   * @brief Template method that must be specialized for all types that are
   * to be used with NativeStdVectorAdapter.
   *
   * Roo provides implementations for common native types, such as
   * integrals, floating point types and std::string.
   */
  template <typename V> constexpr const StdVectorTraits* get_vector_traits();

  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<int> / Roo::Type::VECTOR_INT.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_INT, int, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<short> / Roo::Type::VECTOR_SHORT.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_SHORT, short, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<long> / Roo::Type::VECTOR_LONG.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_LONG, long, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned_int> / Roo::Type::VECTOR_UINT.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_UINT, unsigned int, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned short> / Roo::Type::VECTOR_USHORT.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_USHORT, unsigned short, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned long> / Roo::Type::VECTOR_ULONG.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_ULONG, unsigned long, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<double> / Roo::Type::VECTOR_DOUBLE.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_DOUBLE, double, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<float> / Roo::Type::VECTOR_FLOAT.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_FLOAT, float, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<int8_t> / Roo::Type::VECTOR_INT8.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_INT8, int8_t, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<uint8_t> / Roo::Type::VECTOR_UINT8.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_UINT8, uint8_t, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<std::string> / Roo::Type::VECTOR_STRING.
   */
  ROO__DEFINE_VECTOR_TYPE(Type::VECTOR_STRING, std::string, Type::STRING);

  /*!
   * @brief Template method that must be specialized for all combinations of
   * K and V types that are to be used with NativeStdMapAdapter.
   */
  template <typename K, typename V> constexpr const StdMapTraits* get_map_traits();

  /*!
   * @brief StdMapTraits template specialization of get_map_type<T> for
   * std::map<int, std::string> / Roo::Type::MAP_INT_TO_STRING
   */
  ROO__DEFINE_MAP_TYPE(Roo::Type::MAP_INT_TO_STRING, int, std::string)

  ROO__DEFINE_MAP_TYPE(Roo::Type::MAP_INT_TO_CONST_STRING, int, const std::string)

  /*!
   * @brief StdMapTraits template specialization of get_map_type<T> for
   * std::map<uint8_t, short> / Roo::Type::MAP_UINT8_TO_SHORT
   */
  ROO__DEFINE_MAP_TYPE(Roo::Type::MAP_UINT8_TO_SHORT, uint8_t, short)

  template <typename V, class A = V>
  class NativeStdVectorAdapter : public NativeObject<std::vector<V>>
  {
   public:
    NativeStdVectorAdapter(std::unique_ptr<std::vector<V>>&& obj_ptr)
      : NativeObject<std::vector<V>>(obj_ptr)
    {
    }

    NativeStdVectorAdapter(std::vector<V>& obj_ref)
      : NativeObject<std::vector<V>>(obj_ref)
    {
    }

    std::vector<V>& get_object() const override { return this->object->get_object(); }
    std::vector<V>& get_self_object() const { return get_object(); }
    void* self_object_ptr() const override { return &get_self_object(); }

    static sptr_val claim(std::unique_ptr<std::vector<V>>&& uptr)
    {
      return Value::native_object(
        std::make_shared<NativeStdVectorAdapter<V, A>>(std::move(uptr)));
    }

    template <typename... Args> static sptr_val make_unique(Args&&... args)
    {
      return Value::native_object(std::make_shared<NativeStdVectorAdapter<V, A>>(
        std::make_unique<std::vector<V>>(std::forward<Args>(args)...)));
    }

    static sptr_val make_ref(const std::vector<V>& ref)
    {
      return Value::native_object(
        std::make_shared<NativeStdVectorAdapter<V, A>>(const_cast<std::vector<V>&>(ref)));
    }

    const NativeObjectTraits* get_traits() const override
    {
      static const NativeStdVectorTraits traits(get_vector_traits<V>());
      return &traits;
    }

    sptr_val get_property(const Value& property) const override
    {
      if (property.type != Value::Type::NUMBER) return Constant::NIL;

      int index = property.num().get_int();
      if (index < 0 || index >= static_cast<int>(get_self_object().size()))
      {
        return Constant::NIL;
      }

      return native_to_rtval<V, A>(get_self_object().at(index));
    }

    bool has_property(const Value& property) const override
    {
      if (property.type != Value::Type::NUMBER) return false;

      int index = property.num().get_int();
      return index >= 0 && index < static_cast<int>(get_self_object().size());
    }

    void set_property(const Value& property, sptr_val& value) override
    {
      if (property.type != Value::Type::NUMBER) return;

      int index = property.num().get_int();
      if (index < 0) return;

      auto& vec = get_self_object();
      while (vec.size() <= static_cast<size_t>(index))
      {
        vec.emplace_back();
      }

      vec[index] = rtval_to_native<V>(*value);
    }

    void set_property(const Value& property, const sptr_val& value) override
    {
      sptr_val v = value;
      set_property(property, v);
    }

    sptr_val_v native_children() const override
    {
      sptr_val_v elements;
      elements.reserve(get_self_object().size());
      for (auto& value : get_self_object())
      {
        elements.push_back(native_to_rtval<V, A>(value));
      }
      return elements;
    }

    sptr_val native_child(size_t index) const override
    {
      auto& values = get_self_object();
      if (index >= values.size()) return Constant::NIL;
      return native_to_rtval<V, A>(values[index]);
    }

    size_t size() const override { return get_self_object().size(); }

    NativeObjectStructuralKind structural_kind() const override
    {
      return NativeObjectStructuralKind::VECTOR;
    }

    bool equals_value(const Value& other) const override
    {
      if (other.type == Value::Type::VECTOR)
      {
        const sptr_val_v& elements = other.elements();
        if (elements.size() != get_self_object().size()) return false;
        for (size_t i = 0; i < elements.size(); i++)
        {
          if (!(*get_property(*Value::number((int)i)) == *elements[i])) return false;
        }
        return true;
      }

      if (other.type == Value::Type::NATIVE_OBJECT)
      {
        sptr_native_obj other_native = other.nobj();
        if (other_native->structural_kind() != NativeObjectStructuralKind::VECTOR)
          return false;

        sptr_val_v elements = native_children();
        sptr_val_v other_elements = other_native->native_children();
        if (elements.size() != other_elements.size()) return false;
        for (size_t i = 0; i < elements.size(); i++)
        {
          if (!(*elements[i] == *other_elements[i])) return false;
        }
        return true;
      }

      return false;
    }

    std::string to_string() const override
    {
      return Value::vector(native_children())->to_string();
    }
  };

  template <typename K, typename V, class A1 = K, class A2 = V>
  class NativeStdMapAdapter : public NativeObject<std::map<K, V>>
  {
    using ValueAdapter = std::conditional_t<is_rt_primitive_v<K>, A1, A2>;

   public:
    NativeStdMapAdapter(std::unique_ptr<std::map<K, V>>&& obj_ptr)
      : NativeObject<std::map<K, V>>(obj_ptr)
    {
    }

    NativeStdMapAdapter(std::map<K, V>& obj_ref)
      : NativeObject<std::map<K, V>>(obj_ref)
    {
    }

    std::map<K, V>& get_object() const override { return this->object->get_object(); }
    std::map<K, V>& get_self_object() const { return get_object(); }
    void* self_object_ptr() const override { return &get_self_object(); }

    static sptr_val claim(std::unique_ptr<std::map<K, V>>&& uptr)
    {
      return Value::native_object(
        std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(std::move(uptr)));
    }

    template <typename... Args> static sptr_val make_unique(Args&&... args)
    {
      return Value::native_object(std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(
        std::make_unique<std::map<K, V>>(std::forward<Args>(args)...)));
    }

    static sptr_val make_ref(const std::map<K, V>& ref)
    {
      return Value::native_object(std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(
        const_cast<std::map<K, V>&>(ref)));
    }

    const NativeObjectTraits* get_traits() const override
    {
      static const NativeStdMapTraits traits(get_map_traits<K, V>());
      return &traits;
    }

    const NativeStdMapTraits* get_map_native_traits() const
    {
      return static_cast<const NativeStdMapTraits*>(get_traits());
    }

    bool has_key(const Value& property) const
    {
      const NativeStdMapTraits* traits = get_map_native_traits();
      if (*Constant::NIL == property || !traits->key_type->is_type_of(property))
      {
        return false;
      }

      return get_self_object().count(rtval_to_native<K>(property));
    }

    sptr_val get_property(const Value& property) const override
    {
      if (!has_key(property))
      {
        return Constant::NIL;
      }

      return native_to_rtval<V, ValueAdapter>(
        get_self_object().at(rtval_to_native<K>(property)));
    }

    bool has_property(const Value& property) const override { return has_key(property); }

    void set_property(const Value& property, sptr_val& value) override
    {
      const NativeStdMapTraits* traits = get_map_native_traits();
      if (*value == *Constant::NIL || *Constant::NIL == property ||
          !traits->key_type->is_type_of(property) || !traits->value_type->is_type_of(*value))
      {
        return;
      }

      auto key = rtval_to_native<K>(property);
      auto map_value = rtval_to_native<V>(*value);

      if constexpr (std::is_const_v<V>)
      {
        get_self_object().erase(key);
        get_self_object().emplace(key, map_value);
      }
      else
      {
        get_self_object().insert_or_assign(key, map_value);
      }
    }

    void set_property(const Value& property, const sptr_val& value) override
    {
      sptr_val v = value;
      set_property(property, v);
    }

    sptr_val_v native_children() const override
    {
      sptr_val_v elements;
      elements.reserve(get_self_object().size() * 2);
      for (auto& [key, value] : get_self_object())
      {
        elements.push_back(native_to_rtval<K, A1>(key));
        elements.push_back(native_to_rtval<V, ValueAdapter>(value));
      }
      return elements;
    }

    size_t size() const override { return get_self_object().size(); }

    std::string to_string() const override
    {
      return Value::map(native_children())->to_string();
    }
  };

} // namespace Roo

#endif
