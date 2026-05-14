
#include "lisple/namespace.h"

#include <algorithm>
#include <regex>
#include <utility>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/scope.h>
#include <lisple/type.h>

namespace Lisple
{
  Namespace::Namespace(Type type, const std::string& name)
    : type(type)
    , name(name)
  {
  }

  Namespace::Namespace(Type type,
                       const std::string& name,
                       std::map<std::string, sptr_rtval> lang_symbols)
    : type(type)
    , name(name)
  {
    this->values = std::move(lang_symbols);
  }

  Namespace::Namespace(const std::string& name)
    : Namespace(Type::USER, name)
  {
    static std::regex regex_ns{"^[a-zA-Z0-9\\-_]+(\\.[a-zA-Z0-9\\-_]+)*$"};
    if (!std::regex_match(name, regex_ns))
    {
      throw NamespaceException("Invalid namespace name: '" + name + "'");
    }
  }

  sptr_rtval Namespace::find(const std::string& identifier_s) const
  {
    Symbol identifier(identifier_s);
    if (identifier.is_qualified())
    {
      if (aliased_namespaces.count(identifier.get_qualifier()))
      {
        Namespace* aliased = aliased_namespaces.at(identifier.get_qualifier());
        return aliased->lookup(identifier.get_identifier());
      }
      return nullptr;
    }

    if (this->values.count(identifier.get_identifier()))
    {
      return this->values.at(identifier.get_identifier());
    }
    for (auto* imported : imported_namespaces)
    {
      if (imported->has(identifier.get_identifier()))
      {
        return imported->lookup(identifier.get_identifier());
      }
    }

    return nullptr;
  }

  bool Namespace::has(const std::string& identifier) const
  {
    auto val_it = this->values.find(identifier);
    return val_it != this->values.end();
  }

  sptr_rtval Namespace::lookup(const std::string& identifier) const
  {
    sptr_rtval value = this->find(identifier);
    if (value)
    {
      return value;
    }

    return nullptr;
  }

  sptr_rtval Namespace::lookup(const RTValue& identifier) const
  {
    if (identifier.type != RTValue::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  Namespace Namespace::make_lang(std::map<std::string, sptr_rtval> lang_symbols)
  {
    return Namespace(Type::LANG, "", lang_symbols);
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
    return values.empty() && imported_namespaces.empty() &&
           aliased_namespaces.empty();
  }

  void Namespace::mutate(const std::string& identifier, const sptr_rtval& val)
  {
    if (this->type == Type::LANG)
    {
      throw NamespaceException("Cannnot override language identifier: '" + identifier +
                               "'.");
    }

    Scope::mutate(identifier, val);
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
