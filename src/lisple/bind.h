#ifndef LISPLE_BIND__H
#define LISPLE_BIND__H

#include "runtime/exec_node.h"

namespace Lisple
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

    virtual void apply(Scope& scope, const sptr_rtval& value) const = 0;

    static std::unique_ptr<LexicalBinding> create(LiteralNode& pattern);
    static std::unique_ptr<LexicalBinding> create(sptr_rtval& pattern);
  };

  class SymbolBinding : public LexicalBinding
  {
    std::string symbol;

   public:
    SymbolBinding(const std::string& symbol);

    void apply(Scope& scope, const sptr_rtval& value_expr) const override;
  };

  class MapDestructureBinding : public LexicalBinding
  {
    std::unique_ptr<SymbolBinding> map_symbol;
    std::vector<std::tuple<RTValue, std::unique_ptr<LexicalBinding>>> bindings;

   public:
    MapDestructureBinding(const sptr_rtval_v& map);

    void apply(Scope& scope, const sptr_rtval& value_expr) const override;
  };

  class VectorDestructureBinding : public LexicalBinding
  {
    std::vector<std::unique_ptr<LexicalBinding>> bindings;

   public:
    VectorDestructureBinding(const sptr_rtval_v& vector);

    void apply(Scope& scope, const sptr_rtval& value_expr) const override;
  };

} // namespace Lisple

#endif /* LISPLE_BIND__H */
