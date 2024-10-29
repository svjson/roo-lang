
#include "lisp_reader.h"

#include <exception>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <utility>
#include <vector>

#include "context.h"
#include "lang.h"
#include "exec.h"
#include "form.h"
#include "namespace.h"
#include "reader.h"
#include "type.h"

#include "file_system.h"
#include "lisple_exception.h"

namespace Lisple
{
  const std::string DEFAULT_NAMESPACE = "user";

  LispReader::LispReader()
    : LispReader(nullptr)
  {
  }

  LispReader::LispReader(FileSystem* fs)
    : lang(Lisple::make_language_namespace())
    , fs(fs)
  {
    switch_namespace(DEFAULT_NAMESPACE);
  }

  LispReader::LispReader(Namespace& ns)
    : LispReader(ns, nullptr)
  {
  }

  LispReader::LispReader(Namespace& ns, FileSystem* fs)
    : LispReader(fs)
  {
    if (ns.get_type() != Namespace::Type::USER)
    {
      throw LispleException("Provided namespace '" + ns.get_name() + "' is of an invalid type.");
    }

    const std::string& ns_name = ns.get_name();
    namespaces.emplace(ns.get_name(), std::move(ns));
    switch_namespace(ns_name);
  }

  LispReader::LispReader(const std::string& main_ns, std::map<const std::string, Namespace> namespaces)
    : LispReader(main_ns, namespaces, nullptr)
  {
  }

  LispReader::LispReader(const std::string& main_ns,
                         std::map<const std::string, Namespace> namespaces,
                         FileSystem* fs)
    : LispReader(fs)
  {
    for (auto& [name, ns] : namespaces)
    {
      if (ns.get_type() != Namespace::Type::USER)
      {
        throw LispleException("Provided namespace '" + ns.get_name() + "' is of an invalid type.");
      }
    }

    this->namespaces = std::move(namespaces);
    switch_namespace(main_ns);
  }

  /**
   * Switching to a non-existent namespace will create it and store it the namespace map.
   * It is still empty when switched out, it will be evicted from the map.
   */
  void LispReader::switch_namespace(const std::string& namespace_name)
  {
    if (current_namespace && current_namespace->empty() && namespaces.count(current_namespace->get_name()))
    {
      namespaces.erase(current_namespace->get_name());
    }

    current_namespace = ns(namespace_name, true);
  }

  /**
   *
   */
  void LispReader::import_namespace(const std::string& namespace_name)
  {
    if (!namespaces.count(namespace_name))
    {
      throw NamespaceException("Cannot import namespace '" + namespace_name + "', because it does not exist.");
    }

    get_current_namespace().import_full(namespaces.at(namespace_name));
  }

  /**
   *
   */
  void LispReader::define_namespace_alias(const std::string& namespace_name, const std::string& alias)
  {
    if (!namespaces.count(namespace_name))
    {
      throw NamespaceException("Cannot create an alias for namespace '" + namespace_name + "', because it does not exist");
    }
    get_current_namespace().import_aliased(namespaces.at(namespace_name), alias);
  }

  Namespace* LispReader::ns(const std::string& namespace_name, bool create_if_absent)
  {
    if (!namespaces.count(namespace_name))
    {
      if (!create_if_absent) return nullptr;
      namespaces.emplace(namespace_name, Namespace(namespace_name));
    }

    return &namespaces.at(namespace_name);
  }

  bool LispReader::has_file_system_access() const
  {
    return fs != nullptr;
  }

  Namespace& LispReader::get_current_namespace()
  {
    return *current_namespace;
  }

  void LispReader::read_file(const std::string& file_name)
  {
    Lisple::Context ctx = Lisple::Context(*this);
    read_file(ctx, file_name);
  }

  void LispReader::read_file(Lisple::Context& ctx, const std::string& file_name)
  {
    if (!fs)
    {
      throw new LispleException("This Lisple context does not provide any file system access");
    }
    std::cout << "Reading data: " << file_name << std::endl;

    auto raw_file = fs->read_file_to_string(file_name);

    const std::string& current_ns = get_current_namespace().get_name();
    eval(ctx, raw_file);
    switch_namespace(current_ns); // Revert any namespace changes from evaluating the file
  }

  Lisple::sptr_sobject LispReader::eval(Lisple::Context& ctx, const std::string& str)
  {
    Lisple::sptr_sobject_v script = sexp_reader.read_sexps(str);

    Lisple::sptr_sobject result;

    for (auto& sexp : script)
    {
      result = eval(ctx, sexp);
    }
    return result;
  }

  Lisple::sptr_sobject LispReader::eval(const std::string& str)
  {
    Lisple::Context ctx(*this);
    return this->eval(ctx, str);
  }

  Lisple::sptr_sobject LispReader::eval(const Lisple::sptr_sobject& statement)
  {
    Lisple::Context ctx(*this);
    return this->eval(ctx, statement);
  }

