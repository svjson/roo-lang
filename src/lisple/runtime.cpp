
#include "runtime.h"

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
#include "exception.h"

namespace Lisple
{
  const std::string DEFAULT_NAMESPACE = "user";

  Runtime::Runtime()
    : Runtime(nullptr)
  {
  }

  Runtime::Runtime(FileSystem* fs)
    : lang(make_language_namespace())
    , fs(fs)
  {
    switch_namespace(DEFAULT_NAMESPACE);
  }

  Runtime::Runtime(Namespace& ns)
    : Runtime(ns, nullptr)
  {
  }

  Runtime::Runtime(Namespace& ns, FileSystem* fs)
    : Runtime(fs)
  {
    if (ns.get_type() != Namespace::Type::USER)
    {
      throw LispleException("Provided namespace '" + ns.get_name() + "' is of an invalid type.");
    }

    const std::string& ns_name = ns.get_name();
    namespaces.emplace(ns.get_name(), std::move(ns));
    switch_namespace(ns_name);
  }

  Runtime::Runtime(const std::string& main_ns,
                   std::map<const std::string, Namespace> namespaces)
    : Runtime(main_ns, namespaces, nullptr)
  {
  }

  Runtime::Runtime(const std::string& main_ns,
                   std::map<const std::string, Namespace> namespaces,
                   FileSystem* fs)
    : Runtime(fs)
  {
    for (auto& [name, ns] : namespaces)
    {
      if (ns.get_type() != Namespace::Type::USER)
      {
        throw LispleException("Provided namespace '" + ns.get_name() + "' is of an invalid type.");
      }
      this->namespaces.emplace(name, std::move(ns));
    }

    switch_namespace(main_ns);
  }

  /**
   * Switching to a non-existent namespace will create it and store it the namespace map.
   * It is still empty when switched out, it will be evicted from the map.
   */
  void Runtime::switch_namespace(const std::string& namespace_name)
  {
    if (current_namespace &&
        current_namespace->empty() &&
        namespaces.count(current_namespace->get_name()) &&
        current_namespace->name != namespace_name)
    {
      namespaces.erase(current_namespace->get_name());
    }

    current_namespace = ns(namespace_name, true);
  }

  /**
   *
   */
  void Runtime::import_namespace(const std::string& namespace_name)
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
  void Runtime::define_namespace_alias(const std::string& namespace_name, const std::string& alias)
  {
    if (!namespaces.count(namespace_name))
    {
      throw NamespaceException("Cannot create an alias for namespace '" + namespace_name + "', because it does not exist");
    }
    get_current_namespace().import_aliased(namespaces.at(namespace_name), alias);
  }

  Namespace* Runtime::ns(const std::string& namespace_name, bool create_if_absent)
  {
    if (!namespaces.count(namespace_name))
    {
      if (!create_if_absent) return nullptr;
      namespaces.emplace(namespace_name, Namespace(namespace_name));
    }

    return &namespaces.at(namespace_name);
  }

  bool Runtime::has_file_system_access() const
  {
    return fs != nullptr;
  }

  Namespace& Runtime::get_current_namespace()
  {
    return *current_namespace;
  }

  void Runtime::read_file(const std::string& file_name)
  {
    Context ctx = Context(*this);
    read_file(ctx, file_name);
  }

  void Runtime::read_file(Context& ctx, const std::string& file_name)
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

  sptr_sobject Runtime::eval(Context& ctx, const std::string& str)
  {
    sptr_sobject_v script = sexp_reader.read_sexps(str);

    sptr_sobject result;

    for (auto& sexp : script)
    {
      result = eval(ctx, sexp);
    }
    return result;
  }

  sptr_sobject Runtime::eval(const std::string& str)
  {
    Context ctx(*this);
    return this->eval(ctx, str);
  }

  sptr_sobject Runtime::eval(const sptr_sobject& statement)
  {
    Context ctx(*this);
    return this->eval(ctx, statement);
  }

