
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

#define __SELECT_MACRO__2(_1, _2, MACRO_NAME, ...) MACRO_NAME

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

#define __HOST_TYPE(CONST, NAME, MAKE_FN) \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter with a registered \
   * make function for automatic coercion.                              \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME, MAKE_FN);

#define __HOST_TYPE__NO_MAKE_FN(CONST, NAME) \
  /*!                                                       \
   * @brief Type definition for a Lisple HostObject Adapter \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME);

#define HOST_TYPE(CONST, NAME, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_TYPE, __HOST_TYPE__NO_MAKE_FN)(CONST, NAME, ##__VA_ARGS__)


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

#define HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                                                                \
  class AD_CLASS : public Lisple::HostObject<H_CLASS>                                                            \
  {                                                                                                              \
   public:                                                                                                       \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class and takes   \
    * ownership of the unique pointer.                                  \
    */                                                                  \
    AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access = Lisple::NO_ACCESSORS); \
   /*!                                                                  \
    * @brief Constructs a new instance of the adapter class that holds  \
    * the supplied reference. This effectively means that the adapter   \
    * object is not responsible for the life-cycle of the referenced    \
    * object and may end up with a dangling pointer                     \
    */                                                                  \
    AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access = Lisple::NO_ACCESSORS); \
    HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define SUB_ADAPTER_MAIN_DECL(AD_CLASS, AD_SUP_CLASS, H_CLASS)                                \
  class AD_CLASS : public AD_SUP_CLASS                                                        \
  {                                                                                           \
   public:                                                                                    \
    AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access={});   \
    AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access={});                     \
    HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define HOST_ADAPTER_END_DECL                   \
  }

/* HOST_ADAPTER_GETTER_DECL
 *
 * Declares a single host adapter getter accessor function
 */
#define HOST_ADAPTER_GETTER_DECL(PROP_NAME)             \
  /*! @brief Auto-generated getter */ \
  Lisple::sptr_sobject get_##PROP_NAME() const;

/* __HOST_ADAPTER_GETTERS
 *
 * Wrapper around __VA_MACRO to declare a variadic number of getters
 */
#define __HOST_ADAPTER_GETTERS(...) __VA_MACRO(HOST_ADAPTER_GETTER_DECL, __VA_ARGS__)

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
#define __HOST_ADAPTER_SETTERS(...) __VA_MACRO(HOST_ADAPTER_SETTER_DECL, __VA_ARGS__)

#define SUB_ADAPTER__NO_PROPS(AD_CLASS, SUP_AD_CLASS, H_CLASS)  \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)        \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_GETTERS(AD_CLASS, SUP_AD_CLASS, H_CLASS, GET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)                \
  __HOST_ADAPTER_GETTERS GET_PROPS                                        \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_PROPS(AD_CLASS, SUP_AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)                \
  __HOST_ADAPTER_GETTERS GET_PROPS                                        \
  __HOST_ADAPTER_SETTERS SET_PROPS                                        \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__NO_PROPS(AD_CLASS, H_CLASS)    \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)          \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_GETTERS(AD_CLASS, H_CLASS, GET_PROPS) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                      \
  __HOST_ADAPTER_GETTERS GET_PROPS                                 \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_PROPS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                               \
  __HOST_ADAPTER_GETTERS GET_PROPS                                          \
  __HOST_ADAPTER_SETTERS SET_PROPS                                        \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS, ...) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                               \
  __HOST_ADAPTER_GETTERS GET_PROPS                                          \
  __HOST_ADAPTER_SETTERS SET_PROPS                                        \
  __ESCAPE __VA_ARGS__                         \
  HOST_ADAPTER_END_DECL

