
#ifndef __HOST_H_
#define __HOST_H_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "form.h"
#include "type.h"
#include "lisple_exception.h"

#define __SELECT_MACRO__2(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define __HOST_TYPE(CONST, NAME, MAKE_FN) inline const HostTypeRef CONST = HostTypeRef(HostObjectType::CONST, NAME, MAKE_FN);

#define __HOST_TYPE__NO_MAKE_FN(CONST, NAME) inline const HostTypeRef CONST = HostTypeRef(HostObjectType::CONST, NAME);

#define HOST_TYPE(CONST, NAME, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_TYPE, __HOST_TYPE__NO_MAKE_FN)(CONST, NAME, ##__VA_ARGS__)


#define HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)                                     \
    template <typename T, typename... Args>                                                \
    static std::shared_ptr<AD_CLASS> make(Args&&... args)                                  \
    {                                                                                      \
      return std::make_shared<AD_CLASS>(std::make_unique<T>(std::forward<Args>(args)...)); \
    }                                                                                      \
    static std::shared_ptr<AD_CLASS> make_ref(const H_CLASS& ref)                          \
    {                                                                                      \
      return std::make_shared<AD_CLASS>(const_cast<H_CLASS&>(ref));                        \
    }

#define HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                                                                \
  class AD_CLASS : public Lisple::HostObject<H_CLASS>                                                            \
  {                                                                                                              \
   public:                                                                                                       \
    AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access = Lisple::NO_ACCESSORS);  \
    AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access = Lisple::NO_ACCESSORS);                    \
    HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define SUB_ADAPTER_MAIN_DECL(AD_CLASS, AD_SUP_CLASS, H_CLASS)                                \
  class AD_CLASS : public AD_SUP_CLASS                                                        \
  {                                                                                           \
   public:                                                                                    \
    AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access={});   \
    AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access={});                     \
    HOST_ADAPTER_STATIC_FACTORY(AD_CLASS, H_CLASS)

#define HOST_ADAPTER_END_DECL                        \
  }

#define HOST_ADAPTER_GETTER_DECL(PROP_NAME)          \
  Lisple::sptr_sobject get_##PROP_NAME() const;

#define HOST_ADAPTER_GETTERS1(PROP1)                 \
  HOST_ADAPTER_GETTER_DECL(PROP1)

#define HOST_ADAPTER_GETTERS2(PROP1, PROP2) \
  HOST_ADAPTER_GETTER_DECL(PROP1)           \
  HOST_ADAPTER_GETTER_DECL(PROP2)

#define HOST_ADAPTER_GETTERS3(PROP1, PROP2, PROP3)  \
  HOST_ADAPTER_GETTER_DECL(PROP1)                   \
  HOST_ADAPTER_GETTER_DECL(PROP2)                   \
  HOST_ADAPTER_GETTER_DECL(PROP3)

#define HOST_ADAPTER_GETTERS4(PROP1, PROP2, PROP3, PROP4) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                         \
  HOST_ADAPTER_GETTER_DECL(PROP2)                         \
  HOST_ADAPTER_GETTER_DECL(PROP3)                         \
  HOST_ADAPTER_GETTER_DECL(PROP4)

#define HOST_ADAPTER_GETTERS5(PROP1, PROP2, PROP3, PROP4, PROP5)  \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                 \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                 \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                 \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                 \
  HOST_ADAPTER_GETTER_DECL(PROP5)

#define HOST_ADAPTER_GETTERS6(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                       \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                       \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                       \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                       \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                       \
  HOST_ADAPTER_GETTER_DECL(PROP6)

#define HOST_ADAPTER_GETTERS7(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP6)                                              \
  HOST_ADAPTER_GETTER_DECL(PROP7)

#define HOST_ADAPTER_GETTERS8(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7, PROP8) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP6)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP7)                                                     \
  HOST_ADAPTER_GETTER_DECL(PROP8)

