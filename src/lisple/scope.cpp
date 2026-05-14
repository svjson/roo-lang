
#include "lisple/scope.h"

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  void Scope::store(const std::string& name, const sptr_rtval& value)
  {
    if (values.count(name))
    {
      throw IdentifierException("Identifier '" + name + "' is already defined.");
    }
    values.emplace(name, value);
  }

  void Scope::mutate(const std::string& name, const sptr_rtval& val)
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

  sptr_rtval Scope::lookup(const std::string& symbol) const
  {
    auto it = values.find(symbol);
    if (it == values.end())
    {
      return nullptr;
    }
    return it->second;
  }

  sptr_rtval Scope::lookup(const RTValue& identifier) const
  {
    if (identifier.type != RTValue::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  std::shared_ptr<Array> Scope::get_keys() const
  {
    sptr_sobject_v keys;
    keys.reserve(values.size());
    for (auto& [key, val] : values)
    {
      keys.push_back(Word::make(key));
    }
    return Array::make(keys);
  }
} // namespace Lisple
