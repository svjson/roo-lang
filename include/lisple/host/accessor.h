
#ifndef LISPLE__HOST__ACCESSOR_H
#define LISPLE__HOST__ACCESSOR_H

#include <lisple/host/transform.h>
#include <lisple/macro_support.h>

// clang-format off

/* NOBJ_PROP_GET
 *
 * Simply defines the method signature of a property getter. Should not be used
 * directly by application code unless the specialized macros, such as any of
 * NOBJ_PROP_GET__FIELD, NOBJ_PROP_GET__METHOD or their variants do not
 * apply for the use case.
 *
 * Usage:
 * NOBJ_PROP_GET(SomeHostAdapter, some_property)
 * {
 *   // actual getter code goes here
 * }
 */
#define NOBJ_PROP_GET(AD_CLASS, PROP_NAME)           \
  Lisple::sptr_rtval AD_CLASS::get_##PROP_NAME() const

/* NOBJ_PROP_SET
 *
 * Simply defines the method signature of a property setter. Should not be used
 * directly by application code unless the specialized macros, such as any of
 * NOBJ_PROP_SET__FIELD, NOBJ_PROP_SET__METHOD or their variants do not
 * apply for the use case.
 *
 * Usage:
 * NOBJ_PROP_SET(SomeHostAdapter, some_property)
 * {
 *   // actual setter code goes here
 * }
 */
#define NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                  \
  void AD_CLASS::set_##PROP_NAME([[maybe_unused]]Lisple::Context* ctx, Lisple::sptr_rtval& value)


/* -----------------------------------------------------------------------
 * Field accessor dispatch helpers
 *
 * Three families, differing only in the default name when no explicit
 * field/method name is supplied as an optional argument:
 *
 *   __NOBJ_FIELD_ACCESSOR_MACROS     - default: PROP_NAME        (fields, same-name)
 *   __NOBJ_FIELD_ACCESSOR_MACROS_GET - default: get_##PROP_NAME  (method getters)
 *   __NOBJ_FIELD_ACCESSOR_MACROS_SET - default: set_##PROP_NAME  (method setters)
 * ----------------------------------------------------------------------- */

/* With an explicit name supplied */
#define __NOBJ_FIELD_ACCESSOR(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, NAME)

/* Defaults */
#define __NOBJ_FIELD_ACCESSOR__SAME_NAME(ACCESSOR_MACRO, AD_CLASS, PROP_NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, PROP_NAME)

#define __NOBJ_FIELD_ACCESSOR__GET_PREFIX(ACCESSOR_MACRO, AD_CLASS, PROP_NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, get_##PROP_NAME)

#define __NOBJ_FIELD_ACCESSOR__SET_PREFIX(ACCESSOR_MACRO, AD_CLASS, PROP_NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, set_##PROP_NAME)