#define HOST_ADAPTER_GETTERS9(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7, PROP8, PROP9) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP6)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP7)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP8)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP9)

#define HOST_ADAPTER_GETTERS10(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7, PROP8, PROP9, PROP10) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP6)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP7)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP8)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP9)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP10)

#define HOST_ADAPTER_GETTERS11(PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7, PROP8, PROP9, PROP10, PROP11) \
  HOST_ADAPTER_GETTER_DECL(PROP1)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP2)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP3)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP4)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP5)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP6)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP7)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP8)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP9)                                                            \
  HOST_ADAPTER_GETTER_DECL(PROP10)                                                           \
  HOST_ADAPTER_GETTER_DECL(PROP11)

#define SELECT_GETTERS_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, MACRO_NAME, ...) MACRO_NAME
#define HOST_ADAPTER_GETTERS(...) SELECT_GETTERS_MACRO(__VA_ARGS__, HOST_ADAPTER_GETTERS11, HOST_ADAPTER_GETTERS10, HOST_ADAPTER_GETTERS9, HOST_ADAPTER_GETTERS8, HOST_ADAPTER_GETTERS7, HOST_ADAPTER_GETTERS6, HOST_ADAPTER_GETTERS5, HOST_ADAPTER_GETTERS4, HOST_ADAPTER_GETTERS3, HOST_ADAPTER_GETTERS2, HOST_ADAPTER_GETTERS1)(__VA_ARGS__)

#define HOST_ADAPTER_SETTER_DECL(PROP_NAME)       \
  void set_##PROP_NAME(Lisple::Context* ctx, Lisple::Object& value);

#define HOST_ADAPTER_SETTERS1(PROP1)                 \
  HOST_ADAPTER_SETTER_DECL(PROP1)

#define HOST_ADAPTER_SETTERS2(PROP1, PROP2) \
  HOST_ADAPTER_SETTER_DECL(PROP1)           \
  HOST_ADAPTER_SETTER_DECL(PROP2)

#define HOST_ADAPTER_SETTERS3(PROP1, PROP2, PROP3)  \
  HOST_ADAPTER_SETTER_DECL(PROP1)                   \
  HOST_ADAPTER_SETTER_DECL(PROP2)                   \
  HOST_ADAPTER_SETTER_DECL(PROP3)

#define HOST_ADAPTER_SETTERS4(PROP1, PROP2, PROP3, PROP4) \
  HOST_ADAPTER_SETTER_DECL(PROP1)                         \
  HOST_ADAPTER_SETTER_DECL(PROP2)                         \
  HOST_ADAPTER_SETTER_DECL(PROP3)                         \
  HOST_ADAPTER_SETTER_DECL(PROP4)

#define HOST_ADAPTER_SETTERS5(PROP1, PROP2, PROP3, PROP4, PROP5)  \
  HOST_ADAPTER_SETTER_DECL(PROP1)                                 \
  HOST_ADAPTER_SETTER_DECL(PROP2)                                 \
  HOST_ADAPTER_SETTER_DECL(PROP3)                                 \
  HOST_ADAPTER_SETTER_DECL(PROP4)                                 \
  HOST_ADAPTER_SETTER_DECL(PROP5)

#define SELECT_SETTERS_MACRO(_1, _2, _3, _4, _5, MACRO_NAME, ...) MACRO_NAME
#define HOST_ADAPTER_SETTERS(...) SELECT_SETTERS_MACRO(__VA_ARGS__, HOST_ADAPTER_SETTERS5, HOST_ADAPTER_SETTERS4, HOST_ADAPTER_SETTERS3, HOST_ADAPTER_SETTERS2, HOST_ADAPTER_SETTERS1)(__VA_ARGS__)