#define SELECT_ADAPTER_MACRO(_1, _2, _3, _4, MACRO_NAME, ...) MACRO_NAME
#define SELECT_SUB_ADAPTER_MACRO(_1, _2, _3,  MACRO_NAME, ...) MACRO_NAME
#define HOST_SUB_ADAPTER(AD_CLASS, SUP_AD_CLASS, H_CLASS, ...)                                                     \
  SELECT_SUB_ADAPTER_MACRO(0, ##__VA_ARGS__, SUB_ADAPTER__WITH_PROPS, SUB_ADAPTER__WITH_GETTERS, SUB_ADAPTER__NO_PROPS) \
  (AD_CLASS, SUP_AD_CLASS, H_CLASS, ##__VA_ARGS__)
#define HOST_ADAPTER(AD_CLASS, H_CLASS, ...)                            \
  SELECT_ADAPTER_MACRO(0, ##__VA_ARGS__, HOST_ADAPTER__WITH_PROPS_AND_CUSTOM_FIELDS, HOST_ADAPTER__WITH_PROPS, HOST_ADAPTER__WITH_GETTERS, HOST_ADAPTER__NO_PROPS) \
  (AD_CLASS, H_CLASS, ##__VA_ARGS__)

#define ESC(...) __VA_ARGS__

#define HOST_SUB_ADAPTER_IMPL__NO_ACCESSORS(SUP_AD_CLASS, AD_CLASS, H_CLASS)            \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
    : SUP_AD_CLASS(std::move(obj_ptr), _access) {}                                      \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access)          \
    : SUP_AD_CLASS(obj_ref, _access) {}

#define HOST_SUB_ADAPTER_IMPL__ACCESSORS(SUP_AD_CLASS, AD_CLASS, H_CLASS, ACCESSOR_MAP)   \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
    : SUP_AD_CLASS(std::move(obj_ptr), Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {} \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access)     \
    : SUP_AD_CLASS(obj_ref, Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {}

#define HOST_ADAPTER_IMPL__NO_ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T)            \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
    : HostObject(HOBJ_T, obj_ptr, _access) { }        \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access) \
    : HostObject(HOBJ_T, obj_ref, _access) { }

#define HOST_ADAPTER_IMPL__ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T, ACCESSOR_MAP)   \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
  : HostObject(HOBJ_T, obj_ptr, Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {} \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access)          \
    : HostObject(HOBJ_T, obj_ref, Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {}

#define SELECT_SUB_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME
#define SELECT_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define HOST_SUB_ADAPTER_IMPL(SUP_AD_CLASS, AD_CLASS, H_CLASS, ...) SELECT_SUB_ADAPTER_IMPL_MACRO(0, ##__VA_ARGS__, HOST_SUB_ADAPTER_IMPL__ACCESSORS, HOST_SUB_ADAPTER_IMPL__NO_ACCESSORS)(SUP_AD_CLASS, AD_CLASS, H_CLASS, ##__VA_ARGS__)

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
#define __ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                           \
  {                                                                               \
    return std::make_shared<LISPLE_FORM>(get_object().OBJ_FIELD);                 \
  }

#define __ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    get_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;                      \
  }

/* __ADAPTER_PROP_GET_P__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                             \
  {                                                                                 \
    return std::make_shared<LISPLE_FORM>(*get_object().OBJ_FIELD);                  \
  }

/* __ADAPTER_PROP_GET_OPT__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    return get_object().OBJ_FIELD.has_value()                                      \
      ? std::make_shared<LISPLE_FORM>(*get_object().OBJ_FIELD)                     \
      : Lisple::NIL;                                                               \
  }

#define __ADAPTER_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    if (*Lisple::NIL == value)                                                     \
      get_object().OBJ_FIELD = std::nullopt;                                       \
    else                                                                           \
      get_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;                      \
  }

/* __ADAPTER_PROP_GET_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    Lisple::sptr_sobject_v v;                                                                                \
    for (auto& obj : get_object().FIELD_NAME)                                                                \
    {                                                                                                        \
      v.push_back(std::make_shared<LISPLE_FORM>(obj));                                                       \
    }                                                                                                        \
    return std::make_shared<Lisple::Array>(v);                                                               \
  }

/* __ADAPTER_PROP_SET_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    get_object().FIELD_NAME.clear();                                                                         \
    for (auto& obj : value.get_children())                                                                   \
    {                                                                                                        \
      get_object().FIELD_NAME.push_back(obj->as<LISPLE_FORM>().value);                                       \
    }                                                                                                        \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)                       \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                                      \
  {                                                                                                          \
    Lisple::sptr_sobject_v v;                                                                                \
    for (auto& obj : get_object().FIELD_NAME)                                                                \
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
    get_object().FIELD_NAME.clear();                                                                         \
    for (auto& obj : value.get_children())                                                                   \
    {                                                                                                        \
      get_object().FIELD_NAME.push_back(obj->as<LISPLE_FORM>().get_object());                                \
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
     for (auto obj : get_object().FIELD_NAME)                                            \
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
    return LISPLE_FORM::make_ref(get_object().OBJ_FIELD);                                   \
  }

#define __ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                     \
  {                                                                                         \
    get_object().OBJ_FIELD = value.as<LISPLE_FORM>().get_object();                          \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)      \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                           \
  {                                                                                               \
    return get_object().OBJ_FIELD ? LISPLE_FORM::make_ref(*get_object().OBJ_FIELD) : Lisple::NIL; \
  }

#define __ADAPTER_PROP_SET_HOST_OBJECT_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)      \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                           \
  {                                                                                               \
    get_object().OBJ_FIELD = *Lisple::NIL == value                                                \
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
    return get_object().OBJ_FIELD.has_value()                                                    \
      ? LISPLE_FORM::make_ref(*get_object().OBJ_FIELD)                                           \
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
      get_object().OBJ_FIELD = std::nullopt;                                                     \
    else                                                                                         \
      get_object().OBJ_FIELD = value.as<LISPLE_FORM>().get_object();                             \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    return get_object().OBJ_FIELD.count(MAP_KEY)                                                 \
      ? LISPLE_FORM::make_ref(*get_object().OBJ_FIELD.at(MAP_KEY))                               \
      : Lisple::NIL;                                                                             \
  }

/* __ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT_OPT__MAP_FIELD(AD_CLASS, PROP_NAME, MAP_KEY, LISPLE_FORM, OBJ_FIELD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                              \
    if (get_object().OBJ_FIELD.count(MAP_KEY))                                                   \
    {                                                                                            \
      get_object().OBJ_FIELD.erase(MAP_KEY);                                                     \
    }                                                                                            \
                                                                                                 \
    if (*Lisple::NIL != value)                                                                   \
    {                                                                                            \
      get_object().OBJ_FIELD.emplace(MAP_KEY, value.as<LISPLE_FORM>().get_object());             \
    }                                                                                            \
  }

/* __ADAPTER_PROP_GET__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_METHOD)   \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                            \
  {                                                                                \
    return std::make_shared<LISPLE_FORM>(get_object().OBJ_METHOD());               \
  }

#define __ADAPTER_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)     \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                              \
    get_object().METHOD(value.as<LISPLE_FORM>().value);                          \
  }

/* __ADAPTER_PROP_GET_P__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                           \
  {                                                                               \
    return std::make_shared<LISPLE_FORM>(*get_object().METHOD());                 \
  }

/* __ADAPTER_PROP_GET_VECTOR__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_VECTOR__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD_NAME) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    Lisple::sptr_sobject_v v;                                                            \
    for (auto& obj : get_object().METHOD_NAME())                                         \
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
    for (auto& obj : get_object().METHOD_NAME())                                           \
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
    return LISPLE_FORM::make_ref(get_object().METHOD());                                 \
  }

/* __ADAPTER_PROP_GET_HOST_OBJECT__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_SET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                  \
  {                                                                                      \
    return get_object().METHOD(value.as<LISPLE_FORM>().get_object());                    \
  }


/* __ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD
 *
 * For internal use only
 */
#define __ADAPTER_PROP_GET_HOST_OBJECT_OPT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                      \
  {                                                                                          \
    return get_object().METHOD().has_value()                                                 \
      ? LISPLE_FORM::make_ref(*get_object().METHOD())                                        \
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
  extern const std::shared_ptr<Lisple::Key> CONSTNAME;
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
  class AccessorLookup;
  class Accessors;
  extern const AccessorLookup NO_ACCESSORS;

  /*!
   * @brief TypeRef implementation specialized for HostObject adapters
   *
   * Optionally contains a qualified name of a make-function, assisting in
   * coercion from raw lisple data structures to object instances.
   */
  class HostTypeRef : public TypeRef
  {
    /*!
     * @brief Optional reference to make-function
     */
    std::optional<std::string> make_fn = std::nullopt;

   public:
    HostTypeRef(const std::string& name, const std::optional<std::string>& make_fn = std::nullopt);

    bool is_type_of(const Object& obj) const override;
    CoercionResult coerce(Context& ctx, sptr_sobject& obj) const override;
    bool is_host_object() const override;
  };

  typedef std::map<std::string, Accessors> acc_map;
  typedef std::map<sptr_sobject, Accessors> key_acc_map;

  /**
   *
   */
  class AccessorLookup
  {
   public:
    std::vector<sptr_sobject> keys;
    acc_map accessor_map;

    AccessorLookup();
    AccessorLookup(const key_acc_map& accessors);

    bool has_key(const Object& key) const;
    const Accessors& lookup(const Object& key) const;
  };

  AccessorLookup merge_acc(const AccessorLookup& al1, const key_acc_map& kam2);

  /*!
   * @brief Abstract untyped/non-template base class for Host Object adapters.
   */
  class AbstractHostObject : public Object
  {
    const HostTypeRef* host_type;
    AccessorLookup accessors;

   public:
    AbstractHostObject(const HostTypeRef* type);
    AbstractHostObject(const HostTypeRef* type, const AccessorLookup& accessors);

    bool operator==(const Object& other) const override;
    virtual std::string to_string(int depth=-1) const override;

    const HostTypeRef* get_host_type() const;

    virtual const std::vector<sptr_sobject> keys() const;

    bool has_key(const Object& key) const override;
    sptr_sobject get_sptr_property(const Object& key) const override;
    void set_property(const Object& key, sptr_sobject& value) override;
    void set_property(Context* ctx, const Object& key, sptr_sobject& value) override;

    const key_acc_map& get_accessors() const;
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
    const std::string type_name;
    std::unique_ptr<ValueHolder<T>> object;
    /*! @brief Internally cached list of Lisple/HostObject children */
    sptr_sobject_v __cached_children;

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
    HostObject(const HostTypeRef* type, std::unique_ptr<T>& object, const AccessorLookup& accessors = {})
      : AbstractHostObject(type, accessors)
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
    HostObject(const HostTypeRef* type, T& object, const AccessorLookup& accessors = {})
      : AbstractHostObject(type, accessors)
      , object(std::make_unique<HostObjectRef<T>>(object))
    {
    }

    T& get_object() const
    {
      return object->get_object();
    }

    /*!
     * @brief Returns the properties of the HostObject with :key and value
     * interspersed, as if a Map.
     *
     * Because Object::get_children() assumes  ownership of the child vector we
     * return by reference, which is why the __cached_children is needed.
     *
     * This could give rise to all sorts of problems down the road, and
     * we should probably just own up to the fact that temporary copies
     * of shared_ptr and vector aren't that expensive and change the
     * signature to always return the child array by copy.
     */
    sptr_sobject_v& get_children() override
    {
      sptr_sobject_v kvs;
      for (auto& k : keys())
      {
        if (*get_sptr_property(*k) != *NIL)
        {
          kvs.push_back(k);
          kvs.push_back(get_sptr_property(*k));
        }
      }
      __cached_children = kvs;
      return __cached_children;
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
  };

}

#endif
