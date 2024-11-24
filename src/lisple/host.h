
#ifndef __HOST_H_
#define __HOST_H_

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "context.h"
#include "form.h"
#include "type.h"
#include "exception.h"

/*
 * Helper macro for selecting a specific macro implementation depending on the
 * size of varargs.
 */
#define __SELECT_MACRO__2(_1, _2, MACRO_NAME, ...) MACRO_NAME

/*
 * Helper macro for expanding values eliminating containing parentheses.
 */
#define ESC(...) __VA_ARGS__

/*
 * Helper macros for defining a dynamic number of adapter properties.
 */
#define __VA_REP_1(M_NAME, PROP)                \
  M_NAME(PROP)

#define __VA_REP_2(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_1(M_NAME,__VA_ARGS__)

#define __VA_REP_3(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_2(M_NAME,__VA_ARGS__)

#define __VA_REP_4(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_3(M_NAME,__VA_ARGS__)

#define __VA_REP_5(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_4(M_NAME,__VA_ARGS__)

#define __VA_REP_6(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_5(M_NAME,__VA_ARGS__)

#define __VA_REP_7(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_6(M_NAME,__VA_ARGS__)

#define __VA_REP_8(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_7(M_NAME,__VA_ARGS__)

#define __VA_REP_9(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_8(M_NAME,__VA_ARGS__)

#define __VA_REP_10(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_9(M_NAME,__VA_ARGS__)

#define __VA_REP_11(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_10(M_NAME,__VA_ARGS__)

#define __VA_REP_12(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_11(M_NAME,__VA_ARGS__)

#define __VA_REP_13(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_12(M_NAME,__VA_ARGS__)

#define __VA_REP_14(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_13(M_NAME,__VA_ARGS__)

#define __VA_REP_15(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_14(M_NAME,__VA_ARGS__)

#define __VA_REP_16(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_15(M_NAME,__VA_ARGS__)

#define __VA_REP_17(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_16(M_NAME,__VA_ARGS__)

#define __VA_REP_18(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_17(M_NAME,__VA_ARGS__)

#define __VA_REP_19(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_18(M_NAME,__VA_ARGS__)

#define __VA_REP_20(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_19(M_NAME,__VA_ARGS__)

#define __VA_REP_21(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_20(M_NAME,__VA_ARGS__)

#define __VA_REP_22(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_21(M_NAME,__VA_ARGS__)

#define __VA_REP_23(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_22(M_NAME,__VA_ARGS__)

#define __VA_REP_24(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_23(M_NAME,__VA_ARGS__)

#define __VA_REP_25(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_24(M_NAME,__VA_ARGS__)

#define __VA_REP_26(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_25(M_NAME,__VA_ARGS__)

#define __VA_REP_27(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_26(M_NAME,__VA_ARGS__)

#define __VA_REP_28(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_27(M_NAME,__VA_ARGS__)

#define __VA_REP_29(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_28(M_NAME,__VA_ARGS__)

#define __VA_REP_30(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_29(M_NAME,__VA_ARGS__)

#define __VA_SELECT_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, MACRO_NAME, ...) MACRO_NAME

#define __VA_MACRO(M_NAME, ...) __VA_SELECT_MACRO(__VA_ARGS__,  \
                                                  __VA_REP_30,  \
                                                  __VA_REP_29,  \
                                                  __VA_REP_28,  \
                                                  __VA_REP_27,  \
                                                  __VA_REP_26,  \
                                                  __VA_REP_25,  \
                                                  __VA_REP_24,  \
                                                  __VA_REP_23,  \
                                                  __VA_REP_22,  \
                                                  __VA_REP_21,  \
                                                  __VA_REP_20,  \
                                                  __VA_REP_19,  \
                                                  __VA_REP_18,  \
                                                  __VA_REP_17,  \
                                                  __VA_REP_16,  \
                                                  __VA_REP_15,  \
                                                  __VA_REP_14,  \
                                                  __VA_REP_13,  \
                                                  __VA_REP_12,  \
                                                  __VA_REP_11,  \
                                                  __VA_REP_10,  \
                                                  __VA_REP_9,   \
                                                  __VA_REP_8,   \
                                                  __VA_REP_7,   \
                                                  __VA_REP_6,   \
                                                  __VA_REP_5,   \
                                                  __VA_REP_4,   \
                                                  __VA_REP_3,   \
                                                  __VA_REP_2,   \
                                                  __VA_REP_1)   \
  (M_NAME, __VA_ARGS__)

/*
 * __HOST_TYPE
 * __HOST_TYPE__NO_MAKE_FN
 * HOST_TYPE
 *
 * Macro short-hand for defining HostTypeRef constants.
 */
#define __HOST_TYPE(CONST, NAME, MAKE_FN) \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter with a      \
   * registered make function for automatic coercion.                   \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME, MAKE_FN);

#define __HOST_TYPE__NO_MAKE_FN(CONST, NAME)                            \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter             \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME);

#define HOST_TYPE(CONST, NAME, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_TYPE, __HOST_TYPE__NO_MAKE_FN)(CONST, NAME, ##__VA_ARGS__)

/*
 * __HOST_SUB_TYPE
 * __HOST_SUB_TYPE__NO_MAKE_FN
 * HOST_SUB_TYPE
 *
 * Macro short-hand for defining HostTypeRef constants derived from
 * other HostTypeRef constants.
 */
#define __HOST_SUB_TYPE(CONST, NAME, PARENT_TYPE, MAKE_FN)              \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter that is a   \
   * subtype of another Adapter, and with a registered make-function for\
   * automatic coercion.                                                \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST =                              \
    Lisple::HostTypeRef(NAME, PARENT_TYPE, MAKE_FN);