#define SUB_ADAPTER__NO_PROPS(AD_CLASS, SUP_AD_CLASS, H_CLASS)  \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)        \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_GETTERS(AD_CLASS, SUP_AD_CLASS, H_CLASS, GET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)                \
  HOST_ADAPTER_GETTERS GET_PROPS                                        \
  HOST_ADAPTER_END_DECL

#define SUB_ADAPTER__WITH_PROPS(AD_CLASS, SUP_AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  SUB_ADAPTER_MAIN_DECL(AD_CLASS, SUP_AD_CLASS, H_CLASS)                \
  HOST_ADAPTER_GETTERS GET_PROPS                                        \
  HOST_ADAPTER_SETTERS SET_PROPS                                        \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__NO_PROPS(AD_CLASS, H_CLASS)    \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)          \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_GETTERS(AD_CLASS, H_CLASS, GET_PROPS) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                      \
  HOST_ADAPTER_GETTERS GET_PROPS                                 \
  HOST_ADAPTER_END_DECL

#define HOST_ADAPTER__WITH_PROPS(AD_CLASS, H_CLASS, GET_PROPS, SET_PROPS) \
  HOST_ADAPTER_MAIN_DECL(AD_CLASS, H_CLASS)                               \
  HOST_ADAPTER_GETTERS GET_PROPS                                          \
  HOST_ADAPTER_SETTERS SET_PROPS                                          \
  HOST_ADAPTER_END_DECL

#define SELECT_ADAPTER_MACRO(_1, _2, _3, MACRO_NAME, ...) MACRO_NAME
#define HOST_SUB_ADAPTER(AD_CLASS, SUP_AD_CLASS, H_CLASS, ...)                                                     \
  SELECT_ADAPTER_MACRO(0, ##__VA_ARGS__, SUB_ADAPTER__WITH_PROPS, SUB_ADAPTER__WITH_GETTERS, SUB_ADAPTER__NO_PROPS) \
  (AD_CLASS, SUP_AD_CLASS, H_CLASS, ##__VA_ARGS__)
#define HOST_ADAPTER(AD_CLASS, H_CLASS, ...)                            \
  SELECT_ADAPTER_MACRO(0, ##__VA_ARGS__, HOST_ADAPTER__WITH_PROPS, HOST_ADAPTER__WITH_GETTERS, HOST_ADAPTER__NO_PROPS) \
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

#define HOST_ADAPTER_IMPL__NO_ACCESSORS(TYPE_NAME, AD_CLASS, H_CLASS, HOBJ_T)            \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
    : HostObject(TYPE_NAME, Lisple::HostObjectType::HOBJ_T, obj_ptr, _access) { }        \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access) \
    : HostObject(TYPE_NAME, Lisple::HostObjectType::HOBJ_T, obj_ref, _access) { }

#define HOST_ADAPTER_IMPL__ACCESSORS(TYPE_NAME, AD_CLASS, H_CLASS, HOBJ_T, ACCESSOR_MAP)   \
  AD_CLASS::AD_CLASS(std::unique_ptr<H_CLASS>&& obj_ptr, const Lisple::AccessorLookup& _access) \
  : HostObject(TYPE_NAME, Lisple::HostObjectType::HOBJ_T, obj_ptr, Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {} \
  AD_CLASS::AD_CLASS(H_CLASS& obj_ref, const Lisple::AccessorLookup& _access)          \
    : HostObject(TYPE_NAME, Lisple::HostObjectType::HOBJ_T, obj_ref, Lisple::merge_acc(_access, ESC ACCESSOR_MAP)) {}

#define SELECT_SUB_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME
#define SELECT_ADAPTER_IMPL_MACRO(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define HOST_SUB_ADAPTER_IMPL(SUP_AD_CLASS, AD_CLASS, H_CLASS, ...) SELECT_SUB_ADAPTER_IMPL_MACRO(0, ##__VA_ARGS__, HOST_SUB_ADAPTER_IMPL__ACCESSORS, HOST_SUB_ADAPTER_IMPL__NO_ACCESSORS)(SUP_AD_CLASS, AD_CLASS, H_CLASS, ##__VA_ARGS__)

#define HOST_ADAPTER_IMPL(TYPE_NAME, AD_CLASS, H_CLASS, HOBJ_T, ...) SELECT_ADAPTER_IMPL_MACRO(0, ##__VA_ARGS__, HOST_ADAPTER_IMPL__ACCESSORS, HOST_ADAPTER_IMPL__NO_ACCESSORS)(TYPE_NAME, AD_CLASS, H_CLASS, HOBJ_T, ##__VA_ARGS__)

#define ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)           \
  Lisple::sptr_sobject AD_CLASS::get_##PROP_NAME() const

#define ADAPTER_PROP_GET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_METHOD)   \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                               \
  {                                                                   \
    return std::make_shared<LISPLE_FORM>(get_object().OBJ_METHOD());  \
  }

#define __ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                               \
  {                                                                   \
    return std::make_shared<LISPLE_FORM>(get_object().OBJ_FIELD);     \
  }

#define __ADAPTER_PROP_GET__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM)    \
  __ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET__FIELD, __ADAPTER_PROP_GET__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define __ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                           \
  {                                                                               \
    return std::make_shared<LISPLE_FORM>(*get_object().METHOD());     \
  }

#define __ADAPTER_PROP_GET_P__METHOD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM)    \
  __ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_P__METHOD, __ADAPTER_PROP_GET_P__METHOD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define __ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)    \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                               \
  {                                                                   \
    return std::make_shared<LISPLE_FORM>(*get_object().OBJ_FIELD);     \
  }

#define __ADAPTER_PROP_GET_P__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM)    \
  __ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_P__FIELD, __ADAPTER_PROP_GET_P__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)


#define ADAPTER_PROP_GET_VECTOR__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD_NAME) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                \
  {                                                                                    \
    Lisple::sptr_sobject_v v;                                                          \
    for (auto& obj : get_object().METHOD_NAME())                                       \
    {                                                                                  \
      v.push_back(std::make_shared<LISPLE_FORM>(obj));                                 \
    }                                                                                  \
    return std::make_shared<Lisple::Array>(v);                                         \
  }

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

#define __ADAPTER_PROP_GET_VECTOR__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM) \
  __ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_VECTOR__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)      \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_VECTOR__FIELD, __ADAPTER_PROP_GET_VECTOR__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

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

