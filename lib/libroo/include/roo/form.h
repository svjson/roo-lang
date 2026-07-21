#ifndef __LISP_FORM_H_
#define __LISP_FORM_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <roo/exception.h>
#include <roo/runtime/value.h>
#include <roo/source.h>
#include <roo/type.h>

#define __ESCAPE(...) __VA_ARGS__

namespace Roo
{
  class Context;

  namespace AST
  {
    class Number;
    class Keyword;

    extern const int INT_CONSTANTS_SIZE;
    extern std::vector<std::shared_ptr<Number>> INT_CONSTANTS;

    extern std::unordered_map<std::string, std::shared_ptr<Keyword>> key_intern_pool;

    /*!
     * @brief Abstract base class for all form implementations
     */
    class ASTNode
    {
     protected:
      Form type;
      SourceRef source;

     public:
      ASTNode(Form form);
      virtual ~ASTNode() = default;

      Form get_type() const;
      const SourceRef& get_source() const;
      void set_source(const SourceRef& source);

      virtual bool has_value(const std::string& value) const;
      virtual bool has_value(const int value) const;
      virtual bool has_value(const float value) const;

      virtual void append(const sptr_ast_node& child);
      virtual sptr_ast_node_v& get_children();
      virtual unsigned int size() const;

      virtual bool operator==(const ASTNode& other) const = 0;
      bool operator!=(const ASTNode& other) const;
      virtual ASTNode& get_property(const ASTNode& key) const;
      virtual sptr_ast_node get_sptr_property(const ASTNode& key) const;

      /*!
       * @brief Check truthiness of the object/form.
       *
       * @return true for any non-nil or non-false value, otherwise false
       */
      virtual bool is_truthy() const;
      virtual bool has_key(const ASTNode& key) const;
      virtual void set_property(const ASTNode& key, sptr_ast_node& value);
      virtual void set_property(const sptr_ast_node& key, const sptr_ast_node& value);
      /**
       * TODO: This exists because of the need to use the Context in some
       * HostObject setters.
       * The only known use case, so far, is to be able to use
       * make-functions to coerce maps into HostObjects.
       *
       * Technically, there _could_ also be a need to call other Roo
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
      virtual void set_property(Context* ctx, const ASTNode& key, sptr_ast_node& value);

      virtual std::string to_string(int depth = -1) const = 0;

      template <class OT> OT& as();

      template <class OT> const OT& as() const { return dynamic_cast<const OT&>(*this); }
    };

    class Nil : public ASTNode
    {
     public:
      Nil();

      bool operator==(const ASTNode&) const override;
      bool is_truthy() const override;

      std::string to_string(int depth = -1) const override;
    };

    inline std::shared_ptr<ASTNode> NIL = std::make_shared<Nil>();

    template <typename T> class Value : public ASTNode
    {
     public:
      T value;

      Value(Form type, T value)
        : ASTNode(type)
        , value(value)
      {
      }

      static T value_of(const ASTNode& obj);

      bool operator==(const ASTNode& other) const override
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
     * qualified, ie namespaced, identifiers, ie Keywords and Symbols
     */
    class QualifiableStringValue : public Value<std::string>
    {
      /*!
       * @brief The namespace qualifier, if any. ie, "some.namespace" from
       * "some.namespace/an-identifier"
       */
      std::string ns_qualifier;
      /*!
       * @brief The actual identifier(symbol), ie, "an-identifier" from
       * "some.namespace/an-identifier"
       */
      std::string identifier;

     public:
      QualifiableStringValue(Form type, const std::string& value);

      bool is_qualified() const;
      const std::string& get_qualifier() const;
      const std::string& get_identifier() const;
    };

    class Discard : public ASTNode
    {
      std::shared_ptr<ASTNode> value;

     public:
      Discard();

      bool operator==(const ASTNode&) const override;

      void append(const sptr_ast_node& value) override;

      std::string to_string(int depth = -1) const override;
    };

    class String : public Value<std::string>
    {
     private:
      sptr_ast_node_v _children;

     public:
      String(const std::string& value);

      std::string to_string(int depth = -1) const override;

      unsigned int size() const override;
      bool has_value(const std::string& value) const override;
      sptr_ast_node_v& get_children() override;

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

    class Keyword : public QualifiableStringValue
    {
     public:
      Keyword(const std::string& value);

      bool has_value(const std::string& value) const override;

      std::string to_string(int depth = -1) const override;

      bool operator<(const Keyword& other) const;

      static std::shared_ptr<Keyword> make(const std::string& value);
    };

    enum class NumberType : uint8_t
    {
      INT,
      LONG,
      FLOAT
    };

    /*!
     * @brief The only numeric type of Roo.
     *
     * Internally always represented as a float.
     *
     * FIXME: A better solution would be to have a class hierarchy here and
     * wrap the actual native types and leave the static factory methods on
     * this class, which would then be an abstract class.
     */
    class Number : public Value<float>
    {
      union
      {
        int i32val;
        long long i64val;
        float f32val;
      };

     public:
      Number(int value);
      Number(long value);
      Number(long long value);
      Number(unsigned int value);
      Number(unsigned long value);
      Number(float value);
      Number(double value);

      std::string to_string(int depth = -1) const override;

