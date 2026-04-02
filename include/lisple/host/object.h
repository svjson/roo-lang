
#ifndef LISPLE__HOST__OBJECT_H
#define LISPLE__HOST__OBJECT_H

#include <functional>

#include <lisple/form.h>
#include <lisple/host/accessor.h>
#include <lisple/host/type.h>
#include <lisple/macro_support.h>
#include <lisple/runtime/value.h>

// clang-format off

/**
 * NATIVE_ADAPTER traits
 */
#define __NOBJ_TRAITS_DECL                                              \
  protected:                                                            \
   static Lisple::NativeObjectTraits* _traits;                          \
   const Lisple::NativeObjectTraits* get_traits() const override;       \
   static const Lisple::NativeObjectTraits* traits();                   \


/*
 * NATIVE_ADAPTER_STATIC_FACTORY
 *
 * Macro used internally by Host Adapter declaration macros to add
 * static factory methods to the Host Adapter classes.
 */
#define NATIVE_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)                \
  /*!                                                                   \
   * @brief Static factory method that constructs a new instance of the \
   * underlying object type and wraps it in an adapter instance         \
   */                                                                   \
  template <typename... Args>                                           \
  static Lisple::sptr_rtval make_unique(Args&&... args)                 \
  {                                                                     \
    return Lisple::RTValue::native_object(                              \
      std::make_shared<AD_CLASS>(std::make_unique<H_CLASS>(std::forward<Args>(args)...)) \
    );                                                                  \
  }                                                                     \
  /*!                                                                   \
   * @brief Static factory method that constructs a new instance of the \
   * underlying object type and wraps it in an adapter instance         \
   */                                                                   \
  template <typename... Args>                                           \
  static Lisple::sptr_rtval make_shared(Args&&... args)                 \
  {                                                                     \
    return Lisple::RTValue::native_object(                              \
      std::make_shared<AD_CLASS>(std::make_shared<H_CLASS>(std::forward<Args>(args)...)) \
    );                                                                  \
  }                                                                     \
  /*!                                                                   \
   * @brief Static factory method that wraps an existing shared pointed \
   * underlying object and wraps it in an adapter instance              \
   */                                                                   \
  template <typename... Args>                                           \
  static Lisple::sptr_rtval wrap(const std::shared_ptr<H_CLASS>& sptr)  \
  {                                                                     \
    return Lisple::RTValue::native_object(                              \
      std::make_shared<AD_CLASS>(sptr)                                  \
    );                                                                  \
  }                                                                     \
  /*!                                                                   \
   * @brief Static factory method the wraps an instance of the underlying\
   * object in an adapter instance                                      \
   */                                                                   \
  static Lisple::sptr_rtval make_ref(const H_CLASS& ref)                \
  {                                                                     \
    return Lisple::RTValue::native_object(                              \
      std::make_shared<AD_CLASS>(const_cast<H_CLASS&>(ref))             \
    );                                                                  \
  }


/* NATIVE_ADAPTER_MAIN_DECL
 *
 * Macro that generates the class definition of a regular HostObject
 * specialization, including constructors, static factory methods and
 * facilities for the adapter traits.
 */
#define NATIVE_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                     \
  class AD_CLASS : public Lisple::NativeObject<H_CLASS>                 \
  {                                                                     \
  __NOBJ_TRAITS_DECL                                                    \
  public:                                                               \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class and takes   \
    * ownership of the unique pointer.                                  \
    */                                                                  \
   AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr);                        \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class holding a   \
    * shared pointer to the wrapped value.                              \
    */                                                                  \
   AD_CLASS(std::shared_ptr<H_CLASS>&& obj_ptr);                        \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class that holds  \
    * the supplied reference. This effectively means that the adapter   \
    * object is not responsible for the life-cycle of the referenced    \
    * object and may end up with a dangling pointer                     \
    */                                                                  \
   AD_CLASS(H_CLASS& obj_ref);                                          \
   H_CLASS& get_object() const;                                         \
   NATIVE_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define NATIVE_ADAPTER_END_DECL                   \
  }