#define __HOST_SUB_TYPE__NO_MAKE_FN(CONST, NAME, PARENT_TYPE)           \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter that is a   \
   * subtype of another Adapter.                                        \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST =                              \
    Lisple::HostTypeRef(NAME, PARENT_TYPE);

#define HOST_SUB_TYPE(CONST, NAME, PARENT_TYPE, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_SUB_TYPE, __HOST_SUB_TYPE__NO_MAKE_FN)(CONST, NAME, PARENT_TYPE, ##__VA_ARGS__)

#define __ADAPTER_TRAITS_DECL                                           \
  protected:                                                            \
   static Lisple::AdapterTraits* _traits;                               \
   const Lisple::AdapterTraits* get_traits() const override;            \
   static const Lisple::AdapterTraits* traits();                        \

/*
 * HOST_ADAPTER_STATIC_FACTORY
 *
 * Macro used internally by Host Adapter declaration macros to add
 * static factory methods to the Host Adapter classes.
 */
#define HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)                  \
  /*!                                                                   \
   * @brief Static factor method the constructs a new instance of the   \
   * underlying object and wraps it in an adapter instance              \
   */                                                                   \
  template <typename T, typename... Args>                               \
  static std::shared_ptr<AD_CLASS> make(Args&&... args)                 \
  {                                                                     \
    return std::make_shared<AD_CLASS>(std::make_unique<T>(std::forward<Args>(args)...)); \
  }                                                                     \
  /*!                                                                   \
   * @brief Static factor method the wraps an instance of the underlying \
   * object in an adapter instance                                      \
   */                                                                   \
  static std::shared_ptr<AD_CLASS> make_ref(const H_CLASS& ref)         \
  {                                                                     \
    return std::make_shared<AD_CLASS>(const_cast<H_CLASS&>(ref));       \
  }

/* HOST_ADAPTER_MAIN_DECL
 *
 * Macro that generates the class definition of a regular HostObject
 * specialization, including constructors, static factory methods and
 * facilities for the adapter traits.
 */
#define HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                       \
  class AD_CLASS : public Lisple::HostObject<H_CLASS>                   \
  {                                                                     \
  __ADAPTER_TRAITS_DECL                                                 \
  public:                                                               \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class and takes   \
    * ownership of the unique pointer.                                  \
    */                                                                  \
   AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr);                        \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class that holds  \
    * the supplied reference. This effectively means that the adapter   \
    * object is not responsible for the life-cycle of the referenced    \
    * object and may end up with a dangling pointer                     \
    */                                                                  \
   AD_CLASS(H_CLASS& obj_ref);                                          \
   H_CLASS& get_self_object() const;                                    \
   HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

/* SUB_ADAPTER_MAIN_DECL
 *
 * Macro that generates the class definition of a derived HostObject
 * specialization, including constructors, static factory methods and
 * facilities for the adapter traits.
 */
#define SUB_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS) \
  class AD_CLASS : public AD_SUP_CLASS                                  \
  {                                                                     \
   __ADAPTER_TRAITS_DECL                                                \
   public:                                                              \
    static const Lisple::HostTypeRef* host_type;                        \
    AD_CLASS(std::unique_ptr<H_SUP_CLASS>&& obj_ptr);                   \
    AD_CLASS(H_SUP_CLASS& obj_ref);                                     \
    H_CLASS& get_self_object() const;                                   \
    HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_SUP_CLASS)

#define HOST_ADAPTER_END_DECL                   \
  }

/* HOST_ADAPTER_GETTER_DECL
 *
 * Declares a single host adapter getter accessor function
 */
#define HOST_ADAPTER_GETTER_DECL(PROP_NAME)             \
  /*! @brief Auto-generated getter */                   \
  Lisple::sptr_sobject get_##PROP_NAME() const;

/* __HOST_ADAPTER_GETTERS
 *
 * Wrapper around __VA_MACRO to declare a variadic number of getters
 */
#define __HOST_ADAPTER_GETTERS(...) \
  __VA_MACRO(HOST_ADAPTER_GETTER_DECL, __VA_ARGS__)

/* HOST_ADAPTER_SETTER_DECL
 *
 * Declares a single host adapter setter accessor function
 */
#define HOST_ADAPTER_SETTER_DECL(PROP_NAME)                      \
  /*! @brief Auto-generated setter */ \
  void set_##PROP_NAME(Lisple::Context* ctx, Lisple::Object& value);

/* __HOST_ADAPTER_GETTERS
 *
 * Wrapper around __VA_MACRO to declare a variadic number of setters
 */
#define __HOST_ADAPTER_SETTERS(...) \
  __VA_MACRO(HOST_ADAPTER_SETTER_DECL, __VA_ARGS__)

#define SUB_ADAPTER__NO_PROPS(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS)   \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_GETTERS(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, GET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS)   \
  __HOST_ADAPTER_GETTERS GET_PROPS                                      \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_PROPS(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, GET_PROPS, SET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS)   \
  __HOST_ADAPTER_GETTERS GET_PROPS                                      \
  __HOST_ADAPTER_SETTERS SET_PROPS                                      \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__NO_PROPS(AD_CLASS, H_CLASS)    \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)          \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_GETTERS(AD_CLASS, H_CLASS, GET_PROPS)   \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                        \
  __HOST_ADAPTER_GETTERS GET_PROPS                                 \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_PROPS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                             \
  __HOST_ADAPTER_GETTERS GET_PROPS                                      \
  __HOST_ADAPTER_SETTERS SET_PROPS                                      \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS, ...) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                             \
  __HOST_ADAPTER_GETTERS GET_PROPS                                      \
  __HOST_ADAPTER_SETTERS SET_PROPS                                      \
  __ESCAPE __VA_ARGS__                                                  \
  HOST_ADAPTER_END_DECL

