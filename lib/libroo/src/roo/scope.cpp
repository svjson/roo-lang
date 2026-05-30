
#include "roo/scope.h"

#include <roo/exception.h>
#include <roo/form.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  void Scope::store(const std::string& name, const sptr_val& value)
  {
    if (values.count(name))
    {
      throw IdentifierException("Identifier '" + name + "' is already defined.");
    }
    values.emplace(name, value);
  }

  void Scope::mutate(const std::string& name, const sptr_val& val)
  {
    remove(name);
    values.emplace(name, val);
  }

  void Scope::remove(const std::string& name)
  {
    if (values.count(name))
    {
      values.erase(name);
      return;
    }
    throw IdentifierException("Unknown identifier '" + name + "'");
  }

  void Scope::clear()
  {
    this->values.clear();
  }

  bool Scope::has(const std::string& identifier) const
  {
    return values.count(identifier);
  }

  const sptr_val* Scope::find(const std::string& symbol) const
  {
    auto it = values.find(symbol);
    if (it == values.end())
    {
      return nullptr;
    }
    return &it->second;
  }

  const sptr_val* Scope::find(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return find(identifier.str());
  }

  const sptr_val& Scope::lookup(const std::string& symbol) const
  {
    const sptr_val* value = find(symbol);
    if (!value)
    {
      throw IdentifierException("Unknown identifier '" + symbol + "'");
    }
    return *value;
  }

  const sptr_val& Scope::lookup(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  std::shared_ptr<AST::Vector> Scope::get_keys() const
  {
    sptr_ast_node_v keys;
    keys.reserve(values.size());
    for (auto& [key, val] : values)
    {
      keys.push_back(AST::Symbol::make(key));
    }
    return AST::Vector::make(keys);
  }
} // namespace Roo
