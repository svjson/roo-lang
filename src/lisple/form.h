#ifndef __LISP_FORM_H_
#define __LISP_FORM_H_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "type.h"

namespace Lisple
{
  class Context;

  class Object
  {
   protected:
    Form type;

   public:
    Object(Form form);
    virtual ~Object() = default;

    Form get_type() const;

    virtual bool has_value(const std::string& value) const;
    virtual bool has_value(const int value) const;
    virtual bool has_value(const float value) const;

    virtual void append(const sptr_sobject& child);
    virtual sptr_sobject_v& get_children();
    virtual unsigned int size() const;

    virtual bool operator==(const Object& other) const = 0;
    bool operator!=(const Object& other) const;
    virtual Object& get_property(const Object& key) const;
    virtual sptr_sobject get_sptr_property(const Object& key) const;

    virtual bool has_key(const Object& key) const;
    virtual void set_property(const Object& key, sptr_sobject& value);
    /**
     * TODO: This exists because of the need to use the Context in some
     * HostObject setters.
     * The only known use case, so far, is to be able to use
     * make-functions to coerce maps into HostObjects.
     *
     * Technically, there _could_ also be a need to call other Lisple
     * functions as part of complex custom setters, but that's an edge case.
     *
     * A better solution is to implement support for coercion, ie, if a
     * expects a complex type(HostObjectAdapter) but gets something else,
     * then see if there is support from transformation.
     *
     * A simplistic solution would be to add a registry/map of type->function
     * where the function would be a C++ or Lisple::function that does the
     * conversion. This would also lift the responsibility from regular make
     * functions and remove the need for a lot of MultiRef constructs and type
     * checks there.
     */
    virtual void set_property(Context* ctx, const Object& key, sptr_sobject& value);

    virtual std::string to_string() const = 0;


    virtual std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args);

    template<class OT> OT& as()
    {
      return dynamic_cast<OT&>(*this);
    }

    template<class OT> const OT& as() const
    {
      return dynamic_cast<const OT&>(*this);
    }

  };

  class Nil : public Object
  {
   public:
    Nil();

    bool operator==(const Object&) const override;

    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;

    std::string to_string() const override;
  };

  inline std::shared_ptr<Object> NIL = std::make_shared<Nil>();

  template <typename T>
  class Value : public Object
  {
   public:
    T value;

    Value(Form type, T value)
      : Object(type)
      , value(value)
    {
    }

    static T value_of(const Lisple::Object& obj)
    {
      return dynamic_cast<const Value<T>&>(obj).value;
    }

    bool operator==(const Object& other) const override
    {
      if (this->type != other.get_type())
        {
          return false;
        }
      auto& other_val = dynamic_cast<const Value<T>&>(other);
      return other_val.value == value;
    }
  };

  class QualifiableStringValue : public Value<std::string>
  {
    /*
     * The namespace qualifier, if any. ie, "some.namespace" from "some.namespace/an-identifier"
     */
    std::string ns_qualifier;
    /*
     * The actual identifier(word), ie, "an-identifier" from "some.namespace/an-identifier"
     */
    std::string identifier;

  public:
    QualifiableStringValue(Form type, const std::string& value);

    bool is_qualified() const;
    const std::string& get_qualifier() const;
    const std::string& get_identifier() const;
  };

  class Discard : public Object
  {
    std::shared_ptr<Object> value;

   public:
    Discard();

    bool operator==(const Object&) const override;

    void append(const sptr_sobject& value) override;

    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;

    std::string to_string() const override;
  };

  class String : public Value<std::string>
  {
   public:
    String(const std::string& value);

    std::string to_string() const override;

    bool has_value(const std::string& value) const override;
  };

  class Char : public Value<char>
  {
  public:
    Char(const char value);

    std::string to_string() const override;
  };

  class Boolean : public Value<bool>
  {
   public:
    Boolean(const bool value);

    std::string to_string() const override;
  };

  inline const std::shared_ptr<Lisple::Boolean> B_TRUE = std::make_shared<Lisple::Boolean>(true);
  inline const std::shared_ptr<Lisple::Boolean> B_FALSE = std::make_shared<Lisple::Boolean>(false);

  class Key : public QualifiableStringValue
  {
   public:
    Key(const std::string& value);

    bool has_value(const std::string& value) const override;

    std::string to_string() const override;

    std::shared_ptr<Lisple::Object> execute(Context& ctx, sptr_sobject_v& args) override;

    bool operator<(const Key& other) const;
  };

  enum class NumberType : uint8_t
  {
    INT,
    FLOAT
  };

  class Number : public Value<float>
  {
    NumberType num_type;

   public:
    Number(int value);
    Number(float value);
    Number(const std::string& value);

    std::string to_string() const override;

    int int_value() const;
    float float_value() const;

    bool is_num_type(NumberType type) const;

    bool has_value(const int value) const override;
    bool has_value(const float value) const override;
  };

  class Word : public QualifiableStringValue
  {
   public:
    Word(const std::string& value);

    std::string to_string() const override;

    bool has_value(const std::string& value) const override;
  };

  class QSymbol : public Value<std::string>
  {
   public:
    QSymbol(const std::string& value);

    std::string to_string() const override;

    bool has_value(const std::string& value) const override;
  };

  class Sexpression : public Object
  {
   public:
    sptr_sobject_v children;

    Sexpression(Form form);
    Sexpression(Form form, sptr_sobject_v children);

    static std::shared_ptr<Sexpression> new_sequence(Form type);

    std::string to_string() const override;

    std::shared_ptr<Object>& head();
    sptr_sobject_v tail();

    sptr_sobject get_sptr_property(const Object& form) const override;
    void set_property(const Object& key, sptr_sobject& value) override;

    void append(const sptr_sobject& child) override;
    unsigned int size() const override;
    sptr_sobject_v& get_children() override;
    virtual const std::string lpar() const = 0;
    virtual const std::string rpar() const = 0;
    bool operator==(const Object& other) const override;
  };

  class List : public Sexpression
  {
    bool q;

   public:
    List(bool q=false);
    List(sptr_sobject_v children, bool q=false);

    static std::shared_ptr<List> from(sptr_sobject, sptr_sobject_v);

    const std::string lpar() const override;
    const std::string rpar() const override;

    bool is_quoted() const;

    std::shared_ptr<List> insert(unsigned int index, sptr_sobject& value);

    std::shared_ptr<List> quot_toggle() const;

    std::shared_ptr<Object> execute(Lisple::Context& ctx);
    std::shared_ptr<Object> execute(Lisple::Context& ctx, sptr_sobject_v& args) override;
  };

  class Array : public Sexpression
  {
   public:
    Array();
    Array(sptr_sobject_v children);

    const std::string lpar() const override;
    const std::string rpar() const override;
  };

  class Map : public Sexpression
  {
    void validate_keys() const;

   public:
    Map();
    Map(sptr_sobject_v children);

    void set_property(const Object& key, sptr_sobject& value) override;
    void set_property(const sptr_sobject& key, const sptr_sobject& value);

    const std::string lpar() const override;
    const std::string rpar() const override;

    const std::vector<Object*> keys() const;
    Lisple::sptr_sobject_v key_ptrs() const;

    bool has_key(const Object& key) const override;
  };

}

#endif