#define SELECT_ADAPTER_MACRO(_1, _2, _3, _4, MACRO_NAME, ...) MACRO_NAME
#define SELECT_SUB_ADAPTER_MACRO(_1, _2, _3,  MACRO_NAME, ...) MACRO_NAME
#define HOST_SUB_ADAPTER(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, ...)         \
  SELECT_SUB_ADAPTER_MACRO(0, ##__VA_ARGS__, SUB_ADAPTER__WITH_PROPS, SUB_ADAPTER__WITH_GETTERS, SUB_ADAPTER__NO_PROPS) \
  (AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, ##__VA_ARGS__)
#define HOST_ADAPTER(AD_CLASS, H_CLASS, ...)                            \
  SELECT_ADAPTER_MACRO(0, ##__VA_ARGS__, HOST_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS, HOST_ADAPTER__WITH_PROPS, HOST_ADAPTER__WITH_GETTERS, HOST_ADAPTER__NO_PROPS) \
  (AD_CLASS, H_CLASS, ##__VA_ARGS__)

/*
 * __GET_BASE_SELF_OBJECT_IMPL
 *
 * Generates a method for retrieving the wrapped object of host adapter
 * by its concrete type. In this case, when generated for a regular host
 * adapter, it is the same as AbstractHostObject::get_object
 */
#define __GET_BASE_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)  \
  H_CLASS& AD_CLASS::get_self_object() const            \
  {                                                     \
    return get_object();                                \
  }

/*
 * __GET_DERIVED_SELF_OBJECT_IMPL
 *
 * Generates a method for retrieving the wrapped object of host adapter
 * by its concrete type. In this case, when generated for a derived host
 * adapter, it performs a dynamic cast of the base type to the derived type.
 */
#define __GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)  \
  H_CLASS& AD_CLASS::get_self_object() const               \
  {                                                        \
    return dynamic_cast<H_CLASS&>(get_object());           \
  }

/* __ADAPTER_TRAITS_COMMON
 *
 * Generates AdapterTraits facilities common between regular host adapters
 * and derived host adapters.
 */
#define __ADAPTER_TRAITS_COMMON(AD_CLASS)                               \
  Lisple::AdapterTraits* AD_CLASS::_traits = nullptr;                   \
  const Lisple::AdapterTraits* AD_CLASS::get_traits() const             \
  {                                                                     \
    return AD_CLASS::traits();                                          \
  }

/* __ADAPTER_TRAITS_IMPL
 *
 * Generates AdapterTraits facilities for regular host adapters.
 */
#define __ADAPTER_TRAITS_IMPL(AD_CLASS, HOBJ_T, ACCESSOR_MAP)           \
  __ADAPTER_TRAITS_COMMON(AD_CLASS)                                     \
  const Lisple::AdapterTraits* AD_CLASS::traits()                       \
  {                                                                     \
    if (!AD_CLASS::_traits)                                             \
    {                                                                   \
      AD_CLASS::_traits = new Lisple::AdapterTraits(HOBJ_T, Lisple::AccessorTable ACCESSOR_MAP); \
    }                                                                   \
    return AD_CLASS::_traits;                                           \
  }

/* __ADAPTER_TRAITS_IMPL
 *
 * Generates AdapterTraits facilities for derived host adapters.
 */
#define __ADAPTER_TRAITS_SUB_IMPL(AD_CLASS, AD_SUP_CLASS, HOBJ_T, ACCESSOR_MAP) \
  __ADAPTER_TRAITS_COMMON(AD_CLASS)                                     \
  const Lisple::AdapterTraits* AD_CLASS::traits()                       \
  {                                                                     \
    if (!AD_CLASS::_traits)                                             \
    {                                                                   \
      AD_CLASS::_traits = new Lisple::AdapterTraits(                    \
        HOBJ_T, Lisple::merge_acc(AD_SUP_CLASS::traits()->accessor_table, \
                                  ESC ACCESSOR_MAP));                   \
    }                                                                   \
    return AD_CLASS::_traits;                                           \
  }

#define HOST_SUB_ADAPTER_IMPL__NO_ACCESSORS(AD_SUP_CLASS, AD_CLASS, H_CLASS) \
  __ADAPTER_TRAITS_SUB_IMPL(AD_CLASS, AD_SUP_CLASS, HOBJ_T, NO_ACCESSORS) \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                \
    : AD_SUP_CLASS(std::move(obj_ptr)) {}                               \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                  \
    : AD_SUP_CLASS(obj_ref) {}                                          \
  __GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)

#define HOST_SUB_ADAPTER_IMPL__ACCESSORS(AD_CLASS, H_CLASS, AD_SUP_CLASS, SUP_H_CLASS, HOBJ_T, ACCESSOR_MAP) \
  __ADAPTER_TRAITS_SUB_IMPL(AD_CLASS, AD_SUP_CLASS, HOBJ_T, ACCESSOR_MAP) \
  AD_CLASS::AD_CLASS(std::unique_ptr<SUP_H_CLASS>&& obj_ptr)            \
    : AD_SUP_CLASS(std::move(obj_ptr)) {}                               \
  AD_CLASS::AD_CLASS(SUP_H_CLASS& obj_ref)                              \
    : AD_SUP_CLASS(obj_ref) {}                                          \
  __GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)

#define HOST_ADAPTER_IMPL__NO_ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T)      \
  __ADAPTER_TRAITS_IMPL(AD_CLASS, HOBJ_T, (Lisple::NO_ACCESSORS))       \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                \
    : HostObject(obj_ptr) { }                                           \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                  \
    : HostObject(obj_ref) { }                                           \
  __GET_BASE_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)

#define HOST_ADAPTER_IMPL__ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T, ACCESSOR_MAP)   \
  __ADAPTER_TRAITS_IMPL(AD_CLASS, HOBJ_T, ACCESSOR_MAP) \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                \
    : HostObject(obj_ptr) {}                                            \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                  \
    : HostObject(obj_ref) {}                                            \
  __GET_BASE_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)

