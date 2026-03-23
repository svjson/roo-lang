
#include "scope.h"

#include "exception.h"
#include "form.h"
#include "runtime/value.h"
#include "type.h"

namespace Lisple
{
  void Scope::store(const Word& name, const sptr_sobject& obj)
  {
    this->store(name.value, obj);
  }

  void Scope::store(const std::string& name, const sptr_sobject& obj)
  {
    if (objects.count(name) || values.count(name))
    {
      throw IdentifierException("Identifier '" + name + "' is already defined.");
    }
    objects.emplace(name, obj);
  }

  void Scope::store(const std::string& name, const sptr_rtval& value)
  {
    if (objects.count(name) || values.count(name))
    {
      throw IdentifierException("Identifier '" + name + "' is already defined.");
    }
    values.emplace(name, value);
  }

  void Scope::mutate(const Word& name, const sptr_sobject& obj)
  {
    remove(name);
    objects.emplace(name.value, obj);
  }

  void Scope::remove(const Word& name)
  {
    if (objects.count(name.value))
    {
      objects.erase(name.value);
      return;
    }
    else if (values.count(name.value))
    {
      values.erase(name.value);
      return;
    }
    throw IdentifierException("Unknown identifier '" + name.value + "'");
  }

  void Scope::clear()
  {
    this->objects.clear();
    this->values.clear();
  }

  bool Scope::has(const Word& identifier) const
  {
    return objects.count(identifier.value) || values.count(identifier.value);
  }

  sptr_sobject Scope::lookup(const Word& identifier) const
  {
    if (objects.count(identifier.value))
    {
      return objects.at(identifier.value);
    }
    else if (values.count(identifier.value))
    {
      return RuntimeValueWrapper::make(values.at(identifier.value));
    }
    // Not returning NIL, as we need to know if we hit something whose value
    // is actually NIL or if the identifier doesn't exist in the scope.
    // FIXME: Do we?
    return nullptr;
  }

  sptr_rtval Scope::lookup(const std::string& symbol) const
  {
    auto it = objects.find(symbol);
    if (it == objects.end())
    {
      auto v_it = values.find(symbol);
      if (v_it == values.end())
      {
        return nullptr;
      }

      return v_it->second;
    }

    if (auto* wrapper = dynamic_cast<RuntimeValueWrapper*>(it->second.get()))
    {
      return wrapper->val;
    }

    sptr_sobject result = it->second;
    return to_rt_value(result);
  }

  std::shared_ptr<Array> Scope::get_keys() const
  {
    sptr_sobject_v keys;
    keys.reserve(objects.size() + values.size());
    for (auto& [key, val] : objects)
    {
      keys.push_back(Word::make(key));
    }
    for (auto& [key, val] : values)
    {
      keys.push_back(Word::make(key));
    }
    return Array::make(keys);
  }
} // namespace Lisple
