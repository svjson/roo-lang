
#ifndef __LISPLE_TYPE_H_
#define __LISPLE_TYPE_H_

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
    DISCARD
  };

  class Context;
  class Object;

  typedef std::shared_ptr<Lisple::Object> sptr_sobject;
  typedef std::vector<std::shared_ptr<Lisple::Object>> sptr_sobject_v;

  struct CoercionResult
  {
    bool success;
    sptr_sobject result;
  };

  /*! @brief Basic type reference and identifier for built-in Lisple types */
  class TypeRef
  {
   protected:
    const Lisple::Form form_type;
    const std::string name;

   public:
    TypeRef(Lisple::Form form_type, const std::string& name);
    virtual ~TypeRef() = default;

    virtual bool is_type_of(const Lisple::Object& obj) const;
    virtual CoercionResult coerce(Context& ctx, sptr_sobject& obj) const;

    const std::string& to_string() const;

    friend class SeqRef;
  };

  /*! @brief Type reference wrapper for when two or more types are acceptable */
  class MultiRef : public TypeRef
  {
    std::vector<const Lisple::TypeRef*> types;

   public:
    MultiRef(std::vector<const Lisple::TypeRef*> types, const std::string& name);

    bool is_type_of(const Lisple::Object& obj) const override;
  };

  /*! @brief Type reference for when any type is acceptable */
  class AnyRef : public TypeRef
  {
   public:
    AnyRef();
    bool is_type_of(const Object& obj) const override;
  };

  /*! @brief Type reference wrapper for sequence types holding a specific type,
  *   ie, Array of Number */
  class SeqRef : public TypeRef
  {
    const TypeRef* seq_type;
    const TypeRef* child_type;

   public:
    SeqRef(const TypeRef* seq_type, const TypeRef* child_type, const std::string& name);

    bool is_type_of(const Object& obj) const override;
    CoercionResult coerce(Context& ctx, sptr_sobject& obj) const override;
  };

  namespace Type
  {
    inline const TypeRef LIST(Form::LIST, "List");
    inline const TypeRef ARRAY(Form::ARRAY, "Vector");
    inline const TypeRef BOOL(Form::BOOLEAN, "Boolean");
    inline const TypeRef CHAR(Form::CHAR, "Char");
    inline const TypeRef MAP(Form::MAP, "Map");
    inline const TypeRef STRING(Form::STRING, "String");
    inline const TypeRef NUMBER(Form::NUMBER, "Number");
    inline const TypeRef SYMBOL(Form::SYMBOL, "Symbol");
    inline const TypeRef WORD(Form::WORD, "Word");
    inline const TypeRef KEY(Form::KEY, "Keyword");
    inline const TypeRef FUNCTION(Form::FUNCTION, "Function");
    inline const TypeRef MACRO(Form::MACRO, "Macro");
    inline const TypeRef HOST_OBJECT(Form::HOST_OBJECT, "HostObject");

    inline const SeqRef ARRAY_OF_ARRAY(&ARRAY, &ARRAY, "[Array]");
    inline const SeqRef ARRAY_OF_KEY(&ARRAY, &KEY, "[Key]");
    inline const SeqRef ARRAY_OF_FUNCTION(&ARRAY, &FUNCTION, "[Function]");
    inline const SeqRef ARRAY_OF_NUMBER(&ARRAY, &NUMBER, "[Number]");
    inline const SeqRef ARRAY_OF_CHAR(&ARRAY, &CHAR, "[Char]");
    inline const SeqRef ARRAY_OF_STRING(&ARRAY, &STRING, "[String]");
    inline const SeqRef ARRAY_OF_SYMBOL(&ARRAY, &SYMBOL, "[Symbol]");
    inline const SeqRef ARRAY_OF_MAP(&ARRAY, &SYMBOL, "[Map]");

    inline const MultiRef SEQ(std::vector<const TypeRef*>{ &LIST, &ARRAY, &MAP }, "Seq");
    inline const MultiRef EXEC(std::vector<const TypeRef*>{ &FUNCTION, &KEY }, "Exec");
    inline const MultiRef COMPLEX(std::vector<const TypeRef*>{ &MAP, &HOST_OBJECT}, "Complex");

    inline const AnyRef ANY;
  }
}

#endif
