
#ifndef __ADAPTER_H_
#define __ADAPTER_H_

#include <cstdint>
#include <map>
#include <string>
#include <type_traits>

#include <lisple/form.h>
#include <lisple/host.h>
#include <lisple/host/object.h>
#include <lisple/host/transform.h>
#include <lisple/impl.h>
#include <lisple/runtime/seq.h>
#include <lisple/type.h>

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
 * LISPLE__DEFINE_VECTOR_TYPE
 *
 * Generates necessary boiler-plate for using a Lisple::StdVectorAdapter
 * together with an arbitrary type.
 *
 * This consists of a const Lisple::StdVectorTraits containing the type
 * information and template specification of get_vector_traits<T> for
 * accessing it.
 *
 * DEFINE_VECTOR_TYPE should be used when definition happens in the
 * global namespace.
 *
 * LISPLE__DEFINE_VECTOR_TYPE should be used when definition happens from
 * within the Lisple namespace.
 */
#define __DEFINE_VECTOR_TYPE(NS_PREFIX, TRAITS_ID, VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF) \
  inline static const Lisple::StdVectorTraits TRAITS_ID                 \
  (                                                                     \
    &VEC_TYPE_REF,                                                      \
    &VALUE_TYPE_REF                                                     \
  );                                                                    \
  template <>                                                           \
  constexpr const Lisple::StdVectorTraits* NS_PREFIX get_vector_traits<VALUE_TYPE>() \
  {                                                                     \
    return &TRAITS_ID;                                                  \
  };

#define DEFINE_VECTOR_TYPE(VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)   \
  __DEFINE_VECTOR_TYPE(Lisple::, __TRAITS_NAME(StdVector__traits_), VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)

#define LISPLE__DEFINE_VECTOR_TYPE(VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)   \
  __DEFINE_VECTOR_TYPE(, __TRAITS_NAME(StdVector__traits_), VEC_TYPE_REF, VALUE_TYPE, VALUE_TYPE_REF)

/*
 * __DEFINE_MAP_TYPE
 * DEFINE_MAP_TYPE
 * LISPLE__DEFINE_MAP_TYPE
 *
 * Generates necessary boiler-plate for using a Lisple::StdMapAdapter
 * together with arbitrary key and value types.
 *
 * This consists of a const Lisple::StdMapTraits containing the type
 * information and template specification of get_map_traits<T> for
 * accessing it.
 *
 * DEFINE_MAP_TYPE should be used when definition happens in the
 * global namespace.
 *
 * LISPLE__DEFINE_MAP_TYPE should be used when definition happens from
 * within the Lisple namespace.
 */
#define __DEFINE_MAP_TYPE(NS_PREFIX, TRAITS_ID, MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  inline static const Lisple::StdMapTraits TRAITS_ID                     \
  {                                                                      \
    &MAP_TYPE_REF,                                                       \
    Lisple::get_lisple_type<KEY_TYPE>(),                                 \
    Lisple::get_lisple_type<VALUE_TYPE>()                                \
  };                                                                     \
  template<>                                                             \
  constexpr const Lisple::StdMapTraits* NS_PREFIX get_map_traits<KEY_TYPE, VALUE_TYPE>() \
  {                                                                      \
    return &TRAITS_ID;                                                   \
  }

#define DEFINE_MAP_TYPE(MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  __DEFINE_MAP_TYPE(Lisple::, __TRAITS_NAME(StdMap__traits_), MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE)

#define LISPLE__DEFINE_MAP_TYPE(MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE) \
  __DEFINE_MAP_TYPE(, __TRAITS_NAME(StdMap__traits_), MAP_TYPE_REF, KEY_TYPE, VALUE_TYPE)

/*
 * __DEFINE_LISPLE_TYPE__INTERNAL
 * DEFINE_LISPLE_TYPE
 * LISPLE__DEFINE_LISPLE_TYPE
 *
 * Generates necessary boiler-plate for using native types together with
 * StdMapAdapter and StdVectorAdapter.
 *
 * This consists of a template specification of get_lisple_type<T> used
 * lookup the type Lisple::TypeRef associated with T.
 *
 * DEFINE_LISPLE_TYPE should be used when definition happens in the
 * global namespace.
 *
 * LISPLE__DEFINE_LISPLE_TYPE should be used when definition happens from
 * within the Lisple namespace.
 */
