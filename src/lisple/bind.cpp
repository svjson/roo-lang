#include "bind.h"

#include "exception.h"
#include "scope.h"

namespace Lisple
{

  /** LexicalBinding */
  std::unique_ptr<LexicalBinding> LexicalBinding::create(LiteralNode& pattern)
  {
    if (pattern.value->type == RTValue::Type::SYMBOL)
    {
      return std::make_unique<SymbolBinding>(std::get<std::string>(pattern.value->value));
    }
    else if (pattern.value->type == RTValue::Type::MAP)
    {
      return std::make_unique<MapDestructureBinding>(
        std::get<sptr_rtval_v>(pattern.value->value));
    }
    else if (pattern.value->type == RTValue::Type::VECTOR)
    {
      return std::make_unique<VectorDestructureBinding>(
        std::get<sptr_rtval_v>(pattern.value->value));
    }
    else
    {
      throw LispleException("Invalid bind pattern: " + pattern.ast_node->to_string());
    }
  }

  /** SymbolBinding */
  SymbolBinding::SymbolBinding(const std::string& symbol)
    : symbol(symbol)
  {
  }

  void SymbolBinding::apply(Scope& scope, LiteralNode& value_expr) const
  {
    scope.store(symbol, value_expr.ast_node);
  }

  /** MapDestructureBinding */
  MapDestructureBinding::MapDestructureBinding(const sptr_rtval_v& map_data)
  {
    auto [_k, keys] = map_entry(map_data, *RTValue::keyword("keys"));
    auto [_vs, as_symbol] = map_entry(map_data, *RTValue::keyword("as"));

    if (keys == nullptr || keys->type != RTValue::Type::VECTOR || map_data.size() > 4 ||
        (map_data.size() == 4 && as_symbol == nullptr))
    {
      throw TypeError("Invalid map destructure form.");
    }

    for (auto& symbol : std::get<sptr_rtval_v>(keys->value))
    {
      if (symbol->type == RTValue::Type::SYMBOL)
      {
        bindings.push_back(std::make_pair(
          *RTValue::keyword(std::get<std::string>(symbol->value)),
          std::make_unique<SymbolBinding>(std::get<std::string>(symbol->value))));
      }
      else
      {
        throw TypeError("Binding not valid in binding form");
      }
    }

    if (as_symbol != nullptr)
    {
      this->map_symbol =
        std::make_unique<SymbolBinding>(std::get<std::string>(as_symbol->value));
    }
    else
    {
      this->map_symbol = nullptr;
    }
  }

  void MapDestructureBinding::apply(Scope& scope, LiteralNode& node) const
  {
    sptr_rtval_v& map = std::get<sptr_rtval_v>(node.value->value);
    for (auto& [key, binding] : bindings)
    {
      auto [_, val] = map_entry(map, key);

      if (val)
      {
        sptr_sobject lkey = Lisple::Key::make(std::get<std::string>(key.value));
        sptr_sobject lval = node.ast_node->get_sptr_property(*lkey);

        auto lit_node = LiteralNode(val, lval);
        binding->apply(scope, lit_node);
      }
      else
      {
        // FIXME: Bind nil
      }
    }

    if (this->map_symbol)
    {
      this->map_symbol->apply(scope, node);
    }
  }

  /** VectorDestructureBinding */
  VectorDestructureBinding::VectorDestructureBinding(const sptr_rtval_v& vector)
  {
    for (auto sym : vector)
    {
      if (sym->type == RTValue::Type::SYMBOL)
      {
        this->bindings.push_back(
          std::make_unique<SymbolBinding>(std::get<std::string>(sym->value)));
      }
      else
      {
        throw LispleException("Non-symbol bindings not supported in vector destructuring");
      }
    }
  }

  void VectorDestructureBinding::apply(Scope& scope, LiteralNode& vector_expr) const
  {
    sptr_rtval_v& vec = std::get<sptr_rtval_v>(vector_expr.value->value);

    for (size_t i = 0; i < this->bindings.size(); i++)
    {
      LiteralNode val_node = LiteralNode(vec[i], vector_expr.ast_node->get_children()[i]);
      this->bindings[i]->apply(scope, val_node);
    }
  }

} // namespace Lisple