/* Dispatchers */
#define __NOBJ_FIELD_ACCESSOR_MACROS(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ...)     \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                              \
    __NOBJ_FIELD_ACCESSOR, __NOBJ_FIELD_ACCESSOR__SAME_NAME)                       \
  (ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_FIELD_ACCESSOR_MACROS_GET(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                              \
    __NOBJ_FIELD_ACCESSOR, __NOBJ_FIELD_ACCESSOR__GET_PREFIX)                      \
  (ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_FIELD_ACCESSOR_MACROS_SET(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                              \
    __NOBJ_FIELD_ACCESSOR, __NOBJ_FIELD_ACCESSOR__SET_PREFIX)                      \
  (ACCESSOR_MACRO, AD_CLASS, PROP_NAME, ##__VA_ARGS__)


/* -----------------------------------------------------------------------
 * Adapter accessor dispatch helpers
 *
 * Same three families as above, but with a required ADAPTER_TYPE arg
 * threaded through before the optional field/method name.
 * ----------------------------------------------------------------------- */

/* With explicit field/method name */
#define __NOBJ_ADAPTER_FIELD_ACCESSOR(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, NAME) \
  MACRO(AD_CLASS, PROP_NAME, ADAPTER_TYPE, NAME)

/* Defaults */
#define __NOBJ_ADAPTER_FIELD_ACCESSOR__SAME_NAME(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE) \
  MACRO(AD_CLASS, PROP_NAME, ADAPTER_TYPE, PROP_NAME)

#define __NOBJ_ADAPTER_FIELD_ACCESSOR__GET_PREFIX(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE) \
  MACRO(AD_CLASS, PROP_NAME, ADAPTER_TYPE, get_##PROP_NAME)

#define __NOBJ_ADAPTER_FIELD_ACCESSOR__SET_PREFIX(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE) \
  MACRO(AD_CLASS, PROP_NAME, ADAPTER_TYPE, set_##PROP_NAME)

/* Dispatchers */
#define __NOBJ_ADAPTER_ACCESSOR_MACROS(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                                     \
    __NOBJ_ADAPTER_FIELD_ACCESSOR, __NOBJ_ADAPTER_FIELD_ACCESSOR__SAME_NAME)              \
  (MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define __NOBJ_ADAPTER_ACCESSOR_MACROS_GET(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                                     \
    __NOBJ_ADAPTER_FIELD_ACCESSOR, __NOBJ_ADAPTER_FIELD_ACCESSOR__GET_PREFIX)             \
  (MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define __NOBJ_ADAPTER_ACCESSOR_MACROS_SET(MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__,                                                     \
    __NOBJ_ADAPTER_FIELD_ACCESSOR, __NOBJ_ADAPTER_FIELD_ACCESSOR__SET_PREFIX)             \
  (MACRO, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)


/* -----------------------------------------------------------------------
 * Internal implementation macros
 * ----------------------------------------------------------------------- */

/* No-cache getter via member function */
#define __NOBJ_PROP_GET__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD)         \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    return Lisple::rtval_from(get_self_object().OBJ_METHOD());                \
  }

/* No-cache getter via direct field access */
#define __NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)           \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    return Lisple::rtval_from(get_self_object().OBJ_FIELD);                   \
  }

/* Setter via member function - deduces the parameter type from the method pointer */
#define __NOBJ_PROP_SET__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD)         \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    Lisple::invoke_setter(                                               \
      get_self_object(),                                                      \
      &std::decay_t<decltype(get_self_object())>::OBJ_METHOD,                 \
      value);                                                            \
  }

/* Setter via direct field access - deduces the field type via decltype */
#define __NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)           \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    get_self_object().OBJ_FIELD =                                             \
      Lisple::rtval_to<std::decay_t<decltype(get_self_object().OBJ_FIELD)>>(value); \
  }

/* Cached getter via member function */
#define __NOBJ_CACHED_PROP_GET__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD)  \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    auto& obj = get_self_object();                                            \
    if (!Lisple::rtval_matches(vcache_##PROP_NAME.cached, obj.OBJ_METHOD())) \
      vcache_##PROP_NAME.cached = Lisple::rtval_from(obj.OBJ_METHOD());  \
    return vcache_##PROP_NAME.cached;                                    \
  }

/* Cached getter via direct field access */
#define __NOBJ_CACHED_PROP_GET__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)    \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    auto& obj = get_self_object();                                            \
    if (!Lisple::rtval_matches(vcache_##PROP_NAME.cached, obj.OBJ_FIELD)) \
      vcache_##PROP_NAME.cached = Lisple::rtval_from(obj.OBJ_FIELD);    \
    return vcache_##PROP_NAME.cached;                                    \
  }


/* Adapter getter via direct field access */
#define __NOBJ_PROP_GET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD)  \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                   \
  {                                                                                    \
    return ADAPTER_TYPE::make_ref(get_self_object().OBJ_FIELD);                            \
  }

/* Adapter getter via member function */
#define __NOBJ_PROP_GET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                    \
  {                                                                                     \
    return ADAPTER_TYPE::make_ref(get_self_object().OBJ_METHOD());                          \
  }

/* Adapter setter via direct field access */
#define __NOBJ_PROP_SET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD)  \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                   \
  {                                                                                    \
    get_self_object().OBJ_FIELD = value->adapter<ADAPTER_TYPE>().get_self_object();             \
  }

/* Adapter setter via member function */
#define __NOBJ_PROP_SET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                    \
  {                                                                                     \
    get_self_object().OBJ_METHOD(value->adapter<ADAPTER_TYPE>().get_self_object());              \
  }


/* Optional Adapter getter via direct field access */
#define __NOBJ_PROP_GET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD)  \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                      \
  {                                                                                       \
    return get_self_object().OBJ_FIELD                                                    \
      ? ADAPTER_TYPE::make_ref(*get_self_object().OBJ_FIELD)                              \
      : Lisple::Constant::NIL;                                                            \
  }

/* Optional Adapter getter via member function */
#define __NOBJ_PROP_GET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                       \
  {                                                                                        \
    return get_self_object().OBJ_METHOD())                                                 \
      ? ADAPTER_TYPE::make_ref(*get_self_object().OBJ_METHOD())                            \
      : Lisple::Constant::NIL;                                                             \
  }

/* Optional Adapter setter via direct field access */
#define __NOBJ_PROP_SET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD)  \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                      \
  {                                                                                       \
    get_self_object().OBJ_FIELD = value->type == Lisple::RTValue::Type::NIL               \
      ? std::nullopt                                                                      \
      : value->adapter<ADAPTER_TYPE>().get_self_object();                                 \
  }

/* Optional Adapter setter via member function */
#define __NOBJ_PROP_SET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                       \
  {                                                                                        \
    get_self_object().OBJ_METHOD(value->type == Lisple::RTValue::Type::NIL                 \
      ? std::nullopt                                                                       \
      : value->adapter<ADAPTER_TYPE>().get_self_object());                                 \
  }



/* -----------------------------------------------------------------------
 * Public macros
 * ----------------------------------------------------------------------- */

/* NOBJ_PROP_GET__METHOD
 *
 * No-cache getter via member function.  The 3rd argument (method name) is
 * optional; when omitted it defaults to get_##PROP_NAME.
 *
 * Usage:
 *   NOBJ_PROP_GET__METHOD(SomeAdapter, some_prop);
 *   NOBJ_PROP_GET__METHOD(SomeAdapter, some_prop, custom_method);
 */
#define NOBJ_PROP_GET__METHOD(AD_CLASS, PROP_NAME, ...)                  \
  __NOBJ_FIELD_ACCESSOR_MACROS_GET(                                      \
    __NOBJ_PROP_GET__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET__METHOD
 *
 * Setter via member function.  The 3rd argument (method name) is optional;
 * when omitted it defaults to set_##PROP_NAME.
 *
 * Usage:
 *   NOBJ_PROP_SET__METHOD(SomeAdapter, some_prop);
 *   NOBJ_PROP_SET__METHOD(SomeAdapter, some_prop, custom_setter);
 */
#define NOBJ_PROP_SET__METHOD(AD_CLASS, PROP_NAME, ...)                  \
  __NOBJ_FIELD_ACCESSOR_MACROS_SET(                                      \
  __NOBJ_PROP_SET__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET__METHOD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET__METHOD(AD_CLASS, PROP_NAME, ...)               \
  NOBJ_PROP_GET__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)               \
  NOBJ_PROP_SET__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)


/* NOBJ_PROP_GET__FIELD
 *
 * No-cache getter via direct field access.  The 3rd argument (field name) is
 * optional; when omitted it defaults to PROP_NAME.
 *
 * Usage:
 *   NOBJ_PROP_GET__FIELD(SomeAdapter, x);
 *   NOBJ_PROP_GET__FIELD(SomeAdapter, x, other_field);
*/
#define NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, ...)                   \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
  __NOBJ_PROP_GET__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET__FIELD
 *
 * Setter via direct field access.  The 3rd argument (field name) is optional;
 * when omitted it defaults to PROP_NAME.
 *
 * Usage:
 *   NOBJ_PROP_SET__FIELD(SomeAdapter, x);
 *   NOBJ_PROP_SET__FIELD(SomeAdapter, x, other_field);
 */
#define NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, ...)                   \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
  __NOBJ_PROP_SET__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)


/* NOBJ_PROP_GET_SET__FIELD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, ...)               \
  NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)               \
  NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_CACHED_PROP_GET__METHOD - cached getter via member function */
#define NOBJ_CACHED_PROP_GET__METHOD(AD_CLASS, PROP_NAME, ...)           \
  __NOBJ_FIELD_ACCESSOR_MACROS_GET(                                      \
    __NOBJ_CACHED_PROP_GET__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_CACHED_PROP_GET__FIELD - cached getter via direct field access */
#define NOBJ_CACHED_PROP_GET__FIELD(AD_CLASS, PROP_NAME, ...)            \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_CACHED_PROP_GET__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* -----------------------------------------------------------------------
 * Adapter accessor public macros
 *
 * Use these when the property value is itself a NativeObject-wrapped type.
 * Getters wrap via ADAPTER_TYPE::make_ref(); setters unwrap via
 * value->adapter<ADAPTER_TYPE>().get_self_object().
 *
 * The optional last argument overrides the field or method name on the
 * underlying host object.
 *
 * Usage:
 *   NOBJ_PROP_GET_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_SET_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_SET_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_SET_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_SET_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 * ----------------------------------------------------------------------- */

#define NOBJ_PROP_GET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                \
    __NOBJ_PROP_GET_ADAPTER__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)    \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_GET(                                            \
    __NOBJ_PROP_GET_ADAPTER__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_SET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                \
    __NOBJ_PROP_SET_ADAPTER__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_SET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)    \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_SET(                                            \
    __NOBJ_PROP_SET_ADAPTER__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  NOBJ_PROP_GET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__) \
  NOBJ_PROP_SET_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  NOBJ_PROP_GET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__) \
  NOBJ_PROP_SET_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

