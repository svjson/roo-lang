
#include "namespace.h"

#include "exception.h"
#include "form.h"
#include "scope.h"
#include "type.h"
#include <algorithm>
#include <memory>
#include <regex>
#include <utility>

namespace Lisple
{
  std::regex regex_ns{"^[a-zA-Z0-9\\-_]+(\\.[a-zA-Z0-9\\-_]+)*$"};

  Namespace::Namespace(Type type, const std::string& name)
    : type(type)
    , name(name)
  {
  }

  Namespace::Namespace(Type type,
                       const std::string& name,
                       std::map<std::string, sptr_sobject> lang)
    : type(type)
    , name(name)
  {
    this->objects = std::move(lang);
  }

  Namespace::Namespace(const std::string& name)
    : Namespace(Type::USER, name)
  {
    if (!std::regex_match(name, regex_ns))
    {
      throw NamespaceException("Invalid namespace name: '" + name + "'");
    }
  }

  sptr_sobject Namespace::find(const Word& identifier) const
  {
    if (identifier.is_qualified())
    {
      if (aliased_namespaces.count(identifier.get_qualifier()))
      {
        Namespace* aliased = aliased_namespaces.at(identifier.get_qualifier());
        if (aliased->has(identifier.get_identifier()))
        {
          return aliased->lookup(identifier.get_identifier());
        }
        return nullptr;
      }
    }
    else if (this->objects.count(identifier.get_identifier()))
    {
      return this->objects.at(identifier.get_identifier());
    }

    if (Scope::has(identifier))
    {
      return Scope::lookup(identifier);
    }

    for (auto* imported : imported_namespaces)
    {
      if (imported->has(identifier))
      {
        return imported->lookup(identifier);
      }
    }

    return nullptr;
  }

  bool Namespace::has(const Word& identifier) const
  {
    return this->find(identifier) != nullptr;
  }

  sptr_sobject Namespace::lookup(const Word& identifier) const
  {
    sptr_sobject value = this->find(identifier);
    if (value)
    {
      return value;
    }

    return nullptr;
  }

  Namespace Namespace::make_lang(std::map<std::string, sptr_sobject> lang)
  {
    return Namespace(Type::LANG, "", lang);
  }

  Namespace::Type Namespace::get_type() const
  {
    return type;
  }

  const std::string& Namespace::get_name() const
  {
    return name;
  }

  bool Namespace::empty() const
  {
    return objects.empty() && values.empty();
  }

  void Namespace::mutate(const Word& identifier, const sptr_sobject& obj)
  {
    if (this->type == Type::LANG)
    {
      throw NamespaceException("Cannnot override language identifier: '" + identifier.value +
                               "'.");
    }

    Scope::mutate(identifier, obj);
  }

  void Namespace::import_full(Namespace& ns)
  {
    auto it = std::find(imported_namespaces.begin(), imported_namespaces.end(), &ns);
    if (it == imported_namespaces.end())
    {
      this->imported_namespaces.push_back(&ns);
    }
  }

  void Namespace::import_aliased(Namespace& ns, const std::string& alias)
  {
    if (!aliased_namespaces.count(alias))
    {
      this->aliased_namespaces.emplace(alias, &ns);
    }
    else if (aliased_namespaces.at(alias) != &ns)
    {
      throw NamespaceException("Cannot define alias '" + alias + "' for namespace '" +
                               ns.get_name() + "' " + "because '" + alias +
                               "' is already an alias for '" +
                               aliased_namespaces.at(alias)->get_name() +
                               "' within namespace '" + get_name() + "'");
    }
  }

} // namespace Lisple
