
#ifndef LISPLE__HOST__ACCESSOR_H
#define LISPLE__HOST__ACCESSOR_H

#include <optional>

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
  Lisple::sptr_val AD_CLASS::get_##PROP_NAME() const

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
  void AD_CLASS::set_##PROP_NAME([[maybe_unused]]Lisple::Context* ctx, Lisple::sptr_val& value)


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
 * Keyword vector-copy accessor dispatch helpers
 *
 * Supports both native-style usage with no extra arguments and legacy-style
 * usage with a value type argument:
 *
 *   NOBJ_PROP_GET_SET_KEYWORD_VECTOR_COPY__FIELD(Adapter, transitions);
 *   NOBJ_PROP_GET_SET_KEYWORD_VECTOR_COPY__FIELD(Adapter, transitions, Lisple::AST::Keyword);
 *   NOBJ_PROP_GET_SET_KEYWORD_VECTOR_COPY__FIELD(Adapter, transitions, Lisple::AST::Keyword, field_name);
 * ----------------------------------------------------------------------- */

#define __NOBJ_KEYWORD_VECTOR_ACCESSOR(MACRO, AD_CLASS, PROP_NAME, VALUE_TYPE, NAME) \
  MACRO(AD_CLASS, PROP_NAME, NAME)

#define __NOBJ_KEYWORD_VECTOR_ACCESSOR__IGNORE_TYPE(MACRO, AD_CLASS, PROP_NAME, VALUE_TYPE) \
  MACRO(AD_CLASS, PROP_NAME, PROP_NAME)

#define __NOBJ_KEYWORD_VECTOR_ACCESSOR__SAME_NAME(MACRO, AD_CLASS, PROP_NAME) \
  MACRO(AD_CLASS, PROP_NAME, PROP_NAME)

#define __NOBJ_KEYWORD_VECTOR_ACCESSOR_MACROS(MACRO, AD_CLASS, PROP_NAME, ...) \
  __SELECT_MACRO__3(0, ##__VA_ARGS__,                                          \
    __NOBJ_KEYWORD_VECTOR_ACCESSOR,                                            \
    __NOBJ_KEYWORD_VECTOR_ACCESSOR__IGNORE_TYPE,                               \
    __NOBJ_KEYWORD_VECTOR_ACCESSOR__SAME_NAME)                                 \
  (MACRO, AD_CLASS, PROP_NAME, ##__VA_ARGS__)


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

/* Keyword getter via member function */
#define __NOBJ_PROP_GET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    return Lisple::Value::keyword(get_self_object().OBJ_METHOD());     \
  }

/* Keyword getter via direct field access */
#define __NOBJ_PROP_GET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)   \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    return Lisple::Value::keyword(get_self_object().OBJ_FIELD);        \
  }

/* Keyword setter via member function */
#define __NOBJ_PROP_SET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    get_self_object().OBJ_METHOD(value->str());                          \
  }

/* Keyword setter via direct field access */
#define __NOBJ_PROP_SET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)   \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    get_self_object().OBJ_FIELD = value->str();                          \
  }

/* Optional keyword getter via member function */
#define __NOBJ_PROP_GET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                         \
  {                                                                          \
    auto maybe_value = get_self_object().OBJ_METHOD();                       \
    return maybe_value                                                       \
      ? Lisple::Value::keyword(*maybe_value)                               \
      : Lisple::Constant::NIL;                                               \
  }

/* Optional keyword getter via direct field access */
#define __NOBJ_PROP_GET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                       \
  {                                                                        \
    return get_self_object().OBJ_FIELD.has_value()                         \
      ? Lisple::Value::keyword(*get_self_object().OBJ_FIELD)             \
      : Lisple::Constant::NIL;                                             \
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

/* Optional getter via direct field access */
#define __NOBJ_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)       \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    return get_self_object().OBJ_FIELD.has_value()                       \
      ? Lisple::rtval_from(*get_self_object().OBJ_FIELD)                 \
      : Lisple::Constant::NIL;                                           \
  }

/* Optional setter via direct field access */
#define __NOBJ_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD)       \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                     \
  {                                                                      \
    if (value->type == Lisple::Value::Type::NIL)                       \
      get_self_object().OBJ_FIELD = std::nullopt;                        \
    else                                                                 \
      get_self_object().OBJ_FIELD =                                      \
        Lisple::rtval_to<typename std::decay_t<decltype(get_self_object().OBJ_FIELD)>::value_type>(value); \
  }

