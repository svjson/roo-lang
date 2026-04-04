
#ifndef LISPLE__HOST__ACCESSOR_H
#define LISPLE__HOST__ACCESSOR_H

// clang-format off

/* NOBJ_PROP_GET
 *
 * Simply defines the method signature of a property getter. Should not be used
 * directly by application code unless the  specialized macros, such as any of
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
 * directly by application code unless the  specialized macros, such as any of
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


/* Field accessor macros */
#define __NOBJ_FIELD_ACCESSOR(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, LISPLE_FORM, FIELD_NAME)

#define __NOBJ_FIELD_ACCESSOR__SAME_NAME(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM) \
  ACCESSOR_MACRO(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define __NOBJ_FIELD_ACCESSOR_MACROS(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __NOBJ_FIELD_ACCESSOR, __NOBJ_FIELD_ACCESSOR__SAME_NAME)(ACCESSOR_MACRO, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* __NOBJ_PROP_GET__METHOD
 *
 * For internal use only
 */
#define __NOBJ_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
    auto& obj = get_object(); \
    if (nullptr == vcache_##PROP_NAME.cached || !vcache_##PROP_NAME.cached->is_##LISPLE_FORM(obj.OBJ_METHOD())) \
    { \
      vcache_##PROP_NAME.cached = Lisple::RTValue::LISPLE_FORM(obj.OBJ_METHOD()); \
    } \
    return vcache_##PROP_NAME.cached;               \
  }

/* __NOBJ_NOCACHE_PROP_GET__METHOD
 *
 * For internal use only
 */
#define __NOBJ_NOCACHE_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_METHOD) \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
     return Lisple::RTValue::LISPLE_FORM(get_object().OBJ_METHOD()); \
  }


#define __NOBJ_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)     \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
    get_self_object().METHOD(value.as<LISPLE_FORM>().value);                  \
  }

/* __ADAPTER_PROP_GET__FIELD
 *
 * For internal use only.
 */
#define __NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
    auto& obj = get_object(); \
    if (nullptr == vcache_##PROP_NAME.cached || !vcache_##PROP_NAME.cached->is_##LISPLE_FORM(obj.OBJ_FIELD)) \
    { \
      vcache_##PROP_NAME.cached = Lisple::RTValue::LISPLE_FORM(obj.OBJ_FIELD); \
    } \
    return vcache_##PROP_NAME.cached;               \
  }

#define __NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  NOBJ_PROP_SET(AD_CLASS, PROP_NAME)                                          \
  {                                                                           \
    get_self_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;              \
  }

/* __NOBJ_NOCACHE_PROP_GET__FIELD
 *
 * For internal use only.
 */
#define __NOBJ_NOCACHE_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  NOBJ_PROP_GET(AD_CLASS, PROP_NAME)                                            \
  {                                                                             \
    return Lisple::RTValue::LISPLE_FORM(get_object().OBJ_FIELD);                \
  }


/* NOBJ_PROP_GET__METHOD - get value by field
 *
 * Generates a property getter implementation that retrives the property value
 * using a member function. This means that the field in question needs to be
 * public, or otherwise accessible from the adapter class.
 */
#define NOBJ_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_PROP_GET__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define NOBJ_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_PROP_SET__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* NOBJ_NOCACHE_PROP_GET__METHOD - get value by field
 *
 * Generates a property getter implementation that retrives the property value
 * using a member function. This means that the field in question needs to be
 * public, or otherwise accessible from the adapter class.
 */
#define NOBJ_NOCACHE_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_NOCACHE_PROP_GET__METHOD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)


/* NOBJ_PROP_GET__FIELD - get value by field
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
#define NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_PROP_GET__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...) \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_PROP_SET__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define NOBJ_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  NOBJ_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)   \
  NOBJ_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

/* NOBJ_NOCACHE_PROP_GET__FIELD - get value by field
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
#define NOBJ_NOCACHE_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __NOBJ_FIELD_ACCESSOR_MACROS(__NOBJ_NOCACHE_PROP_GET__FIELD, AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

// clang-format on

#endif /* LISPLE__HOST__ACCESSOR_H */