#define __DEFINE_LISPLE_TYPE__INTERNAL(NS_PREFIX, NATIVE_TYPE, LISPLE_TYPE) \
  template<>                                                            \
  constexpr const Lisple::TypeRef* NS_PREFIX get_lisple_type<NATIVE_TYPE>() \
  {                                                                       \
    return &LISPLE_TYPE;                                                  \
  }

#define DEFINE_LISPLE_TYPE(NATIVE_TYPE, LISPLE_TYPE)                      \
  __DEFINE_LISPLE_TYPE__INTERNAL(Lisple::, NATIVE_TYPE, LISPLE_TYPE)

#define LISPLE__DEFINE_LISPLE_TYPE(NATIVE_TYPE, LISPLE_TYPE)              \
  __DEFINE_LISPLE_TYPE__INTERNAL(, NATIVE_TYPE, LISPLE_TYPE)

namespace Lisple
{
  class Context;

  namespace Type
  {
    /*! @brief HostTypeRef for wrapping std::vector<int> in StdVectorAdapter */
    static const HostTypeRef VECTOR_INT("vector<int>");

    /*! @brief HostTypeRef for wrapping std::vector<short> in StdVectorAdapter */
    static const HostTypeRef VECTOR_SHORT("vector<short>");

    /*! @brief HostTypeRef for wrapping std::vector<long> in StdVectorAdapter */
    static const HostTypeRef VECTOR_LONG("vector<long>");

    /*! @brief HostTypeRef for wrapping std::vector<double> in StdVectorAdapter */
    static const HostTypeRef VECTOR_DOUBLE("vector<double>");