/* Optional keyword setter via direct field access */
#define __NOBJ_PROP_SET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                       \
  {                                                                        \
    if (value->type == Lisple::Value::Type::NIL)                         \
      get_self_object().OBJ_FIELD = std::nullopt;                          \
    else                                                                   \
      get_self_object().OBJ_FIELD =                                        \
        Lisple::rtval_to<typename std::decay_t<decltype(get_self_object().OBJ_FIELD)>::value_type>(value); \
  }

/* Optional keyword setter via member function */
#define __NOBJ_PROP_SET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                         \
  {                                                                          \
    if (value->type == Lisple::Value::Type::NIL)                           \
      get_self_object().OBJ_METHOD(std::nullopt);                            \
    else                                                                     \
      get_self_object().OBJ_METHOD(value->str());                            \
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

/* Pointer adapter getter via direct field access */
#define __NOBJ_PROP_GET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                   \
  {                                                                                    \
    return get_self_object().OBJ_FIELD                                                  \
      ? ADAPTER_TYPE::make_ref(*get_self_object().OBJ_FIELD)                            \
      : Lisple::Constant::NIL;                                                          \
  }

/* Pointer adapter setter via direct field access */
#define __NOBJ_PROP_SET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                   \
  {                                                                                    \
    get_self_object().OBJ_FIELD = value->type == Lisple::Value::Type::NIL             \
      ? nullptr                                                                         \
      : &value->adapter<ADAPTER_TYPE>().get_self_object();                              \
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
    auto maybe_value = get_self_object().OBJ_METHOD();                                      \
    return maybe_value                                                                     \
      ? ADAPTER_TYPE::make_ref(*maybe_value)                                               \
      : Lisple::Constant::NIL;                                                             \
  }

/* Optional Adapter setter via direct field access */
#define __NOBJ_PROP_SET_OPT_ADAPTER__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD)  \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                      \
  {                                                                                       \
    if (value->type == Lisple::Value::Type::NIL)                                        \
      get_self_object().OBJ_FIELD = std::nullopt;                                         \
    else                                                                                  \
      get_self_object().OBJ_FIELD = value->adapter<ADAPTER_TYPE>().get_self_object();     \
  }

/* Optional Adapter setter via member function */
#define __NOBJ_PROP_SET_OPT_ADAPTER__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                       \
  {                                                                                        \
    if (value->type == Lisple::Value::Type::NIL)                                         \
      get_self_object().OBJ_METHOD(std::nullopt);                                          \
    else                                                                                   \
      get_self_object().OBJ_METHOD(value->adapter<ADAPTER_TYPE>().get_self_object());      \
  }

/* Value vector copy getter via direct field access */
#define __NOBJ_PROP_GET_VALUE_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                             \
  {                                                                              \
    Lisple::sptr_val_v values;                                                 \
    values.reserve(get_self_object().OBJ_FIELD.size());                          \
    for (auto& obj : get_self_object().OBJ_FIELD)                                \
    {                                                                            \
      values.push_back(Lisple::rtval_from(obj));                                 \
    }                                                                            \
    return Lisple::Value::vector(values);                                      \
  }

/* Value vector copy getter via member function */
#define __NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                \
  {                                                                                 \
    auto& source = get_self_object().OBJ_METHOD();                                  \
    Lisple::sptr_val_v values;                                                    \
    values.reserve(source.size());                                                  \
    for (auto& obj : source)                                                        \
    {                                                                               \
      values.push_back(Lisple::rtval_from(obj));                                    \
    }                                                                               \
    return Lisple::Value::vector(values);                                         \
  }

/* Keyword vector copy getter via direct field access */
#define __NOBJ_PROP_GET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                               \
  {                                                                                \
    Lisple::sptr_val_v values;                                                   \
    values.reserve(get_self_object().OBJ_FIELD.size());                            \
    for (auto& obj : get_self_object().OBJ_FIELD)                                  \
    {                                                                              \
      values.push_back(Lisple::Value::keyword(obj));                             \
    }                                                                              \
    return Lisple::Value::vector(values);                                        \
  }

/* Keyword vector copy setter via direct field access */
#define __NOBJ_PROP_SET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                               \
  {                                                                                \
    auto& target = get_self_object().OBJ_FIELD;                                    \
    target.clear();                                                                \
    if (value->type == Lisple::Value::Type::NIL) return;                         \
    for (auto& obj : value->elements())                                            \
    {                                                                              \
      target.push_back(obj->str());                                                \
    }                                                                              \
  }

/* Adapter vector copy getter via direct field access */
#define __NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                             \
  {                                                                                              \
    Lisple::sptr_val_v values;                                                                 \
    values.reserve(get_self_object().OBJ_FIELD.size());                                          \
    for (auto& obj : get_self_object().OBJ_FIELD)                                                \
    {                                                                                            \
      values.push_back(ADAPTER_TYPE::make_ref(obj));                                             \
    }                                                                                            \
    return Lisple::Value::vector(values);                                                      \
  }

/* Adapter vector copy getter via member function */
#define __NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                               \
  {                                                                                                \
    auto& source = get_self_object().OBJ_METHOD();                                                 \
    Lisple::sptr_val_v values;                                                                   \
    values.reserve(source.size());                                                                 \
    for (auto& obj : source)                                                                       \
    {                                                                                              \
      values.push_back(ADAPTER_TYPE::make_ref(obj));                                               \
    }                                                                                              \
    return Lisple::Value::vector(values);                                                        \
  }

/* Pointer adapter vector copy getter via direct field access */
#define __NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_FIELD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                               \
  {                                                                                                \
    Lisple::sptr_val_v values;                                                                   \
    values.reserve(get_self_object().OBJ_FIELD.size());                                            \
    for (auto& obj : get_self_object().OBJ_FIELD)                                                   \
    {                                                                                              \
      values.push_back(obj ? ADAPTER_TYPE::make_ref(*obj) : Lisple::Constant::NIL);                \
    }                                                                                              \
    return Lisple::Value::vector(values);                                                        \
  }

/* Pointer adapter vector copy getter via member function */
#define __NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                                 \
  {                                                                                                  \
    auto& source = get_self_object().OBJ_METHOD();                                                   \
    Lisple::sptr_val_v values;                                                                     \
    values.reserve(source.size());                                                                   \
    for (auto& obj : source)                                                                          \
    {                                                                                                \
      values.push_back(obj ? ADAPTER_TYPE::make_ref(*obj) : Lisple::Constant::NIL);                  \
    }                                                                                                \
    return Lisple::Value::vector(values);                                                          \
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

/* NOBJ_PROP_GET_KEYWORD__METHOD - get string-like method result as keyword */
#define NOBJ_PROP_GET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)          \
  __NOBJ_FIELD_ACCESSOR_MACROS_GET(                                      \
    __NOBJ_PROP_GET_KEYWORD__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_KEYWORD__FIELD - get string-like field as keyword */
#define NOBJ_PROP_GET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)           \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_GET_KEYWORD__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET_KEYWORD__METHOD - set string-like method from keyword/string */
#define NOBJ_PROP_SET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)          \
  __NOBJ_FIELD_ACCESSOR_MACROS_SET(                                      \
    __NOBJ_PROP_SET_KEYWORD__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET_KEYWORD__FIELD - set string-like field from keyword/string */
#define NOBJ_PROP_SET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)           \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_SET_KEYWORD__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET_KEYWORD__METHOD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)      \
  NOBJ_PROP_GET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)      \
  NOBJ_PROP_SET_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET_KEYWORD__FIELD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)       \
  NOBJ_PROP_GET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)       \
  NOBJ_PROP_SET_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_OPT_KEYWORD__METHOD - get optional string-like method result as keyword or nil */
#define NOBJ_PROP_GET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)      \
  __NOBJ_FIELD_ACCESSOR_MACROS_GET(                                      \
    __NOBJ_PROP_GET_OPT_KEYWORD__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_OPT_KEYWORD__FIELD - get optional string-like field as keyword or nil */
#define NOBJ_PROP_GET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)       \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_GET_OPT_KEYWORD__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET_OPT_KEYWORD__FIELD - set optional string-like field from keyword/string or nil */
#define NOBJ_PROP_SET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)       \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_SET_OPT_KEYWORD__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET_OPT_KEYWORD__METHOD - set optional string-like method from keyword/string or nil */
#define NOBJ_PROP_SET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)      \
  __NOBJ_FIELD_ACCESSOR_MACROS_SET(                                      \
    __NOBJ_PROP_SET_OPT_KEYWORD__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET_OPT_KEYWORD__FIELD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ...)   \
  NOBJ_PROP_GET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)   \
  NOBJ_PROP_SET_OPT_KEYWORD__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET_OPT_KEYWORD__METHOD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ...)  \
  NOBJ_PROP_GET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)  \
  NOBJ_PROP_SET_OPT_KEYWORD__METHOD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_OPT__FIELD - get std::optional value by field */