/* NATIVE_ADAPTER_GETTER_DECL
 *
 * Declares a single host adapter getter accessor function
 */
#define NATIVE_ADAPTER_GETTER_DECL(PROP_NAME)             \
  /*! @brief Auto-generated getter */                   \
  Lisple::sptr_rtval get_##PROP_NAME() const;

/* __NATIVE_ADAPTER_GETTERS
 *
 * Wrapper around __VA_MACRO to declare a variadic number of getters
 */
#define __NATIVE_ADAPTER_GETTERS(...) \
  __VA_MACRO(NATIVE_ADAPTER_GETTER_DECL, __VA_ARGS__)

/* NATIVE_ADAPTER_SETTER_DECL
 *
 * Declares a single host adapter setter accessor function
 */
#define NATIVE_ADAPTER_SETTER_DECL(PROP_NAME)                      \
  /*! @brief Auto-generated setter */ \
  void set_##PROP_NAME(Lisple::Context* ctx, Lisple::sptr_rtval& value);

/* __NATIVE_ADAPTER_GETTERS
 *
 * Wrapper around __VA_MACRO to declare a variadic number of setters
 */
#define __NATIVE_ADAPTER_SETTERS(...) \
  __VA_MACRO(NATIVE_ADAPTER_SETTER_DECL, __VA_ARGS__)


/**
 * NATIVE_ADAPTER declaration variants
 */

#define NATIVE_ADAPTER__NO_PROPS(AD_CLASS, H_CLASS)    \
  NATIVE_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)          \
  NATIVE_ADAPTER_END_DECL

#define NATIVE_ADAPTER__WITH_GETTERS(AD_CLASS, H_CLASS, GET_PROPS)   \
  NATIVE_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                        \
  __NATIVE_ADAPTER_GETTERS GET_PROPS                                 \
  NATIVE_ADAPTER_END_DECL

#define NATIVE_ADAPTER__WITH_PROPS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  NATIVE_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                             \
  __NATIVE_ADAPTER_GETTERS GET_PROPS                                      \
  __NATIVE_ADAPTER_SETTERS SET_PROPS                                      \
  NATIVE_ADAPTER_END_DECL

#define NATIVE_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS, ...) \
  NATIVE_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                             \
  __NATIVE_ADAPTER_GETTERS GET_PROPS                                      \
  __NATIVE_ADAPTER_SETTERS SET_PROPS                                      \
  __ESCAPE __VA_ARGS__                                                  \
  NATIVE_ADAPTER_END_DECL

/**
 * NATIVE_ADAPTER main declaration macro
 */

