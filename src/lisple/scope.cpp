
#include "scope.h"

#include "form.h"
#include "type.h"

#include "exception.h"

namespace Lisple
{
  void Scope::store(const Word& name, const sptr_sobject& obj)
  {
    if (objects.count(name.value))
    {
      throw IdentifierException("Identifier '" + name.value + "' is already defined.");
    }
    objects.emplace(name.value, obj);
  }

  void Scope::mutate(const Word& name, const sptr_sobject& obj)
  {
    remove(name);
    objects.emplace(name.value, obj);
  }

  void Scope::remove(const Word& name)
  {
    if (!objects.count(name.value))
    {
      throw IdentifierException("Unknown identifier '" + name.value + "'");
    }
    objects.erase(name.value);
  }

  void Scope::clear()
  {
    this->objects.clear();
  }

  bool Scope::has(const Word& identifier) const
  {
    return objects.count(identifier.value);
  }

  sptr_sobject Scope::lookup(const Word& identifier) const
  {
    if (objects.count(identifier.value))
    {
      return objects.at(identifier.value);
    }
    // Not returning NIL, as we need to know if we hit something whose value
    // is actually NIL or if the identifier doesn't exist in the scope.
    return nullptr;
  }

  std::shared_ptr<Array> Scope::get_keys()
  {
    auto array = std::make_shared<Array>();
    for (auto& [key, val] : objects)
    {
      sptr_sobject word = std::make_shared<Word>(key);
      array->append(word);
    }
    return array;
  }
}