#define NOBJ_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, ...)               \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_GET_OPT__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_SET_OPT__FIELD - set std::optional value by field */
#define NOBJ_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, ...)               \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_SET_OPT__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

/* NOBJ_PROP_GET_SET_OPT__FIELD - convenience: generates both getter and setter */
#define NOBJ_PROP_GET_SET_OPT__FIELD(AD_CLASS, PROP_NAME, ...)           \
  NOBJ_PROP_GET_OPT__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)           \
  NOBJ_PROP_SET_OPT__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

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

/* Pointer adapter field accessors */
#define NOBJ_PROP_GET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                  \
    __NOBJ_PROP_GET_ADAPTER_P__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_SET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...)     \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                  \
    __NOBJ_PROP_SET_ADAPTER_P__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  NOBJ_PROP_GET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__) \
  NOBJ_PROP_SET_ADAPTER_P__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

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

/* -----------------------------------------------------------------------
 * Vector-copy getter public macros
 *
 * These intentionally return a new Value vector every time. They are useful
 * for preserving legacy accessor semantics where mutating the returned vector
 * must not mutate the source native collection. They are not live wrappers.
 *
 * Usage:
 *   NOBJ_PROP_GET_VALUE_VECTOR_COPY__FIELD(OrderAdapter, ids);
 *   NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD(OrderAdapter, ids);
 *   NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__FIELD(OrderAdapter, lines, LineAdapter);
 *   NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD(OrderAdapter, lines, LineAdapter);
 *   NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__FIELD(OrderAdapter, line_ptrs, LineAdapter);
 *   NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD(OrderAdapter, line_ptrs, LineAdapter);
 * ----------------------------------------------------------------------- */