    /*! @brief HostTypeRef for wrapping std::vector<float> in StdVectorAdapter */
    static const HostTypeRef VECTOR_FLOAT("vector<float>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<unsigned int> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_UINT("vector<unsigned int>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<unsigned short> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_USHORT("vector<unsigned short>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<unsigned long> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_ULONG("vector<unsigned long>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<int8_t> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_INT8("vector<int8_t>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<uint8_t> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_UINT8("vector<uint8_t>");

    /*!
     * @brief HostTypeRef for wrapping std::vector<std::string> in
     * StdVectorAdapter
     */
    static const HostTypeRef VECTOR_STRING("vector<string>");

    /*!
     * @brief HostTypeRef for wrapping std::map<int, std::string> in
     * StdMapAdapter
     */
    static const HostTypeRef MAP_INT_TO_STRING("map<int, string>");

    /*!
     * @brief HostTypeRef for wrapping std::map<uint8_t, short> in
     * StdMapAdapter
     */
    static const HostTypeRef MAP_UINT8_TO_SHORT("map<uint8_t, short>");
  } // namespace Type

  /*!
   * @brief AdapterTraits-implementation for use with StdVectorAdapter
   */
  struct StdVectorTraits : public AdapterTraits
  {
    /*!
     * @brief The Lisple TypeRef corresponding to the type T of the wrapped
     * std::vector<T>
     */
    const TypeRef* value_type;

    /*!
     * @brief Initializes a new StdVectorTraits instance with the specialized
     * HostTypeRef and TypeRef for the mapped vector type.
     */
    StdVectorTraits(const HostTypeRef* type_ref, const TypeRef* value_type);
  };

  /*!
   * @brief AdapterTraits-implementation for use with StdMapAdapter
   */
  struct StdMapTraits : public AdapterTraits
  {
    /*!
     * @brief The Lisple TypeRef corresponding to the type K of the wrapped
     * std::map<K, V>
     */
    const TypeRef* key_type;
    /*!
     * @brief The Lisple TypeRef corresponding to the type V of the wrapped
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

  template <typename T> std::remove_const_t<T> rtval_to_native(const RTValue& value)
  {
    if constexpr (is_rt_primitive_v<T>)
    {
      return rtval_to<std::remove_const_t<T>>(std::make_shared<RTValue>(value));
    }
    else
    {
      return obj<std::remove_const_t<T>>(value);
    }
  }

  template <typename T, class Adapter = T> sptr_rtval native_to_rtval(const T& value)
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
   * Lisple provides implementations for common native types, such as
   * integrals, floating point types and std::string.
   */
  template <typename V> constexpr const TypeRef* get_lisple_type();

  /*!
   * get_lisple_type<V> template specialization for int
   */
  LISPLE__DEFINE_LISPLE_TYPE(int, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for const int
   */
  LISPLE__DEFINE_LISPLE_TYPE(const int, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for short
   */
  LISPLE__DEFINE_LISPLE_TYPE(short, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for long
   */
  LISPLE__DEFINE_LISPLE_TYPE(long, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for unsigned int
   */
  LISPLE__DEFINE_LISPLE_TYPE(unsigned int, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for unsigned short
   */
  LISPLE__DEFINE_LISPLE_TYPE(unsigned short, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for unsigned long
   */
  LISPLE__DEFINE_LISPLE_TYPE(unsigned long, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for double
   */
  LISPLE__DEFINE_LISPLE_TYPE(double, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for float
   */
  LISPLE__DEFINE_LISPLE_TYPE(float, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for int8_t
   */
  LISPLE__DEFINE_LISPLE_TYPE(int8_t, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for uint8_t
   */
  LISPLE__DEFINE_LISPLE_TYPE(uint8_t, Type::NUMBER);
  /*!
   * get_lisple_type<V> template specialization for std::string
   */
  LISPLE__DEFINE_LISPLE_TYPE(std::string, Type::STRING);
  /*!
   * get_lisple_type<V> template specialization for const std::string
   */
  LISPLE__DEFINE_LISPLE_TYPE(const std::string, Type::STRING);

  /*!
   * @brief Template method that must be specialized for all types that are
   * to be used with StdVectorAdapter.
   *
   * Lisple provides implementations for common native types, such as
   * integrals, floating point types and std::string.
   */
  template <typename V> constexpr const StdVectorTraits* get_vector_traits();

  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<int> / Lisple::Type::VECTOR_INT.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_INT, int, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<short> / Lisple::Type::VECTOR_SHORT.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_SHORT, short, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<long> / Lisple::Type::VECTOR_LONG.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_LONG, long, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned_int> / Lisple::Type::VECTOR_UINT.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_UINT, unsigned int, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned short> / Lisple::Type::VECTOR_USHORT.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_USHORT, unsigned short, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<unsigned long> / Lisple::Type::VECTOR_ULONG.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_ULONG, unsigned long, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<double> / Lisple::Type::VECTOR_DOUBLE.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_DOUBLE, double, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<float> / Lisple::Type::VECTOR_FLOAT.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_FLOAT, float, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<int8_t> / Lisple::Type::VECTOR_INT8.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_INT8, int8_t, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<uint8_t> / Lisple::Type::VECTOR_UINT8.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_UINT8, uint8_t, Type::NUMBER);
  /*!
   * @brief StdVectorTraits template specialization of get_vector_type<T> for
   * std::vector<std::string> / Lisple::Type::VECTOR_STRING.
   */
  LISPLE__DEFINE_VECTOR_TYPE(Type::VECTOR_STRING, std::string, Type::STRING);

  /*!
   * @brief Template method that must be specialized for all combinations of
   * K and V types that are to be used with StdMapAdapter.
   */
  template <typename K, typename V> constexpr const StdMapTraits* get_map_traits();

  /*!
   * @brief StdMapTraits template specialization of get_map_type<T> for
   * std::map<int, std::string> / Lisple::Type::MAP_INT_TO_STRING
   */
  LISPLE__DEFINE_MAP_TYPE(Lisple::Type::MAP_INT_TO_STRING, int, std::string)

  /*!
   * @brief StdMapTraits template specialization of get_map_type<T> for
   * std::map<uint8_t, short> / Lisple::Type::MAP_UINT8_TO_SHORT
   */
  LISPLE__DEFINE_MAP_TYPE(Lisple::Type::MAP_UINT8_TO_SHORT, uint8_t, short)

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

    static sptr_rtval claim(std::unique_ptr<std::vector<V>>&& uptr)
    {
      return RTValue::native_object(
        std::make_shared<NativeStdVectorAdapter<V, A>>(std::move(uptr)));
    }

    template <typename... Args> static sptr_rtval make_unique(Args&&... args)
    {
      return RTValue::native_object(std::make_shared<NativeStdVectorAdapter<V, A>>(
        std::make_unique<std::vector<V>>(std::forward<Args>(args)...)));
    }

    static sptr_rtval make_ref(const std::vector<V>& ref)
    {
      return RTValue::native_object(
        std::make_shared<NativeStdVectorAdapter<V, A>>(const_cast<std::vector<V>&>(ref)));
    }

    const NativeObjectTraits* get_traits() const override
    {
      static const NativeStdVectorTraits traits(get_vector_traits<V>());
      return &traits;
    }

    sptr_rtval get_property(const RTValue& property) const override
    {
      if (property.type != RTValue::Type::NUMBER) return Constant::NIL;

      int index = property.num().get_int();
      if (index < 0 || index >= static_cast<int>(get_self_object().size()))
      {
        return Constant::NIL;
      }

      return native_to_rtval<V, A>(get_self_object().at(index));
    }

    void set_property(const RTValue& property, sptr_rtval& value) override
    {
      if (property.type != RTValue::Type::NUMBER) return;

      int index = property.num().get_int();
      if (index < 0) return;

      auto& vec = get_self_object();
      while (vec.size() <= static_cast<size_t>(index))
      {
        vec.emplace_back();
      }

      vec[index] = rtval_to_native<V>(*value);
    }

    void set_property(const RTValue& property, const sptr_rtval& value) override
    {
      sptr_rtval v = value;
      set_property(property, v);
    }

    sptr_rtval_v native_children() const override
    {
      sptr_rtval_v elements;
      elements.reserve(get_self_object().size());
      for (auto& value : get_self_object())
      {
        elements.push_back(native_to_rtval<V, A>(value));
      }
      return elements;
    }

    size_t size() const override { return get_self_object().size(); }

    std::string to_string() const override
    {
      return RTValue::vector(native_children())->to_string();
    }
  };

  /*!
   * @brief Holds an std::vector and allows it to be exposed to the Lisple
   * runtime and to be treated much like a Lisple::Array, although some
   * differences do apply.
   *
   * In order for Lisple functions to access the contents of the underlying
   * vector, they must still be transformed on the fly behind the scenes.
   * This means that there is a performance cost to be considered.
   */
  template <typename V, class A = V>
  class StdVectorAdapter : public HostObject<std::vector<V>>
  {
    /*!
     * @brief Lisple Type reference of the vector value type
     */
    const TypeRef* value_type;

   public:
    /*!
     * @brief Constructor for pre-compiled vector types that will determine
     * its type references automatically. Requires pre-defined specializations
     * of @ref get_vector_traits and @ref get_lisple_type.
     */
    StdVectorAdapter(std::vector<V>& collection)
      : HostObject<std::vector<V>>(Form::HOST_SEQ, collection)
      , value_type(get_lisple_type<V>())
    {
    }

    /*!
     * @brief Specialized type-specific traits describing the vector and its
     * value type.
     */
    inline static const StdVectorTraits* _traits = get_vector_traits<V>();

    /*!
     * @see Lisple::AbstractHostObject::get_traits
     */
    const AdapterTraits* get_traits() const override { return _traits; }

    /*!
     * @brief Static access to the type traits
     */
    static const AdapterTraits* traits() { return _traits; }

    /*!
     * @see Lisple::Seq::head
     *
     * @brief Retrieves the first element of the Sequence after syncing the
     * vector contents with the cached Lisple view of the underlying std::vector.
     */
    sptr_sobject& head() override
    {
      this->sync_children();
      return Seq::head();
    }

    /*!
     * @see Lisple::Seq::tail
     *
     * @brief Retrieves the first element of the Sequence after syncing the
     * vector contents with the cached Lisple view of the underlying std::vector.
     */
    sptr_sobject_v tail() override
    {
      this->sync_children();
      return Seq::tail();
    }

    /*!
     * @see Lisple::Seq::append
     *
     * @brief Appends a value to the underlying std::vector<V>. The value must be
     * compatible with V.
     */
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

    /*!
     * @see Lisple::Object::get_children
     *
     * @brief Returns a reference to the internally cached vector of children, after
     * syncing it.
     */
    sptr_sobject_v& get_children() override
    {
      this->sync_children();
      return this->children;
      ;
    }

    /*!
     * @see Lisple::Seq::replace_children
     *
     * @brief Replaces the entire contents of the underlying vector with a new set of
     * elements. All new children must be compatible with the value type V.
     */
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

    /*!
     * @see Lisple::Seq::size
     *
     * @brief Does not require synchronization and does not affect the
     * internally cached Lisple vector.
     */
    unsigned int size() const override { return this->get_object().size(); }

    /*!
     * @see Lisple::Object::to_string
     *
     * Requires synchronization of the internally cached Lisple vector, and is
     * therefore a potentially costly operation.
     */
    std::string to_string(int depth = -1) const override
    {
      this->sync_children();
      return Seq::to_string(depth);
    }

    /*! @see Lisple::Seq::lpar */
    const std::string lpar() const override { return "["; }

    /*! @see Lisple::Seq::rpar */
    const std::string rpar() const override { return "]"; }

   protected:
    /*!
     * @brief Syncronizes the internally cached view of the underlying vector as
     * Lisple Objects with a naive implementation that simply reinitializes the
     * the vector from top to bottom.
     */
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

  /*! @brief Pre-defined specialization of StdVectorAdapter<int> */
  template class StdVectorAdapter<int>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<short> */
  template class StdVectorAdapter<short>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<long> */
  template class StdVectorAdapter<long>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<unsigned int> */
  template class StdVectorAdapter<unsigned int>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<unsigned short> */
  template class StdVectorAdapter<unsigned short>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<unsigned long> */
  template class StdVectorAdapter<unsigned long>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<float> */
  template class StdVectorAdapter<float>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<double> */
  template class StdVectorAdapter<double>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<int8_t> */
  template class StdVectorAdapter<int8_t>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<uint8_t> */
  template class StdVectorAdapter<uint8_t>;
  /*! @brief Pre-defined specialization of StdVectorAdapter<std::string> */
  template class StdVectorAdapter<std::string>;

  typedef StdVectorAdapter<int> VectorInt;

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

    static sptr_rtval claim(std::unique_ptr<std::map<K, V>>&& uptr)
    {
      return RTValue::native_object(
        std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(std::move(uptr)));
    }

    template <typename... Args> static sptr_rtval make_unique(Args&&... args)
    {
      return RTValue::native_object(std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(
        std::make_unique<std::map<K, V>>(std::forward<Args>(args)...)));
    }

    static sptr_rtval make_ref(const std::map<K, V>& ref)
    {
      return RTValue::native_object(std::make_shared<NativeStdMapAdapter<K, V, A1, A2>>(
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

    bool has_key(const RTValue& property) const
    {
      const NativeStdMapTraits* traits = get_map_native_traits();
      if (*Constant::NIL == property || !traits->key_type->is_type_of(property))
      {
        return false;
      }

      return get_self_object().count(rtval_to_native<K>(property));
    }

    sptr_rtval get_property(const RTValue& property) const override
    {
      if (!has_key(property))
      {
        return Constant::NIL;
      }

      return native_to_rtval<V, ValueAdapter>(
        get_self_object().at(rtval_to_native<K>(property)));
    }

    void set_property(const RTValue& property, sptr_rtval& value) override
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

    void set_property(const RTValue& property, const sptr_rtval& value) override
    {
      sptr_rtval v = value;
      set_property(property, v);
    }

    sptr_rtval_v native_children() const override
    {
      sptr_rtval_v elements;
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
      return RTValue::map(native_children())->to_string();
    }
  };

  /*!
   * @brief Holds an std::map and allows it to be exposed to the Lisple
   * runtime and to be treated much like a Lisple::Map, although some
   * differences do apply.
   *
   * In order for Lisple functions to access the contents of the underlying
   * map, they must still be transformed on the fly behind the scenes.
   * This means that there is a performance cost to be considered.
   */
  template <typename K, typename V, class A1 = K, class A2 = V>
  class StdMapAdapter : public HostObject<std::map<K, V>>
  {
   public:
    StdMapAdapter(std::map<K, V>& map)
      : HostObject<std::map<K, V>>(map)
    {
    }

    /*!
     * @brief Specialized type-specific traits describing the map and its
     * key and value types.
     */
    inline static const StdMapTraits* _traits = get_map_traits<K, V>();

    /*!
     * @see Lisple::AbstractHostObject::get_traits
     */
    const AdapterTraits* get_traits() const override { return _traits; }

    std::map<K, V> get_self_object() { return this->get_object(); }

    /*!
     * @brief Add or overwrite a key/value pair of the underlying map.
     */
    void set_property(const Object& key, sptr_sobject& value) override
    {
      this->set_property(nullptr, key, value);
    }

    /*!
     * @brief Add or overwrite a key/value pair of the underlying map.
     */
    void set_property(Context*, const Lisple::Object& key, sptr_sobject& value) override
    {
      if (*value != *NIL && key != *NIL && _traits->key_type->is_type_of(key) &&
          _traits->value_type->is_type_of(*value))
      {
        if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
          {
            this->get_object().insert_or_assign(unwrap_primitive<K>(key),
                                                unwrap_primitive<V>(*value));
          }
          else if constexpr (std::is_arithmetic<V>::value ||
                             std::is_same<std::remove_const_t<V>, std::string>::value)
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
            this->get_object().emplace(
              unwrap_primitive<K>(key),
              value->as<HostObject<std::remove_const_t<V>>>().get_object());
          }
        }
        else
        {
          if constexpr ((!std::is_const<V>::value && std::is_arithmetic<V>::value) ||
                        std::is_same<V, std::string>::value)
          {
            this->get_object().insert_or_assign(
              key.as<HostObject<std::remove_const_t<K>>>().get_object(),
              unwrap_primitive<V>(*value));
          }
          else if constexpr (std::is_arithmetic<V>::value ||
                             std::is_same<std::remove_const_t<V>, std::string>::value)
          {
            this->get_object().emplace(
              key.as<HostObject<std::remove_const_t<K>>>().get_object(),
              unwrap_primitive<std::remove_const_t<V>>(*value));
          }
          else
          {
            if (this->has_key(key))
            {
              this->get_object().erase(key.as<HostObject<K>>().get_object());
            }
            this->get_object().emplace(
              key.as<HostObject<K>>().get_object(),
              value->as<HostObject<std::remove_const_t<V>>>().get_object());
          }
        }
      }
    }

    /*!
     * @brief Retrieves a value V stored under a key K.
     *
     * Returns NIL if the underlying map does not contain a key K.
     */
    sptr_sobject get_sptr_property(const Lisple::Object& key) const override
    {
      if (this->has_key(key))
      {
        if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
        {
          if constexpr (std::is_arithmetic<V>::value ||
                        std::is_same<std::remove_const_t<V>, std::string>::value)
          {
            return wrap_primitive<std::remove_const_t<V>>(
              this->get_object().at(unwrap_primitive<K>(key)));
          }
          else
          {
            return A1::make_ref(
              this->get_object().at(unwrap_primitive<std::remove_const_t<K>>(key)));
          }
        }
        else
        {
          if constexpr (std::is_arithmetic<V>::value || std::is_same<V, std::string>::value)
          {
            return wrap_primitive<std::remove_const_t<V>>(
              this->get_object().at(key.as<HostObject<K>>().get_object()));
          }
          else
          {
            return A2::make_ref(this->get_object().at(key.as<HostObject<K>>().get_object()));
          }
        }
      }
      return Lisple::NIL;
    }

    /*!
     * @brief Tests if the underlying map contains a specific key.
     */
    bool has_key(const Lisple::Object& key) const override
    {
      if (key != *Lisple::NIL && _traits->key_type->is_type_of(key))
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

    /*!
     * @brief Returns a vector of all keys contained within the underlying map.
     */
    const Lisple::sptr_sobject_v keys() const override
    {
      Lisple::sptr_sobject_v keys;
      keys.reserve(this->get_object().size());

      if constexpr (std::is_arithmetic<K>::value || std::is_same<K, std::string>::value)
      {
        for (auto& [k, v] : this->get_object())
          keys.push_back(wrap_primitive<K>(k));
      }
      else
      {
        for (auto& [k, v] : this->get_object())
          keys.push_back(A1::make_ref(k));
      }

      return keys;
    }

    /*!
     * @see Lisple::Seq::size
     *
     * The returned size reflects the number of key/value pairs and not the
     * total amount of elements.
     */
    unsigned int size() const override { return this->get_object().size(); }

    static std::shared_ptr<StdMapAdapter<K, V, A1, A2>> make_ref(const std::map<K, V>& ref)
    {
      return std::make_shared<StdMapAdapter<K, V, A1, A2>>(const_cast<std::map<K, V>&>(ref));
    }
  };

  /*! @brief Pre-defined specialization of StdMapAdapter<int, std::string> */
  template class StdMapAdapter<int, std::string>;
  /*! @brief Pre-defined specialization of StdMapAdapter<uint8_t, short> */
  template class StdMapAdapter<uint8_t, short>;

  typedef StdMapAdapter<int, std::string> StdMapIntToString;
  typedef StdMapAdapter<uint8_t, short> StdMapUint8ToShort;
} // namespace Lisple

#endif
