
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
                       std::map<std::string, sptr_val> lang_symbols)
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

  const sptr_val* Namespace::find_local(const std::string& identifier) const
  {
    auto it = values.find(identifier);
    return it == values.end() ? nullptr : &it->second;
  }

  const sptr_val* Namespace::find(const std::string& identifier_s) const
  {
    AST::Symbol identifier(identifier_s);
    if (identifier.is_qualified())
    {
      if (aliased_namespaces.count(identifier.get_qualifier()))
      {
        Namespace* aliased = aliased_namespaces.at(identifier.get_qualifier());
        return aliased->find(identifier.get_identifier());
      }
      return nullptr;
    }

    if (const sptr_val* value = find_local(identifier.get_identifier()))
    {
      return value;
    }
    for (auto* imported : imported_namespaces)
    {
      if (const sptr_val* value = imported->find_local(identifier.get_identifier()))
      {
        return value;
      }
    }

    return nullptr;
  }

  bool Namespace::has(const std::string& identifier) const
  {
    auto val_it = this->values.find(identifier);
    return val_it != this->values.end();
  }

  const sptr_val* Namespace::find(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return find(identifier.str());
  }

  const sptr_val& Namespace::lookup(const std::string& identifier) const
  {
    const sptr_val* value = find(identifier);
    if (!value)
    {
      throw IdentifierException("Unknown identifier: '" + identifier + "'");
    }
    return *value;
  }

  const sptr_val& Namespace::lookup(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  Namespace Namespace::make_lang(std::map<std::string, sptr_val> lang_symbols)
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
    return values.empty() && imported_namespaces.empty() && aliased_namespaces.empty();
  }

  void Namespace::mutate(const std::string& identifier, const sptr_val& val)
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