#define __ADAPTER_PROP_GET_VECTOR_P__METHOD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM) \
  __ADAPTER_PROP_GET_VECTOR_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_VECTOR_P__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_VECTOR_P__METHOD, __ADAPTER_PROP_GET_VECTOR_P__METHOD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

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

#define __ADAPTER_PROP_GET_VECTOR_P__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM) \
  __ADAPTER_PROP_GET_VECTOR_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_VECTOR_P__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)    \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_VECTOR_P__FIELD, __ADAPTER_PROP_GET_VECTOR_P__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define __ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                     \
  {                                                                                         \
    return std::make_shared<LISPLE_FORM>(get_object().OBJ_FIELD);                           \
  }

#define __ADAPTER_PROP_GET_HOST_OBJECT__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM)  \
  __ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)        \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_HOST_OBJECT__FIELD, __ADAPTER_PROP_GET_HOST_OBJECT__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_HOST_OBJECT__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD) \
  ADAPTER_PROP_GET(AD_CLASS, PROP_NAME)                                                \
  {                                                                                    \
    return std::make_shared<LISPLE_FORM>(get_object().METHOD());                       \
  }

#define ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                  \
  void AD_CLASS::set_##PROP_NAME([[maybe_unused]]Lisple::Context* ctx, Lisple::Object& value)

#define ADAPTER_PROP_SET__METHOD(AD_CLASS, PROP_NAME, LISPLE_FORM, METHOD)     \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                        \
  {                                                                            \
    get_object().METHOD(value.as<LISPLE_FORM>().value);                        \
  }

