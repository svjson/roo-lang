
#include "lisple/runtime.h"

#include <exception>
#include <memory>
#include <stddef.h>
#include <utility>
#include <vector>

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/io/file_system.h>
#include <lisple/io/file_system_namespace_source.h>
#include <lisple/io/null_file_system.h>
#include <lisple/lang.h>
#include <lisple/lang/io/io_namespace.h>
#include <lisple/namespace.h>
#include <lisple/namespace_loader.h>
#include <lisple/reader.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  namespace
  {
    NullFileSystem& null_file_system()
    {
      static NullFileSystem fs;
      return fs;
    }

    bool has_parse_file_context(const Lisple::ParseException& e)
    {
      const std::string message = e.what();
      return message.compare(0, 15, "Error parsing '") == 0;
    }
  } // namespace

  const std::string DEFAULT_NAMESPACE = "user";

  Runtime::~Runtime() = default;
  Runtime::Runtime(Runtime&&) = default;

  Runtime::Runtime()
    : Runtime(nullptr)
  {
  }

  Runtime::Runtime(FileSystem* fs)
    : lang(make_language_namespace())
    , fs(fs ? fs : &null_file_system())
    , file_system_access(fs != nullptr)
  {
    Namespace io = make_io_namespace();
    namespaces.emplace(io.get_name(), std::move(io));

    if (has_file_system_access())
    {
      default_ns_source = std::make_unique<FileSystemNamespaceSource>(this->fs);
      namespace_loader = std::make_unique<NamespaceLoader>(default_ns_source.get());
    }
    switch_namespace(DEFAULT_NAMESPACE);
  }

  RuntimeOptions& Runtime::get_options()
  {
    return options;
  }

  const RuntimeOptions& Runtime::get_options() const
  {
    return options;
  }

  void Runtime::set_source_diagnostics(bool enabled)
  {
    options.source_diagnostics = enabled;
  }

  void Runtime::set_call_stack_diagnostics(bool enabled)
  {
    options.call_stack_diagnostics = enabled;
  }

  bool Runtime::source_diagnostics_enabled() const
  {
    return options.source_diagnostics || options.call_stack_diagnostics;
  }

  bool Runtime::call_stack_diagnostics_enabled() const
  {
    return options.call_stack_diagnostics;
  }

  std::string Runtime::describe_source(const SourceRef& source) const
  {
    return source_map.describe(source);
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
        current_namespace->get_origin().type == Namespace::Origin::Type::NATIVE &&
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

  void Runtime::register_namespace(std::unique_ptr<Namespace> ns)
  {
    if (!ns)
    {
      throw LispleException("Cannot register null namespace.");
    }
    if (ns->get_type() != Namespace::Type::USER)
    {
      throw LispleException("Provided namespace '" + ns->get_name() +
                            "' is of an invalid type.");
    }

    const std::string name = ns->get_name();
    auto result = namespaces.emplace(name, std::move(*ns));
    if (!result.second)
    {
      throw LispleException("Namespace '" + name + "' is already registered.");
    }
  }

  void Runtime::set_namespace_roots(std::vector<NamespaceRoot> namespace_roots)
  {
    if (!has_file_system_access() || !default_ns_source)
    {
      if (namespace_roots.empty())
      {
        return;
      }
      throw LispleException("Cannot configure namespace roots without file system access.");
    }

    auto* fs_source = dynamic_cast<FileSystemNamespaceSource*>(default_ns_source.get());
    if (!fs_source)
    {
      throw LispleException("Runtime namespace source does not support namespace roots.");
    }
    fs_source->set_namespace_roots(std::move(namespace_roots));
  }

  bool Runtime::has_file_system_access() const
  {
    return file_system_access;
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
    auto raw_file = fs->read(file_name);

    const std::string current_ns = get_current_namespace().get_name();
    if (namespace_loader)
    {
      namespace_loader->push_file_context(file_name);
    }
    auto restore_file_context = [&]()
    {
      if (namespace_loader)
      {
        namespace_loader->pop_file_context();
      }
      switch_namespace(current_ns);
    };

    try
    {
      eval(ctx, raw_file, file_name);
    }
    catch (const Lisple::ParseException& e)
    {
      restore_file_context();
      if (has_parse_file_context(e))
      {
        throw;
      }
      throw Lisple::ParseException("Error parsing '" + file_name + "': " + e.what());
    }
    catch (const Lisple::LispleException& e)
    {
      restore_file_context();
      throw Lisple::LispleException("Error reading '" + file_name + "': " + e.what());
    }
    if (namespace_loader)
    {
      namespace_loader->pop_file_context();
    }
    get_current_namespace().set_origin(Namespace::Origin::file(file_name));
    switch_namespace(current_ns);
  }

  sptr_val Runtime::eval(Context& ctx, const std::string& str)
  {
    return eval(ctx, str, "<eval>");
  }

  sptr_val Runtime::eval(const std::string& str, const std::string& source_name)
  {
    Context ctx(*this);
    return this->eval(ctx, str, source_name);
  }

  sptr_val Runtime::eval(Context& ctx,
                         const std::string& str,
                         const std::string& source_name)
  {
    uint32_t source_file_id = 0;
    if (source_diagnostics_enabled())
    {
      source_file_id = source_map.intern_file(source_name);
    }
    sptr_ast_node_v script =
      sexp_reader.read_sexps(str, source_file_id, source_diagnostics_enabled());

    sptr_val result;

    for (auto& sexp : script)
    {
      result = eval(ctx, sexp);
    }
    return result;
  }

  sptr_val Runtime::eval(const std::string& str)
  {
    Context ctx(*this);
    return this->eval(ctx, str);
  }

  sptr_val Runtime::eval(const sptr_ast_node& statement)
  {
    Context ctx(*this);
    return this->eval(ctx, statement);
  }

  sptr_val Runtime::eval(Context& ctx, const sptr_ast_node& form)
  {
    LowerContext lctx{&ctx};
    auto exec_node = lower_expr(lctx, form);
    return Lisple::exec(ctx, *exec_node);
  }

  sptr_val Runtime::invoke(const std::string& function, sptr_val_v& args)
  {
    Context ctx(*this);
    sptr_val inv = lookup(function);
    if (inv->type != Value::Type::FUNCTION)
    {
      throw new InvocationException(inv->to_string() + " is not executable.");
    }

    Executable& exec = inv->exec();
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

  sptr_val Runtime::lookup(const std::string& identifier, const sptr_val& default_value)
  {
    if (const sptr_val* value = find(identifier))
    {
      return *value;
    }
    return default_value;
  }

  const sptr_val* Runtime::find(const Value& identifier)
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return find(identifier.str());
  }

  const sptr_val* Runtime::find(const std::string& identifier_s)
  {
    AST::Symbol identifier(identifier_s);
    if (identifier.is_qualified())
    {
      Namespace* _ns = ns(identifier.get_qualifier());
      if (_ns)
      {
        return _ns->find(identifier.get_identifier());
      }

      if (const sptr_val* result = current_namespace->find(identifier.to_string()))
      {
        return result;
      }

      return nullptr;
    }

    if (const sptr_val* lang_obj = lang.find(identifier.get_identifier()))
    {
      return lang_obj;
    }

    return current_namespace->find(identifier.get_identifier());
  }

  const sptr_val& Runtime::lookup(const Value& identifier)
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  const sptr_val& Runtime::lookup(const std::string& identifier_s)
  {
    const sptr_val* value = find(identifier_s);
    if (!value)
    {
      throw IdentifierException("Unknown identifier: '" + identifier_s + "'");
    }
    return *value;
  }

  Namespace& Runtime::get_ns_of(const std::string& identifier)
  {
    if (current_namespace->has(identifier))
    {
      return *current_namespace;
    }
    else if (lang.has(identifier))
    {
      return lang;
    }

    throw IdentifierException("Unknown identifier: " + identifier);
  }

} // namespace Lisple