      NumberType num_type;
      int int_value() const;
      long long long_value() const;
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
      static std::shared_ptr<Number> make(long long value);
      static std::shared_ptr<Number> make(float value);
      static std::shared_ptr<Number> make(double value);
      static std::shared_ptr<Number> make(unsigned int value);
      static std::shared_ptr<Number> make(const std::string& value);
    };

    class Symbol : public QualifiableStringValue
    {
     public:
      Symbol(const std::string& value);

      std::string to_string(int depth = -1) const override;

      bool has_value(const std::string& value) const override;

      static std::shared_ptr<Symbol> make(const std::string& value);
    };

    class QuotedSymbol : public QualifiableStringValue
    {
     public:
      QuotedSymbol(const std::string& value);

      std::string to_string(int depth = -1) const override;

      bool has_value(const std::string& value) const override;
    };

    class Seq : public ASTNode
    {
     public:
      mutable sptr_ast_node_v children;

      Seq(Form form, size_t reserved_size = 0);
      Seq(Form form, const sptr_ast_node_v& children);

      static std::shared_ptr<Seq> new_sequence(Form type, size_t reserved_size = 0);

      std::string to_string(int depth = -1) const override;

      /*!
       * @brief Retrieves the first element of the Sequence
       */
      virtual std::shared_ptr<ASTNode>& head();
      /*!
       * @brief Retrieves a new collection of all the elements of the Seq execpt
       * the first element. This is akin to car-operation, but as the underlying
       * type is not made up cons cells, this is an expensive operation for large
       * sequences.
       */
      virtual sptr_ast_node_v tail();

      sptr_ast_node get_sptr_property(const ASTNode& form) const override;
      void set_property(const ASTNode& key, sptr_ast_node& value) override;

      void append(const sptr_ast_node& child) override;
      unsigned int size() const override;
      sptr_ast_node_v& get_children() override;

      virtual void replace_children(const sptr_ast_node_v& vec);

      virtual const std::string lpar() const = 0;
      virtual const std::string rpar() const = 0;
      bool operator==(const ASTNode& other) const override;
    };

    class List : public Seq
    {
      bool q;

     public:
      List(bool q = false, size_t reserved_size = 0);
      List(const sptr_ast_node_v& children, bool q = false);

      static std::shared_ptr<List> from(sptr_ast_node, sptr_ast_node_v);
      static std::shared_ptr<List> make(const sptr_ast_node_v& children);

      const std::string lpar() const override;
      const std::string rpar() const override;

      bool is_quoted() const;

      std::shared_ptr<List> insert(unsigned int index, sptr_ast_node& value);

      std::shared_ptr<List> quot_toggle() const;
    };

    class Vector : public Seq
    {
     public:
      Vector(size_t reserved_size = 0);
      Vector(const sptr_ast_node_v& children);

      static std::shared_ptr<Vector> make(const sptr_ast_node_v& children);

      const std::string lpar() const override;
      const std::string rpar() const override;
    };

    class Map : public Seq
    {
      void validate_keys() const;

     public:
      Map(size_t reserved_size = 0);
      Map(const sptr_ast_node_v& children);

      void set_property(const ASTNode& key, sptr_ast_node& value) override;
      void set_property(const sptr_ast_node& key, const sptr_ast_node& value) override;

      sptr_ast_node remove_key(const ASTNode& key);

      const std::string lpar() const override;
      const std::string rpar() const override;

      const std::vector<ASTNode*> keys() const;
      sptr_ast_node_v key_ptrs() const;

      unsigned int size() const override;

      static std::shared_ptr<Map> make(const sptr_ast_node_v& children);

      bool has_key(const ASTNode& key) const override;
    };

    class RuntimeValueWrapper : public ASTNode
    {
     public:
      sptr_val val;
      sptr_ast_node delegate;
      sptr_ast_node_v child_memo;

      RuntimeValueWrapper(const sptr_val& val);

      bool operator==(const ASTNode& other) const override;
      unsigned int size() const override;

      void append(const sptr_ast_node& value) override;
      bool has_key(const ASTNode&) const override;
      void set_property(const ASTNode& key, sptr_ast_node& value) override;
      void set_property(const sptr_ast_node& key, const sptr_ast_node& value) override;
      sptr_ast_node get_sptr_property(const ASTNode& key) const override;
      bool is_truthy() const override;

      sptr_ast_node_v& get_children() override;

      static std::shared_ptr<ASTNode> make(const sptr_val& value);

      std::string to_string(int depth = -1) const override;
    };

    template <class OT> OT& ASTNode::as()
    {
      if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(this))
      {
        return wrapper->delegate->as<OT>();
      }

      if (auto* self = dynamic_cast<OT*>(this)) return *self;

      throw RooException("::as - Unexpected AST node type: " +
                         std::to_string((int)this->type));
    }

    template <typename T> T Value<T>::value_of(const ASTNode& obj)
    {
      if (auto* lw = dynamic_cast<const RuntimeValueWrapper*>(&obj))
      {
        return Value<T>::value_of(*lw->delegate);
      }

      return dynamic_cast<const Value<T>&>(obj).value;
    }

  } // namespace AST
} // namespace Roo

#endif