#define ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                        \
  {                                                                            \
    get_object().OBJ_FIELD = value.as<LISPLE_FORM>().value;                    \
  }

#define ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_SET(AD_CLASS, PROP_NAME)                                                   \
  {                                                                                       \
    get_object().OBJ_FIELD = value.as<LISPLE_FORM>().get_object();                        \
  }

#define __ADAPTER_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)   \
  ADAPTER_PROP_GET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)               \
  ADAPTER_PROP_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)

#define __ADAPTER_PROP_GET_SET__FIELD__SAME_NAME(AD_CLASS, PROP_NAME, LISPLE_FORM)   \
  __ADAPTER_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, PROP_NAME)

#define ADAPTER_PROP_GET_SET__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, ...)   \
  __SELECT_MACRO__2(0, ##__VA_ARGS__, __ADAPTER_PROP_GET_SET__FIELD, __ADAPTER_PROP_GET_SET__FIELD__SAME_NAME)(AD_CLASS, PROP_NAME, LISPLE_FORM, ##__VA_ARGS__)

#define ADAPTER_PROP_GET_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)  \
  ADAPTER_PROP_GET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)            \
  ADAPTER_PROP_SET_HOST_OBJECT__FIELD(AD_CLASS, PROP_NAME, LISPLE_FORM, OBJ_FIELD)

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

#define DECL_SHKEY(CONSTNAME) extern const std::shared_ptr<Lisple::Key> CONSTNAME;
#define SHKEY(CONSTNAME,KEYNAME) const std::shared_ptr<Lisple::Key> CONSTNAME = std::make_shared<Lisple::Key>(KEYNAME);


namespace Lisple
{
  class AccessorLookup;
  class Accessors;
  enum class HostObjectType : short;
  extern const AccessorLookup NO_ACCESSORS;

  /**
   *
   */
  class HostTypeRef : public ::Lisple::TypeRef
  {
    Lisple::HostObjectType host_type;
    std::unique_ptr<std::string> make_fn;

   public:
    HostTypeRef(Lisple::HostObjectType host_type, const std::string& name, const std::string& make_fn = "");

    bool is_type_of(const Lisple::Object& obj) const override;
    CoercionResult coerce(Context& ctx, sptr_sobject& obj) const override;
  };

  typedef std::map<std::string, Accessors> acc_map;
  typedef std::map<Lisple::sptr_sobject, Accessors> key_acc_map;

  /**
   *
   */
  class AccessorLookup
  {
   public:
    std::vector<Lisple::sptr_sobject> keys;
    acc_map accessor_map;

    AccessorLookup();
    AccessorLookup(const key_acc_map& accessors);

    bool has_key(const Object& key) const;
    const Accessors& lookup(const Object& key) const;
  };

  AccessorLookup merge_acc(const AccessorLookup& al1, const key_acc_map& kam2);

  /**
   *
   */
  class AbstractHostObject : public Lisple::Object
  {
    HostObjectType host_type;
    AccessorLookup accessors;

   public:
    AbstractHostObject(HostObjectType type);
    AbstractHostObject(HostObjectType type, const AccessorLookup& accessors);

    bool operator==(const Object& other) const override;
    virtual std::string to_string() const override;

    HostObjectType get_host_type() const;

    virtual const std::vector<Lisple::sptr_sobject> keys() const;
    bool has_key(const Lisple::Object& key) const override;
    Lisple::sptr_sobject get_sptr_property(const Lisple::Object& key) const override;
    void set_property(const Lisple::Object& key, sptr_sobject& value) override;
    void set_property(Context* ctx, const Lisple::Object& key, sptr_sobject& value) override;

    const key_acc_map& get_accessors() const;
  };

