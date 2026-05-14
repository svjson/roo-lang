#include "lisple/bind.h"

#include <memory>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/runtime/dict.h>
#include <lisple/scope.h>

namespace Lisple
{

  /** LexicalBinding */
  std::unique_ptr<LexicalBinding> LexicalBinding::create(LiteralNode& pattern)
  {
    if (pattern.value->type == Value::Type::SYMBOL)
    {
      return std::make_unique<SymbolBinding>(std::get<std::string>(pattern.value->value));
    }
    else if (pattern.value->type == Value::Type::MAP)
    {
      return std::make_unique<MapDestructureBinding>(
        std::get<sptr_val_v>(pattern.value->value));
    }
    else if (pattern.value->type == Value::Type::VECTOR)
    {
      return std::make_unique<VectorDestructureBinding>(
        std::get<sptr_val_v>(pattern.value->value));
    }
    else
    {
      throw LispleException("Invalid bind pattern: " + pattern.ast_node->to_string());
    }
  }

  std::unique_ptr<LexicalBinding> LexicalBinding::create(sptr_val& pattern)
  {
    if (pattern->type == Value::Type::SYMBOL)
    {
      return std::make_unique<SymbolBinding>(std::get<std::string>(pattern->value));
    }
    else if (pattern->type == Value::Type::MAP)
    {
      return std::make_unique<MapDestructureBinding>(std::get<sptr_val_v>(pattern->value));
    }
    else if (pattern->type == Value::Type::VECTOR)
    {
      return std::make_unique<VectorDestructureBinding>(
        std::get<sptr_val_v>(pattern->value));
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

  SymbolBinding::SymbolBinding(const SymbolBinding& other)
    : SymbolBinding(other.symbol)
  {
  }

  void SymbolBinding::apply(Scope& scope, const sptr_val& value_expr) const
  {
    scope.store(symbol, value_expr);
  }

  std::unique_ptr<LexicalBinding> SymbolBinding::clone() const
  {
    return std::make_unique<SymbolBinding>(symbol);
  }

  /** MapDestructureBinding */
  MapDestructureBinding::MapDestructureBinding(const sptr_val_v& map_data)
  {
    auto [_k, keys] = Dict::map_entry(map_data, *Value::keyword("keys"));
    auto [_vs, as_symbol] = Dict::map_entry(map_data, *Value::keyword("as"));

    if (keys == nullptr || keys->type != Value::Type::VECTOR || map_data.size() > 4 ||
        (map_data.size() == 4 && as_symbol == nullptr))
    {
      throw TypeError("Invalid map destructure form: " +
                      Lisple::Value::map(map_data)->to_string());
    }

    for (auto& symbol : std::get<sptr_val_v>(keys->value))
    {
      if (symbol->type == Value::Type::SYMBOL)
      {
        bindings.push_back(std::make_pair(
          *Value::keyword(std::get<std::string>(symbol->value)),
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

  MapDestructureBinding::MapDestructureBinding(const MapDestructureBinding& other)
    : map_symbol(other.map_symbol ? std::make_unique<SymbolBinding>(*other.map_symbol)
                                  : nullptr)
  {
    for (auto& [s, v] : other.bindings)
    {
      bindings.push_back(std::make_pair(*Value::keyword(s.str()), v->clone()));
    }
  }

  void MapDestructureBinding::apply(Scope& scope, const sptr_val& value) const
  {
    for (auto& [key, binding] : bindings)
    {
      auto val = Dict::get_property(value, key);

      if (val)
      {
        binding->apply(scope, val);
      }
      else
      {
        binding->apply(scope, Constant::NIL);
      }
    }

    if (this->map_symbol)
    {
      this->map_symbol->apply(scope, value);
    }
  }

  std::unique_ptr<LexicalBinding> MapDestructureBinding::clone() const
  {
    return std::make_unique<MapDestructureBinding>(*this);
  }

  /** VectorDestructureBinding */
  VectorDestructureBinding::VectorDestructureBinding(const sptr_val_v& vector)
  {
    for (auto sym : vector)
    {
      if (sym->type == Value::Type::SYMBOL)
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

  VectorDestructureBinding::VectorDestructureBinding(const VectorDestructureBinding& other)
  {
    for (auto& binding : other.bindings)
    {
      bindings.push_back(binding->clone());
    }
  }

  void VectorDestructureBinding::apply(Scope& scope, const sptr_val& vector_expr) const
  {
    sptr_val_v& vec = std::get<sptr_val_v>(vector_expr->value);
    sptr_ast_node form = std::make_shared<AST::RuntimeValueWrapper>(Value::vector(vec));

    for (size_t i = 0; i < this->bindings.size(); i++)
    {
      this->bindings[i]->apply(scope, i < vec.size() ? vec[i] : Constant::NIL);
    }
  }

  std::unique_ptr<LexicalBinding> VectorDestructureBinding::clone() const
  {
    return std::make_unique<VectorDestructureBinding>(*this);
  }

  /** RestBinding */
  RestBinding::RestBinding(const std::string& symbol)
    : symbol(symbol)
  {
  }

  RestBinding::RestBinding(const RestBinding& other)
    : symbol(other.symbol)
  {
  }

  void RestBinding::apply(Scope& scope, const sptr_val_v& values) const
  {
    scope.store(symbol, Value::vector(values));
  }

  std::unique_ptr<RestBinding> RestBinding::clone() const
  {
    return std::make_unique<RestBinding>(symbol);
  }

} // namespace Lisple