#define SELECT_SUB_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME
#define SELECT_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define HOST_SUB_ADAPTER_IMPL(AD_CLASS, H_CLASS, AD_SUP_CLASS, SUP_H_CLASS, HOBJ_T, ...) SELECT_SUB_ADAPTER_IMPL_MACRO(0, ##__VA_ARGS__, HOST_SUB_ADAPTER_IMPL__ACCESSORS, HOST_SUB_ADAPTER_IMPL__NO_ACCESSORS)(AD_CLASS, H_CLASS, AD_SUP_CLASS, SUP_H_CLASS, HOBJ_T, ##__VA_ARGS__)

#define HOST_ADAPTER_IMPL(AD_CLASS, H_CLASS, HOBJ_T, ...) SELECT_ADAPTER_IMPL_MACRO(0, ##__VA_ARGS__, HOST_ADAPTER_IMPL__ACCESSORS, HOST_ADAPTER_IMPL__NO_ACCESSORS)(AD_CLASS, H_CLASS, HOBJ_T, ##__VA_ARGS__)

/* Field accessor macros */
#define __ADAPTER_FIELD_ACCESSOR(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)

#define __ADAPTER_FIELD_ACCESSOR__SAME_NAME(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define __ADAPTER_FIELD_ACCESSOR_MACROS(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_FIELD_ACCESSOR, __ADAPTER_FIELD_ACCESSOR__SAME_NAME)(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)


/* ADAPTER_PROP_GET
 *
 * Simply defines the method signature of a property getter. Should not be used
 * directly by application code unless the  specialized macros, such as any of
 * ADAPTER_PROP_GET__FIELD, ADAPTER_PROP_GET__METHOD or their variants do not
 * apply for the use case.
 *
 * Usage:
 * ADAPTER_PROP_GET(SomeHostAdapter, some_property)
 * {
 *   // actual getter code goes here
 * }
 */
#define ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)           \
  Lisple::sptr_sobject AD_CLASS::get_##PROP_NAME() const

/* ADAPTER_PROP_SET
 *
 * Simply defines the method signature of a property setter. Should not be used
 * directly by application code unless the  specialized macros, such as any of
 * ADAPTER_PROP_SET__FIELD, ADAPTER_PROP_SET__METHOD or their variants do not
 * apply for the use case.
 *
 * Usage:
 * ADAPTER_PROP_SET(SomeHostAdapter, some_property)
 * {
 *   // actual setter code goes here
 * }
 */
#define ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                  \
  void AD_CLASS::set_##PROP_NAME([[maybe_unused]]Lisple::Context* ctx, Lisple::Object& value)

/* __ADAPTER_PROP_GET__FIELD
 *
 * For internal use only.
 */
#define __ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    return std::make_shared<LISPLE_FORM>(get_self_object().OBJ_FIELD);           \
  }

#define __ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    get_self_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;                 \
  }

