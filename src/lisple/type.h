
#ifndef __LISPLE_TYPE_H_
#define __LISPLE_TYPE_H_

#include "runtime/value.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Lisple
{
  enum class Form : uint8_t
  {
    ANY,
    ARRAY,
    B_TRUE,
    B_FALSE,
    BOOLEAN,
    CHAR,
    FUNCTION,
    HOST_OBJECT,
    KEY,
    LIST,
    MACRO,
    MAP,
    NIL,
    NUMBER,
    STRING,
    SYMBOL,
    WORD,
    DISCARD,
    HOST_SEQ
  };

  class Context;
  class Runtime;
  class Object;

  typedef std::shared_ptr<Object> sptr_sobject;
  typedef std::vector<std::shared_ptr<Object>> sptr_sobject_v;

  template <typename T> struct CoercionResult
  {
    bool success;
    std::shared_ptr<T> result;
  };

  /*! @brief Basic type reference and identifier for built-in Lisple types */
  class TypeRef
  {
   protected:
    const RTValue::Type value_type;
    const Form form_type;

   public:
    const std::string name;

    TypeRef(RTValue::Type value_type, Form form_type, const std::string& name);
    virtual ~TypeRef() = default;

    virtual bool is_type_of(const RTValue& val) const;
    virtual bool is_type_of(const Object& obj) const;

    virtual CoercionResult<Object> coerce(Context& ctx, sptr_sobject& obj) const;
    virtual CoercionResult<RTValue> coerce(Context& ctx, sptr_rtval& val) const;

    CoercionResult<Object> coerce(Runtime& reader, sptr_sobject& obj) const;

    virtual bool is_host_object() const;

    const std::string& to_string() const;

    friend class SeqRef;
  };

  /*! @brief Type reference wrapper for when two or more types are acceptable */
  class MultiRef : public TypeRef
  {
    std::vector<const TypeRef*> types;

   public:
    MultiRef(std::vector<const TypeRef*> types, const std::string& name);

    bool is_type_of(const RTValue& val) const override;
    bool is_type_of(const Object& obj) const override;
    CoercionResult<Object> coerce(Context& ctx, sptr_sobject& obj) const override;
    CoercionResult<RTValue> coerce(Context& ctx, sptr_rtval& val) const override;
  };

  /*! @brief Type reference for when any type is acceptable */
  class AnyRef : public TypeRef
  {
   public:
    AnyRef();
    bool is_type_of(const RTValue& val) const override;
    bool is_type_of(const Object& obj) const override;
  };

  /*!
   * @brief Type reference wrapper for sequence types holding a specific type,
   * ie, Array of Number
   */
  class SeqRef : public TypeRef
  {
    const TypeRef* seq_type;
    const TypeRef* child_type;

   public:
    SeqRef(const TypeRef* seq_type, const TypeRef* child_type, const std::string& name);

    bool is_type_of(const RTValue& val) const override;
    bool is_type_of(const Object& obj) const override;
    CoercionResult<Object> coerce(Context& ctx, sptr_sobject& obj) const override;
    CoercionResult<RTValue> coerce(Context& ctx, sptr_rtval& val) const override;
  };

  namespace Type
  {
    inline const TypeRef LIST(RTValue::Type::LIST, Form::LIST, "List");
    inline const TypeRef ARRAY(RTValue::Type::VECTOR, Form::ARRAY, "Vector");
    inline const TypeRef BOOL(RTValue::Type::BOOL, Form::BOOLEAN, "Boolean");
    inline const TypeRef CHAR(RTValue::Type::CHAR, Form::CHAR, "Char");
    inline const TypeRef MAP(RTValue::Type::MAP, Form::MAP, "Map");
    inline const TypeRef STRING(RTValue::Type::STRING, Form::STRING, "String");
    inline const TypeRef NUMBER(RTValue::Type::NUMBER, Form::NUMBER, "Number");
    inline const TypeRef SYMBOL(RTValue::Type::SYMBOL, Form::SYMBOL, "Symbol");
    inline const TypeRef WORD(RTValue::Type::SYMBOL, Form::WORD, "Word");
    inline const TypeRef KEY(RTValue::Type::KEYWORD, Form::KEY, "Keyword");
    inline const TypeRef FUNCTION(RTValue::Type::FUNCTION, Form::FUNCTION, "Function");
    inline const TypeRef MACRO(RTValue::Type::FUNCTION, Form::MACRO, "Macro");
    inline const TypeRef HOST_OBJECT(RTValue::Type::OBJECT, Form::HOST_OBJECT, "HostObject");
    inline const TypeRef HOST_SEQ(RTValue::Type::OBJECT, Form::HOST_SEQ, "HostSeq");
    inline const TypeRef NIL(RTValue::Type::NIL, Form::NIL, "nil");

    inline const SeqRef ARRAY_OF_ARRAY(&ARRAY, &ARRAY, "[Array]");
    inline const SeqRef ARRAY_OF_KEY(&ARRAY, &KEY, "[Key]");
    inline const SeqRef ARRAY_OF_FUNCTION(&ARRAY, &FUNCTION, "[Function]");
    inline const SeqRef ARRAY_OF_NUMBER(&ARRAY, &NUMBER, "[Number]");
    inline const SeqRef ARRAY_OF_CHAR(&ARRAY, &CHAR, "[Char]");
    inline const SeqRef ARRAY_OF_STRING(&ARRAY, &STRING, "[String]");
    inline const SeqRef ARRAY_OF_SYMBOL(&ARRAY, &SYMBOL, "[Symbol]");
    inline const SeqRef ARRAY_OF_MAP(&ARRAY, &MAP, "[Map]");

    inline const MultiRef QUALIFIABLE({&KEY, &WORD, &SYMBOL}, "Qualifiable");
    inline const MultiRef SYMBOL_VALUE({&SYMBOL, &WORD}, "Symbol");

    inline const MultiRef SEQ(std::vector<const TypeRef*>{&LIST, &ARRAY, &MAP, &HOST_SEQ},
                              "Seq");
    inline const MultiRef SEQ_OR_STRING(std::vector<const TypeRef*>({&SEQ, &STRING}),
                                        "SeqLike");
    inline const MultiRef EXEC(std::vector<const TypeRef*>{&FUNCTION, &MACRO, &KEY}, "Exec");
    inline const MultiRef COMPLEX(std::vector<const TypeRef*>{&MAP, &HOST_OBJECT},
                                  "Complex");

    inline const AnyRef ANY;
  } // namespace Type
} // namespace Lisple

#endif