#define NOBJ_PROP_GET_VALUE_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS(                                          \
    __NOBJ_PROP_GET_VALUE_VECTOR_COPY__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS_GET(                                       \
    __NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define NOBJ_PROP_GET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ...) \
  __NOBJ_KEYWORD_VECTOR_ACCESSOR_MACROS(                                   \
    __NOBJ_PROP_GET_KEYWORD_VECTOR_COPY__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define NOBJ_PROP_SET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ...) \
  __NOBJ_KEYWORD_VECTOR_ACCESSOR_MACROS(                                   \
    __NOBJ_PROP_SET_KEYWORD_VECTOR_COPY__FIELD, AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ...) \
  NOBJ_PROP_GET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__) \
  NOBJ_PROP_SET_KEYWORD_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ##__VA_ARGS__)

#define NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                        \
    __NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_GET(                                                     \
    __NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__FIELD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __NOBJ_ADAPTER_ACCESSOR_MACROS(                                                          \
    __NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__FIELD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

#define NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD(AD_CLASS, PROP_NAME, ADAPTER_TYPE, ...) \
  __NOBJ_ADAPTER_ACCESSOR_MACROS_GET(                                                       \
    __NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD, AD_CLASS, PROP_NAME, ADAPTER_TYPE, ##__VA_ARGS__)

/* -----------------------------------------------------------------------
 * Composable adapter accessor DSL
 *
 * These macros are an additive layer over the NOBJ_PROP_* implementation
 * style. The generated code is the same direct C++ accessor code as the
 * specialized macros above; the token DSL only selects which snippet to emit.
 *
 * Usage:
 *   ADAPTER_PROP_GET_SET(OrderAdapter, FIELD(id));
 *   ADAPTER_PROP_GET_SET(OrderAdapter, FIELD(line), ADAPTER(LineAdapter));
 *   ADAPTER_PROP_GET(OrderAdapter, METHOD(lines), VECTOR_COPY(ADAPTER(LineAdapter)));
 * ----------------------------------------------------------------------- */

#define FIELD(PROP_NAME) FIELD, PROP_NAME
#define METHOD(PROP_NAME) METHOD, PROP_NAME
#define ADAPTER(ADAPTER_TYPE) ADAPTER, ADAPTER_TYPE
#define ADAPTER_P(ADAPTER_TYPE) ADAPTER_P, ADAPTER_TYPE
#define VECTOR_COPY(VALUE_SPEC) VECTOR_COPY, VALUE_SPEC

#define ADAPTER_PROP_GET(AD_CLASS, ...)                                  \
  __NOBJ_DSL_PROP_GET(AD_CLASS, __VA_ARGS__)

#define ADAPTER_PROP_SET(AD_CLASS, ...)                                  \
  __NOBJ_DSL_PROP_SET(AD_CLASS, __VA_ARGS__)

#define ADAPTER_PROP_GET_SET(AD_CLASS, ...)                              \
  ADAPTER_PROP_GET(AD_CLASS, __VA_ARGS__)                                \
  ADAPTER_PROP_SET(AD_CLASS, __VA_ARGS__)

#define __NOBJ_DSL_PROP_GET(AD_CLASS, ACCESS_KIND, PROP_NAME, ...)       \
  __SELECT_MACRO__4(0, ##__VA_ARGS__,                                    \
    __NOBJ_DSL_PROP_GET__TYPED,                                          \
    __NOBJ_DSL_PROP_GET__TYPED,                                          \
    __NOBJ_DSL_PROP_GET__TYPED,                                          \
    __NOBJ_DSL_PROP_GET__DEFAULT)                                        \
  (AD_CLASS, ACCESS_KIND, PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_GET__DEFAULT(AD_CLASS, ACCESS_KIND, PROP_NAME)   \
  __NOBJ_DSL_PROP_GET__##ACCESS_KIND(AD_CLASS, PROP_NAME, VALUE)

#define __NOBJ_DSL_PROP_GET__TYPED(AD_CLASS, ACCESS_KIND, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_GET__##ACCESS_KIND(AD_CLASS, PROP_NAME, VALUE_KIND, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_GET__FIELD(AD_CLASS, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_GET_IMPL__##VALUE_KIND(                                \
    AD_CLASS, PROP_NAME, get_self_object().PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_GET__METHOD(AD_CLASS, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_GET_IMPL__##VALUE_KIND(                                 \
    AD_CLASS, PROP_NAME, get_self_object().get_##PROP_NAME(), ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_GET_IMPL__VALUE(AD_CLASS, PROP_NAME, ACCESS_EXPR) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                      \
  {                                                                       \
    return Lisple::rtval_from(ACCESS_EXPR);                               \
  }

#define __NOBJ_DSL_PROP_GET_IMPL__ADAPTER(AD_CLASS, PROP_NAME, ACCESS_EXPR, ADAPTER_TYPE) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                       \
  {                                                                                         \
    auto&& prop_value = ACCESS_EXPR;                                                        \
    return ADAPTER_TYPE::make_ref(prop_value);                                              \
  }

#define __NOBJ_DSL_PROP_GET_IMPL__ADAPTER_P(AD_CLASS, PROP_NAME, ACCESS_EXPR, ADAPTER_TYPE) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                        \
  {                                                                                          \
    auto&& prop_value = ACCESS_EXPR;                                                         \
    return prop_value ? ADAPTER_TYPE::make_ref(*prop_value) : Lisple::Constant::NIL;         \
  }

#define __NOBJ_DSL_PROP_GET_IMPL__VECTOR_COPY(AD_CLASS, PROP_NAME, ACCESS_EXPR, VALUE_KIND, ...) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                                             \
  {                                                                                               \
    auto&& source = ACCESS_EXPR;                                                                  \
    Lisple::sptr_val_v values;                                                                  \
    values.reserve(source.size());                                                                \
    for (auto& obj : source)                                                                      \
    {                                                                                             \
      values.push_back(__NOBJ_DSL_VECTOR_WRAP__##VALUE_KIND(obj, ##__VA_ARGS__));                 \
    }                                                                                             \
    return Lisple::Value::vector(values);                                                       \
  }

#define __NOBJ_DSL_VECTOR_WRAP__VALUE(obj)                         \
  Lisple::rtval_from(obj)

#define __NOBJ_DSL_VECTOR_WRAP__ADAPTER(obj, ADAPTER_TYPE)         \
  ADAPTER_TYPE::make_ref(obj)

#define __NOBJ_DSL_VECTOR_WRAP__ADAPTER_P(obj, ADAPTER_TYPE)       \
  (obj ? ADAPTER_TYPE::make_ref(*obj) : Lisple::Constant::NIL)

#define __NOBJ_DSL_PROP_SET(AD_CLASS, ACCESS_KIND, PROP_NAME, ...)       \
  __SELECT_MACRO__4(0, ##__VA_ARGS__,                                    \
    __NOBJ_DSL_PROP_SET__TYPED,                                          \
    __NOBJ_DSL_PROP_SET__TYPED,                                          \
    __NOBJ_DSL_PROP_SET__TYPED,                                          \
    __NOBJ_DSL_PROP_SET__DEFAULT)                                        \
  (AD_CLASS, ACCESS_KIND, PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_SET__DEFAULT(AD_CLASS, ACCESS_KIND, PROP_NAME)   \
  __NOBJ_DSL_PROP_SET__##ACCESS_KIND(AD_CLASS, PROP_NAME, VALUE)

#define __NOBJ_DSL_PROP_SET__TYPED(AD_CLASS, ACCESS_KIND, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_SET__##ACCESS_KIND(AD_CLASS, PROP_NAME, VALUE_KIND, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_SET__FIELD(AD_CLASS, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_SET_FIELD_IMPL__##VALUE_KIND(AD_CLASS, PROP_NAME, PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_SET__METHOD(AD_CLASS, PROP_NAME, VALUE_KIND, ...) \
  __NOBJ_DSL_PROP_SET_METHOD_IMPL__##VALUE_KIND(AD_CLASS, PROP_NAME, set_##PROP_NAME, ##__VA_ARGS__)

#define __NOBJ_DSL_PROP_SET_FIELD_IMPL__VALUE(AD_CLASS, PROP_NAME, OBJ_FIELD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
    get_self_object().OBJ_FIELD =                                             \
      Lisple::rtval_to<std::decay_t<decltype(get_self_object().OBJ_FIELD)>>(value); \
  }

#define __NOBJ_DSL_PROP_SET_FIELD_IMPL__ADAPTER(AD_CLASS, PROP_NAME, OBJ_FIELD, ADAPTER_TYPE) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                          \
  {                                                                                            \
    get_self_object().OBJ_FIELD = value->adapter<ADAPTER_TYPE>().get_self_object();            \
  }

#define __NOBJ_DSL_PROP_SET_FIELD_IMPL__ADAPTER_P(AD_CLASS, PROP_NAME, OBJ_FIELD, ADAPTER_TYPE) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                            \
  {                                                                                              \
    get_self_object().OBJ_FIELD = value->type == Lisple::Value::Type::NIL                      \
      ? nullptr                                                                                  \
      : &value->adapter<ADAPTER_TYPE>().get_self_object();                                       \
  }

#define __NOBJ_DSL_PROP_SET_METHOD_IMPL__VALUE(AD_CLASS, PROP_NAME, OBJ_METHOD) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                            \
  {                                                                             \
    Lisple::invoke_setter(                                                       \
      get_self_object(),                                                         \
      &std::decay_t<decltype(get_self_object())>::OBJ_METHOD,                    \
      value);                                                                    \
  }

#define __NOBJ_DSL_PROP_SET_METHOD_IMPL__ADAPTER(AD_CLASS, PROP_NAME, OBJ_METHOD, ADAPTER_TYPE) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                            \
  {                                                                                              \
    get_self_object().OBJ_METHOD(value->adapter<ADAPTER_TYPE>().get_self_object());              \
  }

#define __NOBJ_DSL_PROP_SET_METHOD_IMPL__ADAPTER_P(AD_CLASS, PROP_NAME, OBJ_METHOD, ADAPTER_TYPE) \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                                              \
  {                                                                                                \
    get_self_object().OBJ_METHOD(value->type == Lisple::Value::Type::NIL                         \
      ? nullptr                                                                                    \
      : &value->adapter<ADAPTER_TYPE>().get_self_object());                                        \
  }

// clang-format on

#endif /* LISPLE__HOST__ACCESSOR_H */
