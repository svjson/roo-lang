#include "bind.h"

#include "exception.h"
#include "scope.h"

#include "lisple/form.h"

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

  std::unique_ptr<LexicalBinding> LexicalBinding::create(sptr_rtval& pattern)
  {
    if (pattern->type == RTValue::Type::SYMBOL)
    {
      return std::make_unique<SymbolBinding>(std::get<std::string>(pattern->value));
    }
    else if (pattern->type == RTValue::Type::MAP)
    {
      return std::make_unique<MapDestructureBinding>(std::get<sptr_rtval_v>(pattern->value));
    }
    else if (pattern->type == RTValue::Type::VECTOR)
    {
      return std::make_unique<VectorDestructureBinding>(
        std::get<sptr_rtval_v>(pattern->value));
    }
    else
    {
      throw LispleException("Invalid bind pattern: " + pattern->to_string());
    }
  }

  /** SymbolBinding */
  SymbolBinding::SymbolBinding(const std::string& symbol)
    : symbol(symbol)
  {
  }

  void SymbolBinding::apply(Scope& scope, const sptr_rtval& value_expr) const
  {
    scope.store(symbol, value_expr);
  }

  /** MapDestructureBinding */
  MapDestructureBinding::MapDestructureBinding(const sptr_rtval_v& map_data)
  {
    auto [_k, keys] = map_entry(map_data, *RTValue::keyword("keys"));
    auto [_vs, as_symbol] = map_entry(map_data, *RTValue::keyword("as"));

    if (keys == nullptr || keys->type != RTValue::Type::VECTOR || map_data.size() > 4 ||
        (map_data.size() == 4 && as_symbol == nullptr))
    {
      throw TypeError("Invalid map destructure form: " +
                      Lisple::RTValue::map(map_data)->to_string());
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

  void MapDestructureBinding::apply(Scope& scope, const sptr_rtval& value) const
  {
    sptr_rtval_v& map = std::get<sptr_rtval_v>(value->value);
    for (auto& [key, binding] : bindings)
    {
      auto [_, val] = map_entry(map, key);

      if (val)
      {
        binding->apply(scope, val);
      }
      else
      {
        // FIXME: Bind nil
      }
    }

    if (this->map_symbol)
    {
      this->map_symbol->apply(scope, value);
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

  void VectorDestructureBinding::apply(Scope& scope, const sptr_rtval& vector_expr) const
  {
    sptr_rtval_v& vec = std::get<sptr_rtval_v>(vector_expr->value);
    sptr_sobject form = std::make_shared<RuntimeValueWrapper>(RTValue::vector(vec));

    for (size_t i = 0; i < this->bindings.size(); i++)
    {
      this->bindings[i]->apply(scope, vec[i]);
    }
  }

} // namespace Lisple