/* __ADAPTER_PROP_GET_P__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    return std::make_shared<LISPLE_FORM>(*get_self_object().OBJ_FIELD);          \
  }

/* __ADAPTER_PROP_GET_OPT__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    return get_self_object().OBJ_FIELD.has_value()                                 \
      ? std::make_shared<LISPLE_FORM>(*get_self_object().OBJ_FIELD)                \
      : Lisple::NIL;                                                               \
  }

#define __ADAPTER_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    if (*Lisple::NIL == value)                                                     \
      get_self_object().OBJ_FIELD = std::nullopt;                                  \
    else                                                                           \
      get_self_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;                 \
  }

/* __ADAPTER_PROP_GET_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    Lisple::sptr_sobject_v v;                                                                                \
    for (auto& obj : get_self_object().FIELD_NAME)                                                           \
    {                                                                                                        \
      v.push_back(std::make_shared<LISPLE_FORM>(obj));                                                       \
    }                                                                                                        \
    return std::make_shared<Lisple::Array>(v);                                                               \
  }

/* __ADAPTER_PROP_SET_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)     \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                    \
  {                                                                                        \
    get_self_object().FIELD_NAME.clear();                                                  \
    for (auto& obj : value.get_children())                                                 \
    {                                                                                      \
      get_self_object().FIELD_NAME.push_back(obj->as<LISPLE_FORM>().value);                \
    }                                                                                      \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    Lisple::sptr_sobject_v v;                                                                                \
    for (auto& obj : get_self_object().FIELD_NAME)                                                           \
    {                                                                                                        \
      v.push_back(std::make_shared<LISPLE_FORM>(obj));                                                       \
    }                                                                                                        \
    return std::make_shared<Lisple::Array>(v);                                                               \
  }

/* __ADAPTER_PROP_SET_HOST_OBJECT_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    get_self_object().FIELD_NAME.clear();                                                                    \
    for (auto& obj : value.get_children())                                                                   \
    {                                                                                                        \
      get_self_object().FIELD_NAME.push_back(obj->as<LISPLE_FORM>().get_self_object());                      \
    }                                                                                                        \
  }

/* __ADAPTER_PROP_GET_VECTOR_P__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    Lisple::sptr_sobject_v v;                                                            \
     for (auto obj : get_self_object().FIELD_NAME)                                       \
    {                                                                                    \
      v.push_back(std::make_shared<LISPLE_FORM>(*obj));                                  \
    }                                                                                    \
    return std::make_shared<Lisple::Array>(v);                                           \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                     \
  {                                                                                         \
    return LISPLE_FORM::make_ref(get_self_object().OBJ_FIELD);                              \
  }

#define __ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                     \
  {                                                                                         \
    get_self_object().OBJ_FIELD = value.as<LISPLE_FORM>().get_self_object();                \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)      \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                           \
  {                                                                                               \
    return get_self_object().OBJ_FIELD                                                            \
      ? LISPLE_FORM::make_ref(*get_self_object().OBJ_FIELD)                                       \
      : Lisple::NIL;                                                                              \
  }

#define __ADAPTER_PROP_SET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)      \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                           \
  {                                                                                               \
    get_self_object().OBJ_FIELD = *Lisple::NIL == value                                           \
      ? nullptr                                                                                   \
      : value.as<LISPLE_FORM>().get_object_ptr().get();                                           \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    return get_self_object().OBJ_FIELD.has_value()                                               \
      ? LISPLE_FORM::make_ref(*get_self_object().OBJ_FIELD)                                      \
      : Lisple::NIL;                                                                             \
  }

/* __ADAPTER_PROP_SET_HOST_OBJECT_OPT__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    if (*Lisple::NIL == value)                                                                   \
      get_self_object().OBJ_FIELD = std::nullopt;                                                \
    else                                                                                         \
      get_self_object().OBJ_FIELD = value.as<LISPLE_FORM>().get_self_object();                   \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    return get_self_object().OBJ_FIELD.count(MAP_KEY)                                            \
      ? LISPLE_FORM::make_ref(*get_self_object().OBJ_FIELD.at(MAP_KEY))                          \
      : Lisple::NIL;                                                                             \
  }

/* __ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    if (get_self_object().OBJ_FIELD.count(MAP_KEY))                                              \
    {                                                                                            \
      get_self_object().OBJ_FIELD.erase(MAP_KEY);                                                \
    }                                                                                            \
                                                                                                 \
    if (*Lisple::NIL != value)                                                                   \
    {                                                                                            \
      get_self_object().OBJ_FIELD.emplace(MAP_KEY, value.as<LISPLE_FORM>().get_self_object());   \
    }                                                                                            \
  }

/* __ADAPTER_PROP_GET__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_METHOD)   \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    return std::make_shared<LISPLE_FORM>(get_self_object().OBJ_METHOD());          \
  }

#define __ADAPTER_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)     \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    get_self_object().METHOD(value.as<LISPLE_FORM>().value);                     \
  }

/* __ADAPTER_PROP_GET_P__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                           \
  {                                                                               \
    return std::make_shared<LISPLE_FORM>(*get_self_object().METHOD());            \
  }

/* __ADAPTER_PROP_GET_VECTOR__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD_NAME) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    Lisple::sptr_sobject_v v;                                                            \
    for (auto& obj : get_self_object().METHOD_NAME())                                    \
    {                                                                                    \
      v.push_back(std::make_shared<LISPLE_FORM>(obj));                                   \
    }                                                                                    \
    return std::make_shared<Lisple::Array>(v);                                           \
  }

/* __ADAPTER_PROP_GET_VECTOR_P__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD_NAME) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                    \
  {                                                                                        \
    Lisple::sptr_sobject_v v;                                                              \
    for (auto& obj : get_self_object().METHOD_NAME())                                      \
    {                                                                                      \
      v.push_back(obj == nullptr ? Lisple::NIL : std::make_shared<LISPLE_FORM>(*obj));     \
    }                                                                                      \
    return std::make_shared<Lisple::Array>(v);                                             \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    return LISPLE_FORM::make_ref(get_self_object().METHOD());                            \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    return get_self_object().METHOD(value.as<LISPLE_FORM>().get_self_object());          \
  }


/* __ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                      \
  {                                                                                          \
    return get_self_object().METHOD().has_value()                                            \
      ? LISPLE_FORM::make_ref(*get_self_object().METHOD())                                   \
      : Lisple::NIL;                                                                         \
  }

/* ADAPTER_PROP_GET__FIELD - get value by field
 *
 * Generates a property getter implementation that retrieves the property value
 * directly via the object field. This means that the field in question needs to
 * be public, or otherwise accessible from the adapter class.
 *
 * The Lisple Form class needs to be supplied to construct the return value.
 *
 * Usage:
 * ADAPER_PROP_GET__FIELD(SomeHostAdapter, some_string, Lisple::String);
 */
#define ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)   \
  ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_OPT__FIELD - get std::optional value by field
 *
 * Generates a property getter implementation that retrieves the property value
 * of an std::optional member field. This means that the field in question
 * needs to be public, or otherwise accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_OPT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_OPT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)   \
  ADAPTER_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_P__FIELD - get pointer by field
 *
 * Generates a property getter implementation that retrives the property value
 * of a pointer member field. This means that the field in question needs
 * to be public, or otherwise accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_P__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_VECTOR__FIELD - get vector by field
 *
 * Generates a property getter implementation that retrives the property values
 * of an std::vector or std::list member field and generates a Lisple::Array.
 * This means that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)      \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_VECTOR__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)      \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_VECTOR__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD - get vector by field
 *
 * Generates a property getter implementation that retrives the property values
 * of an std::vector or std::list member field and generates a Lisple::Array.
 * This meagns that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)      \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)      \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT_VECTOR__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)



/* ADAPTER_PROP_GET_VECTOR_P__FIELD - get vector of pointers by field
 *
 * Generates a property getter implementation that retrives the property values
 * of a vector member field and generates a Lisple::Array. This means that the
 * field in question needs to be public, or otherwise accessible from the adapter
 * class.
 */