  Lisple::sptr_sobject LispReader::eval(Lisple::Context& ctx, const Lisple::sptr_sobject& statement)
  {
    switch (statement->get_type())
    {
      case Lisple::Form::LIST:
        return eval_list(ctx, dynamic_cast<Lisple::List&>(*statement));
      case Lisple::Form::ARRAY:
        return eval_array(ctx, dynamic_cast<Lisple::Array&>(*statement));
      case Lisple::Form::MAP:
        return eval_map(ctx, dynamic_cast<Lisple::Map&>(*statement));
      case Lisple::Form::STRING:
      case Lisple::Form::CHAR:
      case Lisple::Form::NUMBER:
      case Lisple::Form::SYMBOL:
      case Lisple::Form::FUNCTION:
      case Lisple::Form::NIL:
      case Lisple::Form::KEY:
      case Lisple::Form::BOOLEAN:
      case Lisple::Form::B_TRUE:
      case Lisple::Form::B_FALSE:
      case Lisple::Form::HOST_OBJECT:
        return statement;
      case Lisple::Form::WORD:
        return ctx.evalp() ? ctx.lookup(dynamic_cast<Lisple::Word&>(*statement)) : statement;
      case Lisple::Form::DISCARD:
        return Lisple::NIL;
      default:
        break;
    }

    throw Lisple::LispleException("Encountered unimplemented form: " + statement->to_string());
 }

  Lisple::sptr_sobject_v LispReader::eval_sexpression(Lisple::Context& ctx, Lisple::Sexpression& sexp)
  {
    Lisple::sptr_sobject_v elements;
    auto& children = sexp.get_children();
    elements.reserve(children.size());

    Lisple::Signature* sig = nullptr;

    for (size_t i=0; i < children.size(); i++)
    {
      if (children[i]->get_type() == Lisple::Form::DISCARD)
      {
        continue;
      }

      if (ctx.evalp() &&
          i == 0 &&
          sexp.get_type() == Lisple::Form::LIST &&
          children[i]->get_type() == Lisple::Form::WORD)
      {
        auto head = this->eval(ctx, children[i]);
        if (head->get_type() == Lisple::Form::MACRO)
        {
          auto tail = sexp.tail();
          sig = &head->as<Lisple::Macro>().get_signature(tail);
        }
        elements.push_back(head);
      }
      else if (sig)
      {
        ctx.push_context(sig->should_eval_arg(i - 1));
        elements.push_back(this->eval(ctx, children[i]));
        ctx.pop_context();
      }
      else
      {
        elements.push_back(this->eval(ctx, children[i]));
      }
    }

    return elements;
  }

  Lisple::sptr_sobject LispReader::eval_list(Lisple::Context& ctx, Lisple::List& list)
  {
    if (list.is_quoted())
    {
      return std::make_shared<Lisple::List>(list.get_children(), true);
    }
    auto elements = eval_sexpression(ctx, list);
    auto lobj = std::make_shared<Lisple::List>(elements);

    if (ctx.evalp())
    {
      try
      {
        return lobj->execute(ctx);
      }
      catch (std::exception& e)
      {
        throw Lisple::InvocationException("Error while invoking " + list.get_children().front()->to_string() + ":\n" +
                                          list.to_string() + "\n" + e.what());
      }
    }

    return lobj;
  }

  Lisple::sptr_sobject LispReader::eval_array(Lisple::Context& ctx, Lisple::Array& array)
  {
    auto elements = eval_sexpression(ctx, array);
    return std::make_shared<Lisple::Array>(elements);
  }

  Lisple::sptr_sobject LispReader::eval_map(Lisple::Context& ctx, Lisple::Map& map)
  {
    auto elements = eval_sexpression(ctx, map);
    return std::make_shared<Lisple::Map>(elements);
  }

  sptr_sobject LispReader::call_fn(const std::string& identifier, sptr_sobject_v& args)
  {
    Context ctx(*this);
    sptr_sobject inv = lookup(Lisple::Word(identifier));
    if (!inv)
    {
      throw Lisple::IdentifierException("Unknown identifier: '" + identifier + "'");
    }

    try
    {
      return inv->execute(ctx, args);
    }
    catch (std::exception& e)
    {
      throw Lisple::InvocationException("Error while invoking " + identifier + ":\n" + inv->to_string() + "\n" + e.what());
    }

    return Lisple::NIL;
  }

  sptr_sobject LispReader::call_fn(const std::string& identifier, sptr_sobject& arg)
  {
    sptr_sobject_v args = sptr_sobject_v { arg };
    return call_fn(identifier, args);
  }

  Lisple::sptr_sobject LispReader::lookup(const Lisple::Word& identifier)
  {
    return lookup(identifier, nullptr);
  }

  Lisple::sptr_sobject LispReader::lookup(const Lisple::Word& identifier, Lisple::sptr_sobject fallback)
  {
    if (identifier.is_qualified())
    {
      Namespace* _ns = ns(identifier.get_qualifier());
      if (_ns) return _ns->lookup(Lisple::Word(identifier.get_identifier()));

      Lisple::sptr_sobject result = current_namespace->lookup(identifier);
      if (result) return result;

      throw Lisple::IdentifierException("Unknown identifier: '" + identifier.value + "'");
    }

    Lisple::sptr_sobject lang_obj = lang.lookup(identifier);
    if (lang_obj.get())
    {
      return lang_obj;
    }

    Lisple::sptr_sobject ns_obj = current_namespace->lookup(identifier);
    if (ns_obj.get())
    {
      return ns_obj;
    }

    if (!fallback)
    {
      throw Lisple::IdentifierException("Unknown identifier: '" + identifier.value + "'");
    }
    return fallback;
  }

  Lisple::Namespace& LispReader::get_ns_of(const Lisple::Word& identifier)
  {
    if (current_namespace->has(identifier))
    {
      return *current_namespace;
    }
    else if (lang.has(identifier))
    {
      return lang;
    }

    throw Lisple::IdentifierException("Unknown identifier: " + identifier.value);
  }

}
