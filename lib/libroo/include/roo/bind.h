#ifndef ROO_BIND__H
#define ROO_BIND__H

#include <roo/runtime/node.h>

namespace Roo
{
  class Scope;

  /**
   * @brief Base class for variable bindings in function definitions
   * and let expressions.
   *
   * Derived types support both simple variable bindings and
   * destructuring bindings.
   *
   * In function definitions, bindings are applied to the function
   * scope, and values are taken from the arguments passed to the
   * function. In let expressions, bindings are applied to the let
   * body scope, and values are taken from the evaluated binding
   * expressions.
   *
   * Constructed through the static create() method, which inspects
   * the LiteralNode pattern.
   *
   * At runtime, the binding is applied to a scope via the apply()
   * method, which takes the scope to apply the binding to, and a
   * LiteralNode.
   */
  class LexicalBinding
  {
   public:
    virtual ~LexicalBinding() = default;

    virtual void apply(Scope& scope, const sptr_val& value) const = 0;

    static std::unique_ptr<LexicalBinding> create(LiteralNode& pattern);
    static std::unique_ptr<LexicalBinding> create(sptr_val& pattern);

    virtual std::unique_ptr<LexicalBinding> clone() const = 0;
  };

  class SymbolBinding : public LexicalBinding
  {
    std::string symbol;

   public:
    SymbolBinding(const std::string& symbol);
    SymbolBinding(const SymbolBinding& other);

    void apply(Scope& scope, const sptr_val& value_expr) const override;

    std::unique_ptr<LexicalBinding> clone() const override;
  };

  class MapDestructureBinding : public LexicalBinding
  {
    std::unique_ptr<SymbolBinding> map_symbol;
    std::vector<std::tuple<Value, std::unique_ptr<LexicalBinding>>> bindings;

   public:
    MapDestructureBinding(const sptr_val_v& map);
    MapDestructureBinding(const MapDestructureBinding& other);

    void apply(Scope& scope, const sptr_val& value_expr) const override;

    std::unique_ptr<LexicalBinding> clone() const override;
  };

  class VectorDestructureBinding : public LexicalBinding
  {
    std::vector<std::unique_ptr<LexicalBinding>> bindings;

   public:
    VectorDestructureBinding(const sptr_val_v& vector);
    VectorDestructureBinding(const VectorDestructureBinding& other);

    void apply(Scope& scope, const sptr_val& value_expr) const override;

    std::unique_ptr<LexicalBinding> clone() const override;
  };

  /**
   * @brief Binding for rest parameters - collects a slice of arguments into a vector
   * and binds it to a named symbol.
   *
   * Not a LexicalBinding subclass because it takes a vector of values rather
   * than a single value.
   */
  class RestBinding
  {
    std::string symbol;

   public:
    RestBinding(const std::string& symbol);
    RestBinding(const RestBinding& other);

    void apply(Scope& scope, const sptr_val_v& values) const;

    std::unique_ptr<RestBinding> clone() const;
  };

} // namespace Roo

#endif /* ROO_BIND__H */
