
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
   const Lisple::NativeObjectTraits* get_traits() const override;       \


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
   * @brief Static factory method that wraps an existing shared pointed \
   * underlying object and wraps it in an adapter instance              \
   */                                                                   \
  static Lisple::sptr_rtval claim(std::unique_ptr<H_CLASS>&& uptr)      \
  {                                                                     \
    return Lisple::RTValue::native_object(                              \
      std::make_shared<AD_CLASS>(std::move(uptr))                       \
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
    * @brief Constructs a new instance of the adapter class that holds  \
    * the supplied reference. This effectively means that the adapter   \
    * object is not responsible for the life-cycle of the referenced    \
    * object and may end up with a dangling pointer                     \
    */                                                                  \
   AD_CLASS(H_CLASS& obj_ref);                                          \
   H_CLASS& get_object() const override;                                \
   H_CLASS& get_self_object() const;                                    \
   void* self_object_ptr() const override;                              \
   static const Lisple::NAccessorTable& static_accessor_table();        \
   NATIVE_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define NATIVE_ADAPTER_END_DECL                   \
  }

/* NATIVE_ADAPTER_GETTER_DECL
 *
 * Declares a single host adapter getter accessor function
 */
#define NATIVE_ADAPTER_GETTER_DECL(PROP_NAME)             \
  /*! @brief Auto-generated getter */                   \
  mutable Lisple::NCacheSlot vcache_##PROP_NAME; \
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


/** NATIVE_SUB_ADAPTER_MAIN_DECL__IMPL
 *
 * Inner generator: AD_CLASS and H_CLASS arrive as already-expanded separate
 * arguments. Never call this directly - use NATIVE_SUB_ADAPTER_MAIN_DECL.
 */
#define NATIVE_SUB_ADAPTER_MAIN_DECL__IMPL(AD_CLASS, H_CLASS, AD_SUP_CLASS)    \
  class AD_CLASS : public AD_SUP_CLASS                                          \
  {                                                                             \
  __NOBJ_TRAITS_DECL                                                            \
  public:                                                                       \
   /*!                                                                          \
    * @brief Constructs a new instance wrapping a newly created H_CLASS.       \
    */                                                                          \
   AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr);                                \
   /*!                                                                          \
    * @brief Constructs a new instance holding a reference to an existing      \
    * H_CLASS object. The adapter does not own the referenced object.          \
    */                                                                          \
   AD_CLASS(H_CLASS& obj_ref);                                                  \
   H_CLASS& get_self_object() const;                                            \
   void* self_object_ptr() const override;                                      \
   static const Lisple::NAccessorTable& static_accessor_table();                \
   NATIVE_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

/** __NATIVE_SUB_ADAPTER_MAIN_DECL_DISPATCH
 *
 * AD_SUP_CLASS is the first named parameter; the variadic receives ESC
 * SUB_PAIR as one collected arg. When __VA_ARGS__ is substituted in the
 * body, ESC expands the pair into two separate tokens (AD_CLASS, H_CLASS).
 */
#define __NATIVE_SUB_ADAPTER_MAIN_DECL_DISPATCH(AD_SUP_CLASS, ...)             \
  NATIVE_SUB_ADAPTER_MAIN_DECL__IMPL(__VA_ARGS__, AD_SUP_CLASS)

/** NATIVE_SUB_ADAPTER_MAIN_DECL
 *
 * Entry point. SUB_PAIR stays as one argument until ESC expands it inside
 * the dispatch helper's variadic substitution.
 */
#define NATIVE_SUB_ADAPTER_MAIN_DECL(SUB_PAIR, AD_SUP_CLASS)                   \
  __NATIVE_SUB_ADAPTER_MAIN_DECL_DISPATCH(AD_SUP_CLASS, ESC SUB_PAIR)


/**
 * NATIVE_SUB_ADAPTER declaration variants
 *
 * SUB_PAIR is forwarded as-is; NATIVE_SUB_ADAPTER_MAIN_DECL handles the
 * ESC expansion internally.
 */

#define NATIVE_SUB_ADAPTER__NO_PROPS(SUB_PAIR, AD_SUP_CLASS)                   \
  NATIVE_SUB_ADAPTER_MAIN_DECL(SUB_PAIR, AD_SUP_CLASS)                         \
  NATIVE_ADAPTER_END_DECL

#define NATIVE_SUB_ADAPTER__WITH_GETTERS(SUB_PAIR, AD_SUP_CLASS, GET_PROPS)    \
  NATIVE_SUB_ADAPTER_MAIN_DECL(SUB_PAIR, AD_SUP_CLASS)                         \
  __NATIVE_ADAPTER_GETTERS GET_PROPS                                            \
  NATIVE_ADAPTER_END_DECL

#define NATIVE_SUB_ADAPTER__WITH_PROPS(SUB_PAIR, AD_SUP_CLASS, GET_PROPS, SET_PROPS) \
  NATIVE_SUB_ADAPTER_MAIN_DECL(SUB_PAIR, AD_SUP_CLASS)                         \
  __NATIVE_ADAPTER_GETTERS GET_PROPS                                            \
  __NATIVE_ADAPTER_SETTERS SET_PROPS                                            \
  NATIVE_ADAPTER_END_DECL

/**
 * NATIVE_SUB_ADAPTER main declaration macro
 *
 * SUB_PAIR is a parenthesised (AdapterClass, HostClass) pair grouping the
 * sub-adapter with its host type.
 *
 * Usage (inherits base props, no own props):
 *   NATIVE_SUB_ADAPTER(BaseAdapter, (SubAdapter, SubClass));
 *
 * Usage (with own getter props):
 *   NATIVE_SUB_ADAPTER(BaseAdapter, (SubAdapter, SubClass), (prop1, prop2));
 *
 * Usage (with own getter and setter props):
 *   NATIVE_SUB_ADAPTER(BaseAdapter, (SubAdapter, SubClass), (prop1), (prop1));
 */
#define NATIVE_SUB_ADAPTER(AD_SUP_CLASS, SUB_PAIR, ...)                         \
  __SELECT_MACRO__3(0, ##__VA_ARGS__,                                            \
    NATIVE_SUB_ADAPTER__WITH_PROPS, NATIVE_SUB_ADAPTER__WITH_GETTERS,           \
    NATIVE_SUB_ADAPTER__NO_PROPS)(SUB_PAIR, AD_SUP_CLASS, ##__VA_ARGS__)


/* __NOBJ_TRAITS_IMPL
 *
 * Generates static_accessor_table() and get_traits() for a base adapter.
 * ACCESSOR_MAP is a parenthesised constructor argument for NAccessorTable,
 * e.g. (Lisple::NO_N_ACCESSORS) or ({entry, entry, ...}).
 */
#define __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, ACCESSOR_MAP)                    \
  const Lisple::NAccessorTable& AD_CLASS::static_accessor_table()              \
  {                                                                            \
    static const Lisple::NAccessorTable t ACCESSOR_MAP;                        \
    return t;                                                                  \
  }                                                                            \
  const Lisple::NativeObjectTraits* AD_CLASS::get_traits() const               \
  {                                                                            \
    static const Lisple::NativeObjectTraits traits(HOBJ_T, static_accessor_table()); \
    return &traits;                                                            \
  }

/*
 * __NOBJ_TRAITS_SUB_IMPL__NO_ACCESSORS
 *
 * Generates static_accessor_table() and get_traits() for a sub-adapter that
 * adds no own properties - the table is inherited from the base adapter.
 */
#define __NOBJ_TRAITS_SUB_IMPL__NO_ACCESSORS(AD_CLASS, AD_SUP_CLASS, HOBJ_T)  \
  const Lisple::NAccessorTable& AD_CLASS::static_accessor_table()              \
  {                                                                            \
    return AD_SUP_CLASS::static_accessor_table();                              \
  }                                                                            \
  const Lisple::NativeObjectTraits* AD_CLASS::get_traits() const               \
  {                                                                            \
    static const Lisple::NativeObjectTraits traits(HOBJ_T, static_accessor_table()); \
    return &traits;                                                            \
  }

/*
 * __NOBJ_TRAITS_SUB_IMPL__ACCESSORS
 *
 * Generates static_accessor_table() and get_traits() for a sub-adapter that
 * adds its own properties on top of those inherited from the base adapter.
 * ACCESSOR_MAP is the parenthesised NAccessorTable constructor arg for the
 * sub-adapter's own entries only.
 */
#define __NOBJ_TRAITS_SUB_IMPL__ACCESSORS(AD_CLASS, AD_SUP_CLASS, HOBJ_T, ACCESSOR_MAP) \
  const Lisple::NAccessorTable& AD_CLASS::static_accessor_table()              \
  {                                                                            \
    static const Lisple::NAccessorTable t = Lisple::merge_acc(                 \
      AD_SUP_CLASS::static_accessor_table(), Lisple::NAccessorTable ACCESSOR_MAP); \
    return t;                                                                  \
  }                                                                            \
  const Lisple::NativeObjectTraits* AD_CLASS::get_traits() const               \
  {                                                                            \
    static const Lisple::NativeObjectTraits traits(HOBJ_T, static_accessor_table()); \
    return &traits;                                                            \
  }

#define __NOBJ_P_GETTER(AD_CLASS, FN) [](const Lisple::NativeObjectBase* adapter) { return dynamic_cast<const AD_CLASS*>(adapter)->FN(); }
#define __NOBJ_P_SETTER(AD_CLASS, FN) [](Lisple::NativeObjectBase* adapter, Lisple::Context* ctx, Lisple::sptr_rtval& value) { dynamic_cast<AD_CLASS*>(adapter)->FN(ctx, value); }

#define __NOBJ_P_NO_GETTER Lisple::n_no_getter
#define __NOBJ_P_NO_SETTER Lisple::n_no_setter

#define __NOBJ_P_GET_SINGLE(FN) get_##FN
#define __NOBJ_P_SET_SINGLE(FN) set_##FN


/*
 * Accessor spec expansion - __NOBJ_EXP_SPEC(AD_CLASS, SPEC)
 *
 * A SPEC is a parenthesised tuple that describes one accessor map entry,
 * resolved by arity:
 *
 *   (field)              key = #field,         getter only
 *   ("key", field)       explicit string key,  getter only
 *   (rw, "key", field)   explicit string key,  getter + setter
 *   (wo, "key", field)   explicit string key,  setter only
 *
 * Tags (rw, wo) only appear in 3-element specs, so there is never any
 * ambiguity with string-literal keys in 2-element specs.  The 2-element
 * form is unconditionally (key, field) - no tag detection needed.
 */

/* Access-mode expanders for 3-element specs */
#define __NOBJ_EXP_TAG_rw(AD, KEY, FIELD) \
  {KEY, Lisple::NAccessors(__NOBJ_P_GETTER(AD, get_##FIELD), __NOBJ_P_SETTER(AD, set_##FIELD))}
#define __NOBJ_EXP_TAG_wo(AD, KEY, FIELD) \
  {KEY, Lisple::NAccessors(__NOBJ_P_NO_GETTER, __NOBJ_P_SETTER(AD, set_##FIELD))}

/* 1-element spec: (field) - key inferred via stringification */
#define __NOBJ_EXP_1EL(AD, FIELD) \
  {#FIELD, Lisple::NAccessors(__NOBJ_P_GETTER(AD, get_##FIELD), __NOBJ_P_NO_SETTER)}

/* 2-element spec: ("key", field) - explicit string key, getter only */
#define __NOBJ_EXP_2EL(AD, KEY, FIELD) \
  {KEY, Lisple::NAccessors(__NOBJ_P_GETTER(AD, get_##FIELD), __NOBJ_P_NO_SETTER)}

/* 3-element spec: (tag, "key", field) */
#define __NOBJ_EXP_3EL(AD, TAG, KEY, FIELD) __NOBJ_EXP_TAG_##TAG(AD, KEY, FIELD)

/* Dispatch a single spec tuple by arity */
#define __NOBJ_EXP_SPEC(AD, SPEC)         __NOBJ_EXP_SPEC_IMPL(AD, ESC SPEC)
#define __NOBJ_EXP_SPEC_IMPL(AD, ...)     \
  __SELECT_MACRO__3(__VA_ARGS__, __NOBJ_EXP_3EL, __NOBJ_EXP_2EL, __NOBJ_EXP_1EL)(AD, __VA_ARGS__)


/*
 * __NOBJ_CMAP(AD_CLASS, spec1, spec2, ...)
 *
 * Expands a comma-separated list of accessor spec tuples, injecting
 * AD_CLASS into each entry.  Uses __VA_SELECT_MACRO (from macro_support.h)
 * to pick the right __NOBJ_CMAP_N overload based on the number of specs.
 */
#define __NOBJ_CMAP_1(AD, S)       __NOBJ_EXP_SPEC(AD, S)
#define __NOBJ_CMAP_2(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_1(AD, __VA_ARGS__)
#define __NOBJ_CMAP_3(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_2(AD, __VA_ARGS__)
#define __NOBJ_CMAP_4(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_3(AD, __VA_ARGS__)
#define __NOBJ_CMAP_5(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_4(AD, __VA_ARGS__)
#define __NOBJ_CMAP_6(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_5(AD, __VA_ARGS__)
#define __NOBJ_CMAP_7(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_6(AD, __VA_ARGS__)
#define __NOBJ_CMAP_8(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_7(AD, __VA_ARGS__)
#define __NOBJ_CMAP_9(AD, S, ...)  __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_8(AD, __VA_ARGS__)
#define __NOBJ_CMAP_10(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_9(AD, __VA_ARGS__)
#define __NOBJ_CMAP_11(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_10(AD, __VA_ARGS__)
#define __NOBJ_CMAP_12(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_11(AD, __VA_ARGS__)
#define __NOBJ_CMAP_13(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_12(AD, __VA_ARGS__)
#define __NOBJ_CMAP_14(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_13(AD, __VA_ARGS__)
#define __NOBJ_CMAP_15(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_14(AD, __VA_ARGS__)
#define __NOBJ_CMAP_16(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_15(AD, __VA_ARGS__)
#define __NOBJ_CMAP_17(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_16(AD, __VA_ARGS__)
#define __NOBJ_CMAP_18(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_17(AD, __VA_ARGS__)
#define __NOBJ_CMAP_19(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_18(AD, __VA_ARGS__)
#define __NOBJ_CMAP_20(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_19(AD, __VA_ARGS__)
#define __NOBJ_CMAP_21(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_20(AD, __VA_ARGS__)
#define __NOBJ_CMAP_22(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_21(AD, __VA_ARGS__)
#define __NOBJ_CMAP_23(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_22(AD, __VA_ARGS__)
#define __NOBJ_CMAP_24(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_23(AD, __VA_ARGS__)
#define __NOBJ_CMAP_25(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_24(AD, __VA_ARGS__)
#define __NOBJ_CMAP_26(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_25(AD, __VA_ARGS__)
#define __NOBJ_CMAP_27(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_26(AD, __VA_ARGS__)
#define __NOBJ_CMAP_28(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_27(AD, __VA_ARGS__)
#define __NOBJ_CMAP_29(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_28(AD, __VA_ARGS__)
#define __NOBJ_CMAP_30(AD, S, ...) __NOBJ_EXP_SPEC(AD, S), __NOBJ_CMAP_29(AD, __VA_ARGS__)

#define __NOBJ_CMAP(AD, ...)                                           \
  __VA_SELECT_MACRO(__VA_ARGS__,                                       \
    __NOBJ_CMAP_30, __NOBJ_CMAP_29, __NOBJ_CMAP_28, __NOBJ_CMAP_27,   \
    __NOBJ_CMAP_26, __NOBJ_CMAP_25, __NOBJ_CMAP_24, __NOBJ_CMAP_23,   \
    __NOBJ_CMAP_22, __NOBJ_CMAP_21, __NOBJ_CMAP_20, __NOBJ_CMAP_19,   \
    __NOBJ_CMAP_18, __NOBJ_CMAP_17, __NOBJ_CMAP_16, __NOBJ_CMAP_15,   \
    __NOBJ_CMAP_14, __NOBJ_CMAP_13, __NOBJ_CMAP_12, __NOBJ_CMAP_11,   \
    __NOBJ_CMAP_10, __NOBJ_CMAP_9,  __NOBJ_CMAP_8,  __NOBJ_CMAP_7,    \
    __NOBJ_CMAP_6,  __NOBJ_CMAP_5,  __NOBJ_CMAP_4,  __NOBJ_CMAP_3,    \
    __NOBJ_CMAP_2,  __NOBJ_CMAP_1)                                     \
  (AD, ##__VA_ARGS__)


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

/*
 * __NOBJ_GET_SELF_OBJECT_IMPL
 *
 * Generates get_self_object() for a base adapter - returns the wrapped
 * object directly via get_object().
 */
#define __NOBJ_GET_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)  \
  H_CLASS& AD_CLASS::get_self_object() const            \
  {                                                     \
    return get_object();                                \
  }

/*
 * __NOBJ_GET_DERIVED_SELF_OBJECT_IMPL
 *
 * Generates get_self_object() for a sub-adapter - downcasts the base
 * object returned by get_object() to the concrete derived type.
 */
#define __NOBJ_GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS) \
  H_CLASS& AD_CLASS::get_self_object() const                   \
  {                                                            \
    return dynamic_cast<H_CLASS&>(get_object());               \
  }

/*
 * __NOBJ_GET_SELF_OBJECT_PTR_IMPL
 *
 * Generates self_object_ptr() by forwarding to get_self_object(). Works
 * for both base adapters (returns Base*) and sub-adapters (returns Derived*)
 * because each adapter's get_self_object() already returns the right type.
 */
#define __NOBJ_GET_SELF_OBJECT_PTR_IMPL(AD_CLASS) \
  void* AD_CLASS::self_object_ptr() const         \
  {                                               \
    return &get_self_object();                    \
  }


/**
 * NATIVE_ADAPTER_IMPL variants
 *
 * SPEC_LIST is a parenthesised, comma-separated list of accessor spec
 * tuples, e.g. ((x), ("my-key", field), (rw, w)).  The outer parens keep
 * it a single macro argument; ESC unwraps them inside the macro body.
 */

#define NATIVE_ADAPTER_IMPL__NO_ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T)         \
  __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, (Lisple::NO_N_ACCESSORS))             \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                      \
    : NativeObject(obj_ptr) {}                                                \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                        \
    : NativeObject(obj_ref) {}                                                \
  __NOBJ_GET_BASE_OBJECT_IMPL(AD_CLASS, H_CLASS)                              \
  __NOBJ_GET_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)                              \
  __NOBJ_GET_SELF_OBJECT_PTR_IMPL(AD_CLASS)

#define NATIVE_ADAPTER_IMPL__ACCESSORS(AD_CLASS, H_CLASS, HOBJ_T, ...)        \
  __NOBJ_TRAITS_IMPL(AD_CLASS, HOBJ_T, ({__NOBJ_CMAP(AD_CLASS, __VA_ARGS__)})) \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                      \
    : NativeObject(obj_ptr) {}                                                \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                        \
    : NativeObject(obj_ref) {}                                                \
  __NOBJ_GET_BASE_OBJECT_IMPL(AD_CLASS, H_CLASS)                              \
  __NOBJ_GET_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)                              \
  __NOBJ_GET_SELF_OBJECT_PTR_IMPL(AD_CLASS)

/**
 * NATIVE_ADAPTER_IMPL variants for sub-adapters
 */

#define NATIVE_ADAPTER_IMPL__SUB_NO_ACCESSORS(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, HOBJ_T) \
  __NOBJ_TRAITS_SUB_IMPL__NO_ACCESSORS(AD_CLASS, AD_SUP_CLASS, HOBJ_T)        \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                      \
    : AD_SUP_CLASS(std::unique_ptr<H_SUP_CLASS>(std::move(obj_ptr))) {}       \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                        \
    : AD_SUP_CLASS(static_cast<H_SUP_CLASS&>(obj_ref)) {}                     \
  __NOBJ_GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)                      \
  __NOBJ_GET_SELF_OBJECT_PTR_IMPL(AD_CLASS)

#define NATIVE_ADAPTER_IMPL__SUB_ACCESSORS(AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, HOBJ_T, ...) \
  __NOBJ_TRAITS_SUB_IMPL__ACCESSORS(AD_CLASS, AD_SUP_CLASS, HOBJ_T, ({__NOBJ_CMAP(AD_CLASS, __VA_ARGS__)})) \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr)                      \
    : AD_SUP_CLASS(std::unique_ptr<H_SUP_CLASS>(std::move(obj_ptr))) {}       \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref)                                        \
    : AD_SUP_CLASS(static_cast<H_SUP_CLASS&>(obj_ref)) {}                     \
  __NOBJ_GET_DERIVED_SELF_OBJECT_IMPL(AD_CLASS, H_CLASS)                      \
  __NOBJ_GET_SELF_OBJECT_PTR_IMPL(AD_CLASS)


/**
 * NATIVE_ADAPTER_IMPL - main implementation macro
 *
 * Usage (no properties):
 *   NATIVE_ADAPTER_IMPL(MyAdapter, MyClass, &MY_TYPE);
 *
 * Usage (with accessor specs):
 *   NATIVE_ADAPTER_IMPL(MyAdapter, MyClass, &MY_TYPE,
 *     (field1), ("kebab-key", field2), (rw, field3));
 */
#define NATIVE_ADAPTER_IMPL(AD_CLASS, H_CLASS, HOBJ_T, ...)                   \
  __VA_SELECT_MACRO(__VA_OPT__(__VA_ARGS__,)                                   \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__ACCESSORS, NATIVE_ADAPTER_IMPL__ACCESSORS,            \
    NATIVE_ADAPTER_IMPL__NO_ACCESSORS)                                         \
  (AD_CLASS, H_CLASS, HOBJ_T __VA_OPT__(, __VA_ARGS__))

/**
 * NATIVE_SUB_ADAPTER_IMPL - implementation macro for sub-adapters
 *
 * SUB_PAIR is a parenthesised (AdapterClass, HostClass) pair.
 *
 * Usage (inherits base props only):
 *   NATIVE_SUB_ADAPTER_IMPL(BaseAdapter, BaseClass, (SubAdapter, SubClass), &SUB_TYPE);
 *
 * Usage (with own accessor specs):
 *   NATIVE_SUB_ADAPTER_IMPL(BaseAdapter, BaseClass, (SubAdapter, SubClass), &SUB_TYPE,
 *     (field1), ("key", field2));
 *
 * Two-level dispatch is used to expand SUB_PAIR into two separate arguments
 * (AD_CLASS, H_CLASS) before the variant selector runs. ESC SUB_PAIR enters
 * DISPATCH1 as one collected arg; when __VA_ARGS__ is substituted in the body
 * of DISPATCH1, ESC expands the pair so DISPATCH2 sees AD_CLASS and H_CLASS
 * as distinct positional parameters.
 */
#define __NATIVE_SUB_ADAPTER_IMPL_DISPATCH2(AD_SUP_CLASS,                         \
                                            H_SUP_CLASS,                          \
                                            HOBJ_T,                               \
                                            AD_CLASS,                             \
                                            H_CLASS,                              \
                                            ...)                                  \
  __VA_SELECT_MACRO(__VA_OPT__(__VA_ARGS__,)                                      \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_ACCESSORS, NATIVE_ADAPTER_IMPL__SUB_ACCESSORS,       \
    NATIVE_ADAPTER_IMPL__SUB_NO_ACCESSORS)                                        \
  (AD_CLASS, H_CLASS, AD_SUP_CLASS, H_SUP_CLASS, HOBJ_T __VA_OPT__(, __VA_ARGS__))

#define __NATIVE_SUB_ADAPTER_IMPL_DISPATCH1(AD_SUP_CLASS, H_SUP_CLASS, HOBJ_T, ...) \
  __NATIVE_SUB_ADAPTER_IMPL_DISPATCH2(AD_SUP_CLASS, H_SUP_CLASS, HOBJ_T, __VA_ARGS__)

#define NATIVE_SUB_ADAPTER_IMPL(AD_SUP_CLASS, H_SUP_CLASS, SUB_PAIR, HOBJ_T, ...)   \
  __NATIVE_SUB_ADAPTER_IMPL_DISPATCH1(AD_SUP_CLASS,                                 \
                                      H_SUP_CLASS,                                  \
                                      HOBJ_T,                                       \
                                      ESC SUB_PAIR __VA_OPT__(, __VA_ARGS__))

// clang-format on

namespace Lisple
{

  class NAccessors;
  class NAccessorTable;
  struct NativeObjectTraits;

  extern const NAccessorTable NO_N_ACCESSORS;

  typedef std::unordered_map<std::string, NAccessors> n_acc_map;
  typedef std::unordered_map<sptr_rtval, NAccessors> key_n_acc_map;

  /*! @brief Convenience type definition for GETTER function references */
  using n_acc_get_t = sptr_rtval (*)(const NativeObjectBase*);
  /*! @brief Convenience type definition for SETTER function references */
  using n_acc_set_t = void (*)(NativeObjectBase*, Context*, Lisple::sptr_rtval&);

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

  struct NCacheSlot
  {
    sptr_rtval cached = nullptr;
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
     * specifications keyed by RTValue keyword objects.
     */
    NAccessorTable(const key_n_acc_map& accessors);

    /*!
     * @brief Constructs an AccessorTable from a braced list of
     * (string-key, NAccessors) pairs.  String keys are used directly; the
     * corresponding keyword RTValues are interned into the keys vector.
     */
    NAccessorTable(std::initializer_list<std::pair<std::string, NAccessors>> entries);

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
   * @brief Constructs a new NAccessorTable by merging a base table with an
   * additional table. Entries in the base take precedence when the same key
   * appears in both. Used internally by NATIVE_SUB_ADAPTER_IMPL.
   */
  NAccessorTable merge_acc(const NAccessorTable& base, const NAccessorTable& additional);

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
    void set_property(const RTValue& property, sptr_rtval& value);
    void set_property(const RTValue& property, const sptr_rtval& value);
    virtual const NativeObjectTraits* get_traits() const = 0;
    const HostTypeRef* get_host_type() const;
    const NAccessorTable& accessor_table() const;
    virtual std::string to_string() const;
    /** Returns a void* to the concrete host object as known by the adapter. */
    virtual void* self_object_ptr() const = 0;
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

    std::unique_ptr<T> release_pointer() { return std::move(object->get_object_ptr()); }
    virtual T& get_object() const = 0;
  };

  template <typename T> T& obj(const RTValue& v)
  {
    if (v.type == RTValue::Type::OBJECT)
    {
      return dynamic_cast<HostObject<T>*>(v.obj().get())->get_object();
    }

    auto* nobj = v.nobj().get();
    if (auto* typed = dynamic_cast<NativeObject<T>*>(nobj)) return typed->get_object();

    /**
     * If T is a derived host class held via a base adapter; self_object_ptr()
     * returns &get_self_object() which yields Derived* directly.
     */
    return *static_cast<T*>(nobj->self_object_ptr());
  }

} // namespace Lisple

#endif /* LISPLE__HOST__OBJECT_H */