#define ADAPTER_PROP_GET_VECTOR_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_VECTOR_P__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT__FIELD - get host object instance by field
 *
 * Generates a property getter implementation that retrieves the property value
 * of a member field containing a host object that needs to be wrapped in an
 * adapter.

 * This means that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)  \
  ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD - get host object pointer by field
 *
 * Generates a property getter implementation that retrieves the property value
 * of a member field containing a host object pointer that needs to be wrapped
 * in an adapter.

 * This means that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT_P__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD - get host object by optional field
 *
 * Generates a property getter implementation that retrives the property value
 * of an std::optional member field containing a host object that needs to be
 * wrapped in a host adapter.
 *
 * This means that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT_OPT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD - get optional host object
 * from an std::map of the underlying object
 *
 * Generates a property getter implementation that retrives the property value
 * of an std::optional value of a member std::map field containing a host object
 * that needs to be wrapped in a host adapter.
 *
 * This means that the map field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD) \
  __ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD)

#define ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD) \
  __ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD) \
  ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD) \
  ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, MAP_FIELD)

/* ADAPTER_PROP_GET__METHOD - get value by field
 *
 * Generates a property getter implementation that retrives the property value
 * using a member function. This means that the field in question needs to be
 * public, or otherwise accessible from the adapter class.
 */
#define ADAPTER_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_P__METHOD - get pointer by method
 *
 * Generates a property getter implementation that retrives the property value
 * using a member function that returns a pointer. This means that the field in
 * question needs to be public, or otherwise accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_P__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_VECTOR__METHOD - get vector by method
 *
 * Generates a property getter implementation that retrives the property values
 * of a vector member function and generates a Lisple::Array. This means that the
 * field in question needs to be public, or otherwise accessible from the adapter
 * class.
 */
#define ADAPTER_PROP_GET_VECTOR__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_VECTOR__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_VECTOR_P__METHOD - get vector of pointers by method
 *
 * Generates a property getter implementation that retrives the property values
 * of a vector member function and generates a Lisple::Array. This means that the
 * field in question needs to be public, or otherwise accessible from the adapter
 * class.
 */
#define ADAPTER_PROP_GET_VECTOR_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_VECTOR_P__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT__METHOD - get host object by method
 *
 * Generates a property getter implementation that retrieves the property using
 * a member function returning a host object reference that needs to be wrapped
 * in an adapter.

 * This means that the field in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_HOST_OBJECT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \

/* ADAPTER_PROP_GET_HOST_OBJECT_P__METHOD - get host object pointer by method
 *
 * Generates a property getter implementation that retrieves the property using
 * a member function returning a host object pointer that needs to be wrapped
 * in an adapter.

 * This means that the method in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT_P__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT_P__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_HOST_OBJECT_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD - get optional host object by method
 *
 * Generates a property getter implementation that retrieves an optional property
 * using a member function
 *
 * This means that the method in question needs to be public, or otherwise
 * accessible from the adapter class.
 */
#define ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_SET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __ADAPTER_FIELD_ACCESSOR_MACROS(__ADAPTER_PROP_SET_HOST_OBJECT_OPT__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__) \
  ADAPTER_PROP_SET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)


#define P_GETTER(AD_CLASS, FN) [](const Lisple::AbstractHostObject* adapter) { return dynamic_cast<const AD_CLASS*>(adapter)->FN(); }
#define P_SETTER(AD_CLASS, FN) [](Lisple::AbstractHostObject* adapter, Lisple::Context* ctx, Lisple::Object& value) { dynamic_cast<AD_CLASS*>(adapter)->FN(ctx, value); }

#define P_NO_GETTER Lisple::no_getter
#define P_NO_SETTER Lisple::no_setter

#define P_GET_SINGLE(FN) get_##FN
#define P_GET_DUAL(_1, FN) get_##FN

#define P_SET_SINGLE(FN) set_##FN
#define P_SET_DUAL(_1, FN) set_##FN

#define P_GET(AD_CLASS, FN, ...) {std::make_shared<Lisple::Key>(#FN), Lisple::Accessors(P_GETTER(AD_CLASS, __SELECT_MACRO__2(0, ##__VA_ARGS__, P_GET_DUAL, P_GET_SINGLE)(FN, ##__VA_ARGS__)), P_NO_SETTER)}

