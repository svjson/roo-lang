
#include "lisple/runtime.h"

#include <exception>
#include <memory>
#include <stddef.h>
#include <utility>
#include <vector>

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/file_system.h>
#include <lisple/file_system_namespace_source.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/namespace_loader.h>
#include <lisple/reader.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  const std::string DEFAULT_NAMESPACE = "user";

  Runtime::~Runtime() = default;
  Runtime::Runtime(Runtime&&) = default;

  Runtime::Runtime()
    : Runtime(nullptr)
  {
  }

  Runtime::Runtime(FileSystem* fs)
    : lang(make_language_namespace())
    , fs(fs)
  {
    if (fs)
    {
      default_ns_source = std::make_unique<FileSystemNamespaceSource>(fs);
      namespace_loader = std::make_unique<NamespaceLoader>(default_ns_source.get());
    }
    switch_namespace(DEFAULT_NAMESPACE);
  }

  void Runtime::ensure_namespace_loaded(const std::string& ns_name)
  {
    if (namespace_loader)
    {
      namespace_loader->load(*this, ns_name);
    }
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
      throw LispleException("Provided namespace '" + ns.get_name() +
                            "' is of an invalid type.");
    }

    const std::string& ns_name = ns.get_name();
    namespaces.emplace(ns.get_name(), std::move(ns));
    switch_namespace(ns_name);
  }

  Runtime::Runtime(std::map<const std::string, Namespace> namespaces)
    : Runtime(DEFAULT_NAMESPACE, namespaces, nullptr)
  {
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
        throw LispleException("Provided namespace '" + ns.get_name() +
                              "' is of an invalid type.");
      }
      this->namespaces.emplace(name, std::move(ns));
    }

    switch_namespace(main_ns);
  }

  Runtime::Runtime(std::vector<std::unique_ptr<Namespace>> namespaces, FileSystem* fs)
    : Runtime(DEFAULT_NAMESPACE, std::move(namespaces), fs)
  {
  }

  Runtime::Runtime(const std::string& default_namespace,
                   std::vector<std::unique_ptr<Namespace>> namespaces,
                   FileSystem* fs)
    : Runtime(fs)
  {
    for (auto& ns : namespaces)
    {
      this->namespaces.emplace(ns->get_name(), std::move(*ns.get()));
    }
    switch_namespace(default_namespace);
  }

  /**
   * Switching to a non-existent namespace will create it and store it the namespace map.
   * It is still empty when switched out, it will be evicted from the map.
   */
  void Runtime::switch_namespace(const std::string& namespace_name)
  {
    if (current_namespace && current_namespace->empty() &&
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
    ensure_namespace_loaded(namespace_name);

    if (!namespaces.count(namespace_name))
    {
      throw NamespaceException("Cannot import namespace '" + namespace_name +
                               "', because it does not exist.");
    }

    get_current_namespace().import_full(namespaces.at(namespace_name));
  }

  /**
   *
   */
  void Runtime::define_namespace_alias(const std::string& namespace_name,
                                       const std::string& alias)
  {
    ensure_namespace_loaded(namespace_name);

    if (!namespaces.count(namespace_name))
    {
      throw NamespaceException("Cannot create an alias for namespace '" + namespace_name +
                               "', because it does not exist");
    }
    get_current_namespace().import_aliased(namespaces.at(namespace_name), alias);
  }

  Namespace* Runtime::ns(const std::string& namespace_name, bool create_if_absent)
  {
    if (!namespaces.count(namespace_name))
    {
      if (!create_if_absent) return nullptr;
      namespaces.emplace(namespace_name, Namespace(namespace_name));
      if (this->namespace_loader)
      {
        this->namespace_loader->apply_metadata(*this, namespace_name);
      }
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
      throw new LispleException(
        "This Lisple context does not provide any file system access");
    }

    auto raw_file = fs->read_file_to_string(file_name);

    const std::string current_ns = get_current_namespace().get_name();
    if (namespace_loader)
    {
      namespace_loader->push_file_context(file_name);
    }
    try
    {
      eval(ctx, raw_file);
    }
    catch (Lisple::LispleException* e)
    {
      if (namespace_loader)
      {
        namespace_loader->pop_file_context();
      }
      throw Lisple::LispleException("Error reading '" + file_name + "': " + e->what());
    }
    if (namespace_loader)
    {
      namespace_loader->pop_file_context();
    }
    get_current_namespace().set_origin(Namespace::Origin::file(file_name));
    switch_namespace(current_ns);
  }

  sptr_rtval Runtime::eval(Context& ctx, const std::string& str)
  {
    sptr_sobject_v script = sexp_reader.read_sexps(str);

    sptr_rtval result;

    for (auto& sexp : script)
    {
      result = eval(ctx, sexp);
    }
    return result;
  }

  sptr_rtval Runtime::eval(const std::string& str)
  {
    Context ctx(*this);
    return this->eval(ctx, str);
  }

  sptr_rtval Runtime::eval(const sptr_sobject& statement)
  {
    Context ctx(*this);
    return this->eval(ctx, statement);
  }

  sptr_rtval Runtime::eval(Context& ctx, const sptr_sobject& form)
  {
    LowerContext lctx{&ctx};
    auto exec_node = lower_expr(lctx, form);
    return Lisple::exec(ctx, *exec_node);
  }

  sptr_rtval Runtime::invoke(const std::string& function, sptr_rtval_v& args)
  {
    Context ctx(*this);
    sptr_rtval inv = lookup_value(Word(function));
    if (inv->type != RTValue::Type::FUNCTION)
    {
      throw new InvocationException(inv->to_string() + " is not executable.");
    }

    sptr_sobject& fun_obj = std::get<sptr_sobject>(inv->value);
    Executable& exec = fun_obj->as<Executable>();
    try
    {
      return exec.execute(ctx, args);
    }
    catch (std::exception& e)
    {
      throw InvocationException("Error while invoking " + function + ":\n" +
                                inv->to_string() + "\n" + e.what());
    }
  }

  sptr_sobject Runtime::lookup(const Word& identifier)
  {
    return lookup(identifier, nullptr);
  }

  sptr_rtval Runtime::lookup_value(const std::string& identifier)
  {
    return lookup_value(Lisple::Word(identifier));
  }

  sptr_rtval Runtime::lookup_value(const std::string& identifier,
                                   const sptr_rtval& default_value)
  {
    sptr_rtval value = lookup_value(Lisple::Word(identifier));
    if (*value == *Constant::NIL)
    {
      return default_value;
    }
    return value;
  }

  sptr_rtval Runtime::lookup_value(const Word& identifier)
  {
    if (identifier.is_qualified())
    {
      Namespace* _ns = ns(identifier.get_qualifier());
      if (_ns)
      {
        return _ns->lookup_symbol(identifier.get_identifier());
      }

      sptr_rtval result = current_namespace->lookup_symbol(identifier);
      if (result) return result;

      throw IdentifierException("Unknown identifier: '" + identifier.to_string() + "'");
    }

    sptr_rtval lang_obj = lang.lookup_symbol(identifier.get_identifier());
    if (lang_obj.get())
    {
      return lang_obj;
    }

    sptr_rtval ns_obj = current_namespace->lookup_symbol(identifier.get_identifier());
    if (ns_obj.get())
    {
      return ns_obj;
    }

    throw IdentifierException("Unknown identifier: '" + identifier.value + "'");
  }

  sptr_sobject Runtime::lookup(const Word& identifier, sptr_sobject fallback)
  {
    if (identifier.is_qualified())
    {
      Namespace* _ns = ns(identifier.get_qualifier());
      if (_ns)
      {
        return _ns->lookup(Word(identifier.get_identifier()));
      }

      sptr_sobject result = current_namespace->lookup(identifier);
      if (result) return result;

      throw IdentifierException("Unknown identifier: '" + identifier.to_string() + "'");
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

} // namespace Lisple