/* -----------------------------------------------------------------------
 * Optional Adapter accessor public macros
 *
 * Use these when the property value is itself a NativeObject type wrapped
 * in std::optional.
 * Getters wrap via ADAPTER_TYPE::make_ref() if the std::optional has a value,
 * otherwise returns NIL; setters unwrap non-NIL values via
 * value->adapter<ADAPTER_TYPE>().get_self_object() or sets the value to
 * std::nullopt for NIL values.
 *
 * The optional last argument overrides the field or method name on the
 * underlying host object.
 *
 * Usage:
 *   NOBJ_PROP_GET_OPT_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_OPT_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_SET_OPT_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_SET_OPT_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_SET_OPT_ADAPTER__FIELD(OrderAdapter, line, LineAdapter);
 *   NOBJ_PROP_GET_SET_OPT_ADAPTER__METHOD(OrderAdapter, line, LineAdapter);
 * ----------------------------------------------------------------------- */

#define NOBJ_PROP_GET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                \
    __NOBJ_PROP_GET_OPT_ADAPTER__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)    \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_GET(                                            \
    __NOBJ_PROP_GET_OPT_ADAPTER__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_SET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                \
    __NOBJ_PROP_SET_OPT_ADAPTER__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_SET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)    \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_SET(                                            \
    __NOBJ_PROP_SET_OPT_ADAPTER__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  NOBJ_PROP_GET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__) \
  NOBJ_PROP_SET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  NOBJ_PROP_GET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__) \
  NOBJ_PROP_SET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

// clang-format on

#endif /* LISPLE__HOST__ACCESSOR_H */