#define P_GET_SET(AD_CLASS, FN, ...) {\
    #FN, \
    Lisple::Accessors(P_GETTER(AD_CLASS, &AD_CLASS::__SELECT_MACRO__2(0, ##__VA_ARGS__, P_GET_DUAL, P_GET_SINGLE)(FN, ##__VA_ARGS__)), \
                      P_SETTER(AD_CLASS, &AD_CLASS::__SELECT_MACRO__2(0, ##__VA_ARGS__, P_SET_DUAL, P_SET_SINGLE)(FN, ##__VA_ARGS__)))}

#define K_GET(AD_CLASS, KEY, FN) {KEY, Lisple::Accessors(P_GETTER(AD_CLASS, P_GET_SINGLE(FN)), P_NO_SETTER)}
#define K_SET(AD_CLASS, KEY, FN) {KEY, Lisple::Accessors(P_NO_GETTER, P_SETTER(P_SET_SINGLE(AD_CLASS, FN)))}
#define K_GET_SET(AD_CLASS, KEY, FN) {KEY, Lisple::Accessors(P_GETTER(AD_CLASS, P_GET_SINGLE(FN)), \
                                                             P_SETTER(AD_CLASS, P_SET_SINGLE(FN))) }

#define DECL_SHKEY(CONSTNAME) \
  /*! @brief Constant Lisple::Key */ \
  extern const std::shared_ptr<Lisple::Key> CONSTNAME;             \
  constexpr const std::shared_ptr<Lisple::Key>* __##CONSTNAME = &CONSTNAME;

#define SHKEY(CONSTNAME,KEYNAME) \
  /*! @brief Constant Lisple::Key */ \
  const std::shared_ptr<Lisple::Key> CONSTNAME = std::make_shared<Lisple::Key>(KEYNAME);

// Helper to detect if a type has operator==
template <typename T, typename = void>
struct is_comparable : std::false_type {};

template <typename T>
struct is_comparable<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type {};

// Specialization for std::map<K, V> to check if both K and V are comparable
template <typename K, typename V>
struct is_comparable<std::map<K, V>>
    : std::integral_constant<bool, is_comparable<K>::value && is_comparable<V>::value> {};

namespace Lisple
{
  class Accessors;
  class AccessorTable;
  struct AdapterTraits;

  extern const AccessorTable NO_ACCESSORS;

  /*!
   * @brief TypeRef implementation specialized for HostObject adapters
   *
   * This includes both base types and derived types. In order for type testing
   * to work properly, it is recommended that derived types specify the @ref
   * parent_type.
   *
   * Optionally contains a qualified name of a make-function, assisting in
   * coercion from raw lisple data structures to object instances.
   */
  class HostTypeRef : public TypeRef
  {

   public:
    /*!
     * @brief Optional parent type for derived types
     */
    std::optional<const HostTypeRef*> parent_type = std::nullopt;
    /*!
     * @brief Optional reference to make-function
     */
    std::optional<std::string> make_fn = std::nullopt;

    /*!
     * @brief Constructs a type reference with an optional make function name
     */
    HostTypeRef(const std::string& name,
                const std::optional<std::string>& make_fn = std::nullopt);

    /*!
     * @brief Constructs a type reference for a derived type and with an
     * optional make function name.
     */
    HostTypeRef(const std::string& name,
                const HostTypeRef* parent_type,
                const std::optional<std::string>& make_fn = std::nullopt);

    /*!
     * @brief Tests is an object is this type.
     *
     * @return true if the object is of this concrete type or if the type if a
     * base of the concrete type of the object.
     */
    bool is_type_of(const Object& obj) const override;

    /*!
     * @brief Attempts to coerce a Lisple Object of a different type to this
     * this type by using the make function provided by the type.
     */
    CoercionResult coerce(Context& ctx, sptr_sobject& obj) const override;

    /*!
     * @brief Tests if this type is a HostObject type.
     *
     * @return true
     */
    bool is_host_object() const override;
  };

  typedef std::map<std::string, Accessors> acc_map;
  typedef std::map<sptr_sobject, Accessors> key_acc_map;

  /*!
   * @brief A lookup table for and specification of accessor methods and their
   * corresponding keys of a HostObject.
   *
   * Stored as part of @see Lisple::AdapterTraits, or one of its derived types.
   */
  class AccessorTable
  {
   public:
    /*!
     * @brief A set of valid keys for the described HostObject
     */
    std::vector<sptr_sobject> keys;
    /*!
     * @brief Lookup table for accessor implementations by key/name.
     */
    acc_map accessor_map;

    /*!
     * @brief Default constructor providing an empty table for types without
     * properties or types with a custom property resolution mechanism.
     */
    AccessorTable() = default;

    /*!
     * @brief Constructs an AccessorTable instance from a map of accessor
     * specifications.
     */
    AccessorTable(const key_acc_map& accessors);

    /*!
     * @brief Tests if an accessor is provided for a specific key.
     */
    bool has_key(const Object& key) const;

    /*!
     * @brief Looks up the accessor definition for a specific key
     */
    const Accessors& lookup(const Object& key) const;
  };

  /*!
   * @brief Constructs a new AccessorTable from an existing instance and an
   * additional accessor map. Used for defining accessor tables for derived
   * types.
   */
  AccessorTable merge_acc(const AccessorTable& al1, const key_acc_map& kam2);

  /*!
   * @brief Defines the type traits of a HostObject specializing, including
   * its HostTypeRef and AccessorTable.
   */
  struct AdapterTraits
  {
    const HostTypeRef* type_ref;
    AccessorTable accessor_table;

    AdapterTraits(const HostTypeRef* type_ref, const AccessorTable& accessor_table);
    virtual ~AdapterTraits() = default;
  };

  /*!
   * @brief Abstract untyped/non-template base class for Host Object adapters.
   */
  class AbstractHostObject : public Seq
  {
   public:
    AbstractHostObject(Form form);

    bool operator==(const Object& other) const override;

    virtual const std::vector<sptr_sobject> keys() const;

    bool has_key(const Object& key) const override;
    sptr_sobject get_sptr_property(const Object& key) const override;
    void set_property(const Object& key, sptr_sobject& value) override;
    void set_property(Context* ctx, const Object& key, sptr_sobject& value) override;

    const std::string lpar() const override;
    const std::string rpar() const override;

    const key_acc_map& get_accessors() const;

    sptr_sobject_v& get_children() override;
    std::string to_string(int depth=1) const override;

    const HostTypeRef* get_host_type() const;
    virtual const AdapterTraits* get_traits() const = 0;
    const AccessorTable& accessor_table() const;

   protected:
    virtual void sync_children() const = 0;
  };

  /*! @brief Convenience type definition for GETTER function references */
  typedef std::function<sptr_sobject(const AbstractHostObject*)> acc_get_t;
  /*! @brief Convenience type definition for SETTER function references */
  typedef std::function<void(AbstractHostObject*, Context*, Object&)> acc_set_t;

  /*!
   * @brief Stock getter-implementation for non-gettable properties that will
   * always return NIL
   */
  extern const acc_get_t no_getter;
  /*!
   * @brief Stock setter-implementation for non-settable properties that will
   * always throw InvocationException
   */
  extern const acc_set_t no_setter;

  /*!
   * @brief Holds function references to getters and setters for a specific
   * field. Both must be defined for every field, but they may optionally refer
   * to no_getter / no_setter.
   */
  class Accessors
  {
   public:
    acc_get_t getter;
    acc_set_t setter;

    Accessors(const acc_get_t& getter, const acc_set_t& setter);
  };

  /*!
   * @brief Abstract base class for wrapping the object held by a Host Object
   * Adapter.
   */
  template<class T>
  class ValueHolder
  {
   public:
    virtual ~ValueHolder() = default;

    virtual T& get_object() = 0;
    virtual std::unique_ptr<T>& get_object_ptr() = 0;
  };

  /*!
   * @brief ValueHolder-implementation that owns the Host Object instance,
   * containing it in a std::unique_ptr
   */
  template<class T>
  class HostObjectValue : public ValueHolder<T>
  {
    std::unique_ptr<T> object;

  public:
    HostObjectValue(std::unique_ptr<T>& object) : object(std::move(object)) {}
    T& get_object() override { return *object; }
    std::unique_ptr<T>& get_object_ptr() override { return object; }
  };

  /*!
   * @brief ValueHolder-implementation that holds a reference to the Host Object
   * for cases where ownership cannot be given to the Adapter.
   */
  template<class T>
  class HostObjectRef : public ValueHolder<T>
  {
    T& object_reference;

   public:
    HostObjectRef(T& ref) : object_reference(ref) {}
    T& get_object() override { return object_reference; }
    std::unique_ptr<T>& get_object_ptr() override
    {
      throw InvocationException("Cannot give up ownership of Object Reference");
    }
  };

  /*!
   * Template base class for Host Object Adapters. Concrete adapters will
   * typically be derived directly from this class, using the HOST_ADAPTER and
   * HOST_ADAPTER_IMPL macros for convenience.
   */
  template<class T>
  class HostObject : public AbstractHostObject
  {
   protected:
    std::unique_ptr<ValueHolder<T>> object;

   public:
    /*!
     * @brief Creates a HostObject instance holding a UNIQUE POINTER to a host
     * object.
     *
     * This is to be used when the HostObject will own the underlying object
     * instance.
     *
     * @param type The type of the Object
     * @param object The actual object
     * @param accessors Description of setters and getters and how they are
     *        invoked
     */
    HostObject(std::unique_ptr<T>& object)
      : AbstractHostObject(Form::HOST_OBJECT)
      , object(std::make_unique<HostObjectValue<T>>(object))
    {
    }

    /*!
     *  @brief Creates a HostObject instance holding a REFERENCE to a host
     *  object.
     *
     *  This is to be used only when the HostObject adapter cannot "own"
     *  the object itself. To be used with care, as the adapter object may
     *  be kept alive in the Lisple context/engine, while the original
     *  instance is destroyed.
     *
     *  Typical usage is for objects that will be "lent" out to the Lisple
     *  engine for quick calculatons or operations.
     *
     *  @param type The type of the object.
     *  @param object The actual object reference
     *  @param accessors Description of getters and setters, and how they
     *         are invoked
     */
    HostObject(T& object)
      : AbstractHostObject(Form::HOST_OBJECT)
      , object(std::make_unique<HostObjectRef<T>>(object))
    {
    }

    /*!
     *  @brief Creates a HostObject instance holding a REFERENCE to a host
     *  object, with a custom @ref Lisple::Form type.
     *
     *  Use this when HostObject adapter cannot "own" the object itself.
     *
     *  To be used with care, as the adapter object may
     *  be kept alive in the Lisple context/engine, while the original
     *  instance is destroyed.
     *
     *  Typical usage is for objects that will be "lent" out to the Lisple
     *  engine for quick calculatons or operations.
     *
     *  @param type The type of the object.
     *  @param object The actual object reference
     *  @param accessors Description of getters and setters, and how they
     *         are invoked
     */
    HostObject(Form form,
               T& object)
      : AbstractHostObject(form)
      , object(std::make_unique<HostObjectRef<T>>(object))
    {
    }

    T& get_object() const
    {
      return object->get_object();
    }

    template <typename U = T>
    typename std::enable_if<is_comparable<U>::value, bool>::type
    compare_to(const HostObject<T>& other) const
    {
      auto equals = [&]() { return this->get_object() == other.get_object(); };
      return equals();  // Only evaluated if T supports ==
    }

    template <typename U = T>
    typename std::enable_if<!is_comparable<U>::value, bool>::type
    compare_to(const HostObject<T>& other) const
    {
      return this->to_string() == other.to_string();
    }

    bool operator==(const Object& other) const override
    {
      if (other.get_type() != Form::HOST_OBJECT) return false;
      if (other.as<AbstractHostObject>().get_host_type() != this->get_host_type()) return false;

      return compare_to(other.as<HostObject<T>>());
    }

    std::string to_string(int depth=-1) const override
    {
      sptr_sobject_v kvs = const_cast<HostObject<T>&>(*this).get_children();

      return Map(kvs).to_string(depth);
    }

    std::unique_ptr<T>& get_object_ptr()
    {
      return object->get_object_ptr();
    }

   protected:
    void sync_children() const override
    {
      this->children.clear();
      sptr_sobject_v kvs;
      for (auto& k : keys())
      {
        if (*get_sptr_property(*k) != *NIL)
        {
          children.push_back(k);
          children.push_back(get_sptr_property(*k));
        }
      }
    }
  };

}

#endif