  /*! \brief Convenience type definition for GETTER function references */
  typedef std::function<Lisple::sptr_sobject(const AbstractHostObject*)> acc_get_t;
  /*! \brief Convenience type definition for SETTER function references */
  typedef std::function<void(AbstractHostObject*, Lisple::Context*, Lisple::Object&)> acc_set_t;

  /*! \brief Stock getter-implementation for non-gettable properties that
   * returns Lisple::NIL */
  extern const acc_get_t no_getter;
  /*! \brief Stock setter-implementation for non-settable properties that always
   * throws InvocationException
   * */
  extern const acc_set_t no_setter;

  /**
   * \brief Holds function references to getters and setters for a specific
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

  /**
   * \brief Abstract base class for wrapping the object held by a Host Object
   * Adapter.
   *
   */
  template<class T>
  class ValueHolder
  {
   public:
    virtual ~ValueHolder() = default;

    virtual T& get_object() = 0;
    virtual std::unique_ptr<T>& get_object_ptr() = 0;
  };

  /**
   * \brief ValueHolder-implementation that owns the Host Object instance,
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

  /**
   * \brief ValueHolder-implementation that holds a reference to the Host Object
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
      throw Lisple::InvocationException("Cannot give up ownership of Object Reference");
    }
  };

  /**
   * Template base class for Host Object Adapters. Concrete adapters will
   * typically be derived directly from this class, using the HOST_ADAPTER and
   * HOST_ADAPTER_IMPL macros for convenience.
   */
  template<class T>
  class HostObject : public Lisple::AbstractHostObject
  {
   protected:
    const std::string type_name;
    std::unique_ptr<ValueHolder<T>> object;
    /*! Internally cached list of Lisple/HostObject children */
    sptr_sobject_v __cached_children;

   public:
    /**
     * \brief Creates a HostObject instance holding a UNIQUE POINTER to a host
     * object.
     *
     * This is to be used when the HostObject will own the underlying object
     * instance.
     *
     * \param type_name The name of the object type
     * \param type Enum type value of the Object
     * \param object The actual object
     * \param accessors Description of setters and getters and how they are
     *        invoked
     */
    HostObject(const std::string& type_name, Lisple::HostObjectType type, std::unique_ptr<T>& object, const AccessorLookup& accessors = {})
      : AbstractHostObject(type, accessors)
      , type_name(type_name)
      , object(std::make_unique<HostObjectValue<T>>(object))
    {
    }

    /**
     *  \brief Creates a HostObject instance holding a REFERENCE to a host
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
     *  \param type_name The name of the object type
     *  \param type Enum type value of the object
     *  \param object The actual object reference
     *  \param accessors Description of getters and setters, and how they
     *         are invoked
     */
    HostObject(const std::string& type_name, Lisple::HostObjectType type, T& object, const AccessorLookup& accessors = {})
      : AbstractHostObject(type, accessors)
      , type_name(type_name)
      , object(std::make_unique<HostObjectRef<T>>(object))
    {
    }

    T& get_object() const
    {
      return object->get_object();
    }

    /**
     * Returns the properties of the HostObject with :key and value
     * interspersed, as if a Map. Because Object::get_children() assumes
     * ownership of the child vector we return by reference, which is why
     * the __cached_children is needed.
     * This could give rise to all sorts of problems down the road, and
     * we should probably just own up to the fact that temporary copies
     * of shared_ptr and vector aren't expensive and change the signature
     * to always return the child array by copy.
     */
    sptr_sobject_v& get_children() override
    {
      sptr_sobject_v kvs;
      for (auto& k : keys())
      {
        kvs.push_back(k);
        kvs.push_back(get_sptr_property(*k));
      }
      __cached_children = kvs;
      return __cached_children;
    }

    std::string to_string() const override
    {
      sptr_sobject_v kvs = const_cast<HostObject<T>&>(*this).get_children();

      return Map(kvs).to_string();
    }

    std::unique_ptr<T>& get_object_ptr()
    {
      return object->get_object_ptr();
    }
  };

}

#endif