  sptr_sobject Runtime::eval(Context& ctx, const sptr_sobject& statement)
  {
    switch (statement->get_type())
    {
      case Form::LIST:
        return eval_list(ctx, dynamic_cast<List&>(*statement));
      case Form::ARRAY:
        return eval_array(ctx, dynamic_cast<Array&>(*statement));
      case Form::MAP:
        return eval_map(ctx, dynamic_cast<Map&>(*statement));
      case Form::STRING:
      case Form::CHAR:
      case Form::NUMBER:
      case Form::SYMBOL:
      case Form::FUNCTION:
      case Form::NIL:
      case Form::KEY:
      case Form::BOOLEAN:
      case Form::B_TRUE:
      case Form::B_FALSE:
      case Form::HOST_OBJECT:
        return statement;
      case Form::WORD:
        return ctx.evalp() ? ctx.lookup(dynamic_cast<Word&>(*statement)) : statement;
      case Form::DISCARD:
        return NIL;
      default:
        break;
    }

    throw LispleException("Encountered unimplemented form: " + statement->to_string());
 }

  sptr_sobject_v Runtime::eval_sexpression(Context& ctx, Seq& sexp)
  {
    sptr_sobject_v elements;
    auto& children = sexp.get_children();
    elements.reserve(children.size());

    Signature* sig = nullptr;

    for (size_t i=0; i < children.size(); i++)
    {
      if (children[i]->get_type() == Form::DISCARD)
      {
        continue;
      }

      if (ctx.evalp() &&
          i == 0 &&
          sexp.get_type() == Form::LIST &&
          children[i]->get_type() == Form::WORD)
      {
        auto head = this->eval(ctx, children[i]);
        if (head->get_type() == Form::MACRO)
        {
          auto tail = sexp.tail();
          sig = &head->as<Macro>().get_signature(tail);
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

  sptr_sobject Runtime::eval_list(Context& ctx, List& list)
  {
    if (list.is_quoted())
    {
      return std::make_shared<List>(list.get_children(), true);
    }
    auto elements = eval_sexpression(ctx, list);
    auto lobj = std::make_shared<List>(elements);

    if (ctx.evalp())
    {
      try
      {
        return lobj->execute(ctx);
      }
      catch (std::exception& e)
      {
        throw InvocationException("Error while invoking " + list.get_children().front()->to_string() + ":\n" +
                                          list.to_string() + "\n" + e.what());
      }
    }

    return lobj;
  }

  sptr_sobject Runtime::eval_array(Context& ctx, Array& array)
  {
    auto elements = eval_sexpression(ctx, array);
    return std::make_shared<Array>(elements);
  }

  sptr_sobject Runtime::eval_map(Context& ctx, Map& map)
  {
    auto elements = eval_sexpression(ctx, map);
    return std::make_shared<Map>(elements);
  }

  sptr_sobject Runtime::call_fn(const std::string& identifier, sptr_sobject_v& args)
  {
    Context ctx(*this);
    sptr_sobject inv = lookup(Word(identifier));
    if (!inv)
    {
      throw IdentifierException("Unknown identifier: '" + identifier + "'");
    }

    try
    {
      return inv->execute(ctx, args);
    }
    catch (std::exception& e)
    {
      throw InvocationException("Error while invoking " + identifier + ":\n" + inv->to_string() + "\n" + e.what());
    }

    return NIL;
  }

  sptr_sobject Runtime::call_fn(const std::string& identifier, sptr_sobject& arg)
  {
    sptr_sobject_v args = sptr_sobject_v { arg };
    return call_fn(identifier, args);
  }

  sptr_sobject Runtime::lookup(const Word& identifier)
  {
    return lookup(identifier, nullptr);
  }

  sptr_sobject Runtime::lookup(const Word& identifier, sptr_sobject fallback)
  {
    if (identifier.is_qualified())
    {
      Namespace* _ns = ns(identifier.get_qualifier());
      if (_ns) return _ns->lookup(Word(identifier.get_identifier()));

      sptr_sobject result = current_namespace->lookup(identifier);
      if (result) return result;

      throw IdentifierException("Unknown identifier: '" + identifier.value + "'");
    }

    sptr_sobject lang_obj = lang.lookup(identifier);
    if (lang_obj.get())
    {
      return lang_obj;
    }

    sptr_sobject ns_obj = current_namespace->lookup(identifier);
    if (ns_obj.get())
    {
      return ns_obj;
    }

    if (!fallback)
    {
      throw IdentifierException("Unknown identifier: '" + identifier.value + "'");
    }
    return fallback;
  }

  sptr_sobject Runtime::lookup(const std::string& identifier)
  {
    return this->lookup(Word(identifier));
  }

  Namespace& Runtime::get_ns_of(const Word& identifier)
  {
    if (current_namespace->has(identifier))
    {
      return *current_namespace;
    }
    else if (lang.has(identifier))
    {
      return lang;
    }

    throw IdentifierException("Unknown identifier: " + identifier.value);
  }

}
