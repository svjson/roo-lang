#ifndef __LISP_FORM_H_
#define __LISP_FORM_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <lisple/exception.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

#define __ESCAPE(...) __VA_ARGS__

namespace Lisple
{
  class Context;
  class Number;
  class Key;

  extern const int INT_CONSTANTS_SIZE;
  extern std::vector<std::shared_ptr<Number>> INT_CONSTANTS;

  extern std::unordered_map<std::string, std::shared_ptr<Key>> key_intern_pool;

  /*!
   * @brief Abstract base class for all form implementations
   */
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

    /*!
     * @brief Check truthiness of the object/form.
     *
     * @return true for any non-nil or non-false value, otherwise false
     */
    virtual bool is_truthy() const;
    virtual bool has_key(const Object& key) const;
    virtual void set_property(const Object& key, sptr_sobject& value);
    virtual void set_property(const sptr_sobject& key, const sptr_sobject& value);
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
     * where the function would be a C++ or function that does the
     * conversion. This would also lift the responsibility from regular make
     * functions and remove the need for a lot of MultiRef constructs and type
     * checks there.
     */
    virtual void set_property(Context* ctx, const Object& key, sptr_sobject& value);

    virtual std::string to_string(int depth = -1) const = 0;

    virtual std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args);

    template <class OT> OT& as();

    template <class OT> const OT& as() const { return dynamic_cast<const OT&>(*this); }
  };

  class Nil : public Object
  {
   public:
    Nil();

    bool operator==(const Object&) const override;
    bool is_truthy() const override;

    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;

    std::string to_string(int depth = -1) const override;
  };

  inline std::shared_ptr<Object> NIL = std::make_shared<Nil>();

  template <typename T> class Value : public Object
  {
   public:
    T value;

    Value(Form type, T value)
      : Object(type)
      , value(value)
    {
    }

    static T value_of(const Object& obj);

    bool operator==(const Object& other) const override
    {
      if (this->type != other.get_type())
      {
        return false;
      }
      return Value<T>::value_of(other) == value;
    }
  };

  /*!
   * @brief Abstract intermediate class from string values that may use recognized
   * qualified, ie namespaced, identifiers, ie Keys and Words
   */
  class QualifiableStringValue : public Value<std::string>
  {
    /*!
     * @brief The namespace qualifier, if any. ie, "some.namespace" from
     * "some.namespace/an-identifier"
     */
    std::string ns_qualifier;
    /*!
     * @brief The actual identifier(word), ie, "an-identifier" from
     * "some.namespace/an-identifier"
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

    std::string to_string(int depth = -1) const override;
  };

  class String : public Value<std::string>
  {
   private:
    sptr_sobject_v _children;

   public:
    String(const std::string& value);

    std::string to_string(int depth = -1) const override;

    unsigned int size() const override;
    bool has_value(const std::string& value) const override;
    sptr_sobject_v& get_children() override;

    static std::shared_ptr<String> make(const std::string& value);
  };

  class Char : public Value<char>
  {
   public:
    Char(const char value);

    std::string to_string(int depth = -1) const override;

    static std::shared_ptr<Char> make(char value);
  };

  class Boolean : public Value<bool>
  {
   public:
    Boolean(const bool value);

    std::string to_string(int depth = -1) const override;

    bool is_truthy() const override;
    static std::shared_ptr<Boolean> wrap(bool value);
  };

  inline const std::shared_ptr<Boolean> B_TRUE = std::make_shared<Boolean>(true);
  inline const std::shared_ptr<Boolean> B_FALSE = std::make_shared<Boolean>(false);

  class Key : public QualifiableStringValue
  {
   public:
    Key(const std::string& value);

    bool has_value(const std::string& value) const override;

    std::string to_string(int depth = -1) const override;

    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;

    bool operator<(const Key& other) const;

    static std::shared_ptr<Key> make(const std::string& value);
  };

  enum class NumberType : uint8_t
  {
    INT,
    LONG,
    FLOAT
  };

  /*!
   * @brief The only numeric type of Lisple.
   *
   * Internally always represented as a float.
   *
   * FIXME: A better solution would be to have a class hierarchy here and
   * wrap the actual native types and leave the static factory methods on
   * this class, which would then be an abstract class.
   */
  class Number : public Value<float>
  {

   public:
    Number(int value);
    Number(long value);
    Number(unsigned int value);
    Number(unsigned long value);
    Number(float value);
    Number(double value);

    std::string to_string(int depth = -1) const override;

    NumberType num_type;
    int int_value() const;
    long long_value() const;
    float float_value() const;

    bool is_num_type(NumberType type) const;

    bool has_value(const int value) const override;
    bool has_value(const float value) const override;

    std::shared_ptr<Number> operator+(const Number& other);
    std::shared_ptr<Number> operator-(const Number& other);
    std::shared_ptr<Number> operator*(const Number& other);
    std::shared_ptr<Number> operator/(const Number& other);

    std::shared_ptr<Number> flip_sign();

    static std::shared_ptr<Number> make(int value);
    static std::shared_ptr<Number> make(unsigned long value);
    static std::shared_ptr<Number> make(long value);
    static std::shared_ptr<Number> make(float value);
    static std::shared_ptr<Number> make(double value);
    static std::shared_ptr<Number> make(unsigned int value);
    static std::shared_ptr<Number> make(const std::string& value);
  };

  class Word : public QualifiableStringValue
  {
   public:
    Word(const std::string& value);

    std::string to_string(int depth = -1) const override;

    bool has_value(const std::string& value) const override;

    static std::shared_ptr<Word> make(const std::string& value);
  };

  class QSymbol : public QualifiableStringValue
  {
   public:
    QSymbol(const std::string& value);

    std::string to_string(int depth = -1) const override;

    bool has_value(const std::string& value) const override;
  };

  class Seq : public Object
  {
   public:
    mutable sptr_sobject_v children;

    Seq(Form form, size_t reserved_size = 0);
    Seq(Form form, const sptr_sobject_v& children);

    static std::shared_ptr<Seq> new_sequence(Form type, size_t reserved_size = 0);

    std::string to_string(int depth = -1) const override;

    /*!
     * @brief Retrieves the first element of the Sequence
     */
    virtual std::shared_ptr<Object>& head();
    /*!
     * @brief Retrieves a new collection of all the elements of the Seq execpt
     * the first element. This is akin to car-operation, but as the underlying
     * type is not made up cons cells, this is an expensive operation for large
     * sequences.
     */
    virtual sptr_sobject_v tail();

    sptr_sobject get_sptr_property(const Object& form) const override;
    void set_property(const Object& key, sptr_sobject& value) override;

    void append(const sptr_sobject& child) override;
    unsigned int size() const override;
    sptr_sobject_v& get_children() override;

    virtual void replace_children(const sptr_sobject_v& vec);

    virtual const std::string lpar() const = 0;
    virtual const std::string rpar() const = 0;
    bool operator==(const Object& other) const override;
  };

  class List : public Seq
  {
    bool q;

   public:
    List(bool q = false, size_t reserved_size = 0);
    List(const sptr_sobject_v& children, bool q = false);

    static std::shared_ptr<List> from(sptr_sobject, sptr_sobject_v);
    static std::shared_ptr<List> make(const sptr_sobject_v& children);

    const std::string lpar() const override;
    const std::string rpar() const override;

    bool is_quoted() const;

    std::shared_ptr<List> insert(unsigned int index, sptr_sobject& value);

    std::shared_ptr<List> quot_toggle() const;

    std::shared_ptr<Object> execute(Context& ctx);
    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;
  };

  class Array : public Seq
  {
   public:
    Array(size_t reserved_size = 0);
    Array(const sptr_sobject_v& children);

    static std::shared_ptr<Array> make(const sptr_sobject_v& children);

    const std::string lpar() const override;
    const std::string rpar() const override;
  };

  class Map : public Seq
  {
    void validate_keys() const;

   public:
    Map(size_t reserved_size = 0);
    Map(const sptr_sobject_v& children);

    void set_property(const Object& key, sptr_sobject& value) override;
    void set_property(const sptr_sobject& key, const sptr_sobject& value) override;

    sptr_sobject remove_key(const Object& key);

    const std::string lpar() const override;
    const std::string rpar() const override;

    const std::vector<Object*> keys() const;
    sptr_sobject_v key_ptrs() const;

    unsigned int size() const override;

    static std::shared_ptr<Map> make(const sptr_sobject_v& children);

    bool has_key(const Object& key) const override;
  };

  class RuntimeValueWrapper : public Object
  {
   public:
    sptr_rtval val;
    sptr_sobject delegate;
    sptr_sobject_v child_memo;

    RuntimeValueWrapper(const sptr_rtval& val);

    bool operator==(const Object& other) const override;
    unsigned int size() const override;

    void append(const sptr_sobject& value) override;
    bool has_key(const Object&) const override;
    void set_property(const Object& key, sptr_sobject& value) override;
    void set_property(const sptr_sobject& key, const sptr_sobject& value) override;
    sptr_sobject get_sptr_property(const Object& key) const override;
    bool is_truthy() const override;

    sptr_sobject_v& get_children() override;

    std::shared_ptr<Object> execute(Context& ctx, sptr_sobject_v& args) override;

    static std::shared_ptr<Object> make(const sptr_rtval& value);

    std::string to_string(int depth = -1) const override;
  };

  template <class OT> OT& Object::as()
  {
    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(this))
    {
      return wrapper->delegate->as<OT>();
    }

    if (auto* self = dynamic_cast<OT*>(this)) return *self;

    throw LispleException("Unexpected AST node type: " + std::to_string((int)this->type));
  }

  template <typename T> T Value<T>::value_of(const Object& obj)
  {
    if (auto* lw = dynamic_cast<const RuntimeValueWrapper*>(&obj))
    {
      return Value<T>::value_of(*lw->delegate);
    }

    return dynamic_cast<const Value<T>&>(obj).value;
  }

} // namespace Lisple

#endif