#define NATIVE_ADAPTER(AD_CLASS, H_CLASS, ...)                            \
  __SELECT_MACRO__4(0, ##__VA_ARGS__, NATIVE_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS, NATIVE_ADAPTER__WITH_PROPS, NATIVE_ADAPTER__WITH_GETTERS, NATIVE_ADAPTER__NO_PROPS) \
  (AD_CLASS, H_CLASS, ##__VA_ARGS__)

/* __ADAPTER_TRAITS_COMMON
 *
 * Generates AdapterTraits facilities common between regular host adapters
 * and derived host adapters.
 */
#define __NOBJ_TRAITS_COMMON(AD_CLASS)                                  \
  Lisple::NativeObjectTraits* AD_CLASS::_traits = nullptr;                   \
  const Lisple::NativeObjectTraits* AD_CLASS::get_traits() const             \
  {                                                                     \
    return AD_CLASS::traits();                                          \
  }

/* __NOBJ_TRAITS_IMPL
 *
 * Generates AdapterTraits facilities for regular host adapters.
 */
#define __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, ACCESSOR_MAP)              \
  __NOBJ_TRAITS_COMMON(AD_CLASS)                                        \
  const Lisple::NativeObjectTraits* AD_CLASS::traits()                       \
  {                                                                     \
    if (!AD_CLASS::_traits)                                             \
    {                                                                   \
      AD_CLASS::_traits = new Lisple::NativeObjectTraits(HOBJ_T, Lisple::NAccessorTable ACCESSOR_MAP); \
    }                                                                   \
    return AD_CLASS::_traits;                                           \
  }

#define __NOBJ_P_GETTER(AD_CLASS, FN) [](const Lisple::NativeObjectBase* adapter) { return dynamic_cast<const AD_CLASS*>(adapter)->FN(); }
#define __NOBJ_P_SETTER(AD_CLASS, FN) [](Lisple::NativeObjectBase* adapter, Lisple::Context* ctx, Lisple::Object& value) { dynamic_cast<AD_CLASS*>(adapter)->FN(ctx, value); }

#define __NOBJ_P_NO_GETTER Lisple::n_no_getter
#define __NOBJ_P_NO_SETTER Lisple::n_no_setter

#define __NOBJ_P_GET_SINGLE(FN) get_##FN
#define __NOBJ_P_GET_DUAL(_1, FN) get_##FN

#define __NOBJ_P_SET_SINGLE(FN) set_##FN
#define __NOBJ_P_SET_DUAL(_1, FN) set_##FN

#define __NOBJ_P_GET(AD_CLASS, FN, ...) {std::make_shared<Lisple::Key>(#FN), Lisple::Accessors(P_GETTER(AD_CLASS, __SELECT_MACRO__2(0, ##__VA_ARGS__, P_GET_DUAL, P_GET_SINGLE)(FN, ##__VA_ARGS__)), P_NO_SETTER)}

#define __NOBJ_P_GET_SET(AD_CLASS, FN, ...) {\
    #FN, \
    Lisple::Accessors(P_GETTER(AD_CLASS, &AD_CLASS::__SELECT_MACRO__2(0, ##__VA_ARGS__, P_GET_DUAL, P_GET_SINGLE)(FN, ##__VA_ARGS__)), \
                      P_SETTER(AD_CLASS, &AD_CLASS::__SELECT_MACRO__2(0, ##__VA_ARGS__, P_SET_DUAL, P_SET_SINGLE)(FN, ##__VA_ARGS__)))}



#define NOBJ_GET(AD_CLASS, KEY, FN) {KEY, Lisple::NAccessors(__NOBJ_P_GETTER(AD_CLASS, P_GET_SINGLE(FN)), __NOBJ_P_NO_SETTER)}
#define NOBJ_SET(AD_CLASS, KEY, FN) {KEY, Lisple::NAccessors(__NOBJ_P_NO_GETTER, __NOBJ_P_SETTER(__NOBJ_P_SET_SINGLE(AD_CLASS, FN)))}
#define NOBJ_GET_SET(AD_CLASS, KEY, FN) {KEY, Lisple::NAccessors(__NOBJ_P_GETTER(AD_CLASS, __NOBJ_P_GET_SINGLE(FN)), \
                                                             __NOBJ_P_SETTER(AD_CLASS, __NOBJ_P_SET_SINGLE(FN))) }


/*
 * __NOBJ_GET_BASE_OBJECT_IMPL
 *
 * Generates a method for retrieving the wrapped object of host adapter
 * by its concrete type.
 */
#define __NOBJ_GET_BASE_OBJECT_IMPL(AD_CLASS, H_CLASS)  \
  H_CLASS& AD_CLASS::get_object() const                 \
  {                                                     \
    return object->get_object();                        \
  }


/**
 * NATIVE_ADAPTER_IMPL variants
 */

#define NATIVE_ADAPTER_IMPL__NO_ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T)      \
  __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, (Lisple::NO_N_ACCESSORS))         \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                  \
    : NativeObject(obj_ptr) { }                                             \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                    \
    : NativeObject(obj_ref) { }                                             \
  __GET_BASE_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)

#define NATIVE_ADAPTER_IMPL__ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T, ACCESSOR_MAP)   \
  __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, ACCESSOR_MAP) \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                  \
    : NativeObject(obj_ptr) {}                                              \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                    \
    : NativeObject(obj_ref) {}                                              \
  __NOBJ_GET_BASE_OBJECT_IMPL(AD_CLASS, H_CLASS)




/**
 * NATIVE_ADAPTER_IMPL implementation
 */

#define NATIVE_ADAPTER_IMPL(AD_CLASS, H_CLASS, HOBJ_T, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__NO_ACCESSORS)(AD_CLASS, H_CLASS, HOBJ_T, ##__VA_ARGS__)

// clang-format on

namespace Lisple
{
  template <typename T> T& obj(const RTValue& v)
  {
    return v.obj()->as<HostObject<T>>().get_object();
  }

  class NAccessors;
  class NAccessorTable;
  struct NativeObjectTraits;

  extern const NAccessorTable NO_N_ACCESSORS;

  typedef std::unordered_map<std::string, NAccessors> n_acc_map;
  typedef std::unordered_map<sptr_rtval, NAccessors> key_n_acc_map;

  /*! @brief Convenience type definition for GETTER function references */
  typedef std::function<sptr_rtval(const NativeObjectBase*)> n_acc_get_t;
  /*! @brief Convenience type definition for SETTER function references */
  typedef std::function<void(NativeObjectBase*, Context*, Lisple::sptr_rtval&)> n_acc_set_t;

  /*!
   * @brief Stock getter-implementation for non-gettable properties that will
   * always return NIL
   */
  extern const n_acc_get_t n_no_getter;
  /*!
   * @brief Stock setter-implementation for non-settable properties that will
   * always throw InvocationException
   */
  extern const n_acc_set_t n_no_setter;

  /*!
   * @brief Holds function references to getters and setters for a specific
   * field. Both must be defined for every field, but they may optionally refer
   * to no_getter / no_setter.
   */
  class NAccessors
  {
   public:
    n_acc_get_t getter;
    n_acc_set_t setter;

    NAccessors(const n_acc_get_t& getter, const n_acc_set_t& setter);
  };

  /*!
   * @brief A lookup table for and specification of accessor methods and their
   * corresponding keys of a HostObject.
   *
   * Stored as part of @see Lisple::NativeObjectTraits, or one of its derived types.
   */
  class NAccessorTable
  {
   public:
    /*!
     * @brief A set of valid keys for the described HostObject
     */
    std::vector<sptr_rtval> keys;
    /*!
     * @brief Lookup table for accessor implementations by key/name.
     */
    n_acc_map accessor_map;

    /*!
     * @brief Default constructor providing an empty table for types without
     * properties or types with a custom property resolution mechanism.
     */
    NAccessorTable() = default;

    /*!
     * @brief Constructs an AccessorTable instance from a map of accessor
     * specifications.
     */
    NAccessorTable(const key_n_acc_map& accessors);

    /*!
     * @brief Tests if an accessor is provided for a specific key.
     */
    bool has_key(const Object& key) const;

    /*!
     * @brief Looks up the accessor definition for a specific key
     */
    const NAccessors* lookup(const RTValue& key) const;
  };

  /*!
   * @brief Constructs a new AccessorTable from an existing instance and an
   * additional accessor map. Used for defining accessor tables for derived
   * types.
   */
  NAccessorTable merge_acc(const NAccessorTable& al1, const key_n_acc_map& kam2);

  /*!
   * @brief Defines the type traits of a HostObject specializing, including
   * its HostTypeRef and AccessorTable.
   */
  struct NativeObjectTraits
  {
    const HostTypeRef* type_ref;
    NAccessorTable accessor_table;

    NativeObjectTraits(const HostTypeRef* type_ref, const NAccessorTable& accessor_table);
    virtual ~NativeObjectTraits() = default;
  };

  struct NativeObjectBase
  {
    virtual ~NativeObjectBase() = default;

    sptr_rtval get_property(const RTValue& property) const;
    virtual const NativeObjectTraits* get_traits() const = 0;
    const HostTypeRef* get_host_type() const;
    const NAccessorTable& accessor_table() const;
  };

  template <typename T> struct NativeObject : public NativeObjectBase
  {
    std::unique_ptr<ValueHolder<T>> object;

    NativeObject(std::unique_ptr<T>& object)
      : object(std::make_unique<HostObjectValue<T>>(object))
    {
    }

    NativeObject(T& object)
      : object(std::make_unique<HostObjectRef<T>>(object))
    {
    }
  };

} // namespace Lisple

#endif /* LISPLE__HOST__OBJECT_H */
