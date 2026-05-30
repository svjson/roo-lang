
#ifndef __ROO_TYPE_H_
#define __ROO_TYPE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <roo/runtime/value.h>

namespace Roo
{
  enum class Form : uint8_t
  {
    ANY,
    VECTOR,
    B_TRUE,
    B_FALSE,
    BOOLEAN,
    CHAR,
    FUNCTION,
    HOST_OBJECT,
    KEYWORD,
    LIST,
    MACRO,
    MAP,
    NIL,
    NUMBER,
    STRING,
    SYMBOL,
    QUOTED_SYMBOL,
    DISCARD,
    HOST_SEQ
  };

  class Context;
  class Runtime;
  namespace AST
  {
    class ASTNode;
  }

  typedef std::shared_ptr<AST::ASTNode> sptr_ast_node;
  typedef std::vector<std::shared_ptr<AST::ASTNode>> sptr_ast_node_v;

  struct CoercionResult
  {
    bool success;
    sptr_val result;
  };

  /*! @brief Basic type reference and identifier for built-in Roo types */
  class TypeRef
  {
   protected:
    const Value::Type value_type;
    const Form form_type;

   public:
    const std::string name;

    TypeRef(Value::Type value_type, Form form_type, const std::string& name);
    virtual ~TypeRef() = default;

    virtual bool is_type_of(const Value& val) const;
    virtual bool is_type_of(const AST::ASTNode& obj) const;

    virtual CoercionResult coerce(Context& ctx, sptr_val& val) const;

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

    bool is_type_of(const Value& val) const override;
    bool is_type_of(const AST::ASTNode& obj) const override;
    CoercionResult coerce(Context& ctx, sptr_val& val) const override;
  };

  /*! @brief Type reference for when any type is acceptable */
  class AnyRef : public TypeRef
  {
   public:
    AnyRef();
    bool is_type_of(const Value& val) const override;
    bool is_type_of(const AST::ASTNode& obj) const override;
  };

  /*!
   * @brief Type reference wrapper for sequence types holding a specific type,
   * ie, Vector of Number
   */
  class SeqRef : public TypeRef
  {
    const TypeRef* seq_type;
    const TypeRef* child_type;

   public:
    SeqRef(const TypeRef* seq_type, const TypeRef* child_type, const std::string& name);

    bool is_type_of(const Value& val) const override;
    bool is_type_of(const AST::ASTNode& obj) const override;
    CoercionResult coerce(Context& ctx, sptr_val& val) const override;
  };

  namespace Type
  {
    inline const TypeRef LIST(Value::Type::LIST, Form::LIST, "List");
    inline const TypeRef VECTOR(Value::Type::VECTOR, Form::VECTOR, "Vector");
    inline const TypeRef BOOL(Value::Type::BOOL, Form::BOOLEAN, "Boolean");
    inline const TypeRef CHAR(Value::Type::CHAR, Form::CHAR, "Char");
    inline const TypeRef MAP(Value::Type::MAP, Form::MAP, "Map");
    inline const TypeRef STRING(Value::Type::STRING, Form::STRING, "String");
    inline const TypeRef NUMBER(Value::Type::NUMBER, Form::NUMBER, "Number");
    inline const TypeRef SYMBOL(Value::Type::SYMBOL, Form::SYMBOL, "Symbol");
    inline const TypeRef QUOTED_SYMBOL(Value::Type::SYMBOL,
                                       Form::QUOTED_SYMBOL,
                                       "QuotedSymbol");
    inline const TypeRef KEYWORD(Value::Type::KEYWORD, Form::KEYWORD, "Keyword");
    inline const TypeRef FUNCTION(Value::Type::FUNCTION, Form::FUNCTION, "Function");
    inline const TypeRef MACRO(Value::Type::FUNCTION, Form::MACRO, "Macro");
    inline const TypeRef HOST_OBJECT(Value::Type::OBJECT, Form::HOST_OBJECT, "HostObject");
    inline const TypeRef NATIVE_OBJECT(Value::Type::NATIVE_OBJECT,
                                       Form::HOST_OBJECT,
                                       "NativeObject");
    inline const TypeRef HOST_SEQ(Value::Type::OBJECT, Form::HOST_SEQ, "HostSeq");
    inline const TypeRef NIL(Value::Type::NIL, Form::NIL, "nil");

    inline const MultiRef QUALIFIABLE({&KEYWORD, &SYMBOL, &QUOTED_SYMBOL}, "Qualifiable");
    inline const MultiRef SYMBOL_VALUE({&SYMBOL, &QUOTED_SYMBOL}, "Symbol");
    inline const MultiRef STRING_OR_SYMBOL({&STRING, &SYMBOL_VALUE}, "String|Symbol");

    inline const MultiRef SEQ(std::vector<const TypeRef*>{&LIST, &VECTOR, &MAP, &HOST_SEQ},
                              "Seq");
    inline const MultiRef STRICT_SEQ(std::vector<const TypeRef*>{&LIST, &VECTOR, &HOST_SEQ},
                                     "Seq");
    inline const MultiRef SEQ_OR_STRING(std::vector<const TypeRef*>({&SEQ, &STRING}),
                                        "SeqLike");
    inline const MultiRef STRICT_SEQ_OR_STRING(std::vector<const TypeRef*>({&STRICT_SEQ,
                                                                            &STRING}),
                                               "SeqLike");
    inline const MultiRef EXEC(std::vector<const TypeRef*>{&FUNCTION, &MACRO, &KEYWORD},
                               "Exec");
    inline const MultiRef COMPLEX(std::vector<const TypeRef*>{&MAP,
                                                              &HOST_OBJECT,
                                                              &NATIVE_OBJECT},
                                  "Complex");

    inline const SeqRef VECTOR_OF_VECTOR(&VECTOR, &VECTOR, "[Vector]");
    inline const SeqRef VECTOR_OF_KEYWORD(&VECTOR, &KEYWORD, "[Keyword]");
    inline const SeqRef VECTOR_OF_FUNCTION(&VECTOR, &FUNCTION, "[Function]");
    inline const SeqRef VECTOR_OF_NUMBER(&VECTOR, &NUMBER, "[Number]");
    inline const SeqRef VECTOR_OF_CHAR(&VECTOR, &CHAR, "[Char]");
    inline const SeqRef VECTOR_OF_STRING(&VECTOR, &STRING, "[String]");
    inline const SeqRef VECTOR_OF_SYMBOL(&VECTOR, &SYMBOL, "[Symbol]");
    inline const SeqRef VECTOR_OF_SYMBOL_VALUE(&VECTOR, &SYMBOL_VALUE, "[Symbol]");
    inline const SeqRef VECTOR_OF_MAP(&VECTOR, &MAP, "[Map]");

    inline const AnyRef ANY;
  } // namespace Type
} // namespace Roo

#endif
