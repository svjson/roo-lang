
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
                       std::map<std::string, sptr_sobject> lang)
    : type(type)
    , name(name)
  {
    this->objects = std::move(lang);
  }

  Namespace::Namespace(Type type,
                       const std::string& name,
                       std::map<std::string, sptr_sobject> lang,
                       std::map<std::string, sptr_rtval> lang_symbols)
    : type(type)
    , name(name)
  {
    this->objects = std::move(lang);
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

  sptr_rtval Namespace::find_symbol(const Word& identifier) const
  {
    if (identifier.is_qualified())
    {
      if (aliased_namespaces.count(identifier.get_qualifier()))
      {
        Namespace* aliased = aliased_namespaces.at(identifier.get_qualifier());
        if (aliased->has(identifier.get_identifier()))
        {
          return aliased->lookup_symbol(identifier.get_identifier());
        }
        return nullptr;
      }
    }
    else if (this->values.count(identifier.get_identifier()))
    {
      return this->values.at(identifier.get_identifier());
    }
    else if (this->objects.count(identifier.get_identifier()))
    {
      sptr_sobject match = this->objects.at(identifier.get_identifier());
      return Lisple::to_rt_value(match);
    }

    if (Scope::has(identifier))
    {
      return Scope::lookup_value(identifier.get_identifier());
    }

    for (auto* imported : imported_namespaces)
    {
      if (imported->has(identifier))
      {
        return imported->lookup_symbol(identifier.get_identifier());
      }
    }

    return nullptr;
  }

  bool Namespace::has(const Word& identifier) const
  {
    auto val_it = this->values.find(identifier.to_string());
    if (val_it != this->values.end()) return true;

    auto obj_it = this->objects.find(identifier.to_string());
    return obj_it != this->objects.end();
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

  sptr_rtval Namespace::lookup_symbol(const Word& identifier) const
  {
    sptr_rtval value = this->find_symbol(identifier);
    if (value)
    {
      return value;
    }

    return nullptr;
  }

  Namespace Namespace::make_lang(std::map<std::string, sptr_sobject> lang,
                                 std::map<std::string, sptr_rtval> lang_symbols)
  {
    return Namespace(Type::LANG, "", lang, lang_symbols);
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
    return objects.empty() && values.empty() && imported_namespaces.empty() &&
           aliased_namespaces.empty();
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
