
#include "roo/runtime.h"

#include <chrono>
#include <cstdint>
#include <exception>
#include <memory>
#include <random>
#include <stddef.h>
#include <utility>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/io/file_system.h>
#include <roo/io/file_system_namespace_source.h>
#include <roo/io/null_file_system.h>
#include <roo/lang.h>
#include <roo/lang/io/io_namespace.h>
#include <roo/namespace.h>
#include <roo/namespace_loader.h>
#include <roo/reader.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    NullFileSystem& null_file_system()
    {
      static NullFileSystem fs;
      return fs;
    }

    bool has_parse_file_context(const Roo::ParseException& e)
    {
      const std::string message = e.what();
      return message.compare(0, 15, "Error parsing '") == 0;
    }

    RandomState make_random_state()
    {
      std::random_device rd;
      const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
      std::seed_seq seed{rd(), rd(), rd(), rd(), static_cast<unsigned int>(now)};
      return RandomState(seed);
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
    : Runtime(fs,
              fs ? std::make_unique<FileSystemNamespaceSource>(fs)
                 : std::unique_ptr<NamespaceSource>())
  {
  }

  Runtime::Runtime(std::unique_ptr<NamespaceSource> namespace_source)
    : Runtime(nullptr, std::move(namespace_source))
  {
  }

  Runtime::Runtime(FileSystem* fs, std::unique_ptr<NamespaceSource> namespace_source)
    : lang(make_language_namespace())
    , fs(fs ? fs : &null_file_system())
    , file_system_access(fs != nullptr)
    , random_state(make_random_state())
  {
    Namespace io = make_io_namespace();
    namespaces.emplace(io.get_name(), std::move(io));
    Namespace legacy_io = make_legacy_io_namespace();
    namespaces.emplace(legacy_io.get_name(), std::move(legacy_io));

    set_namespace_source(std::move(namespace_source));
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
      throw RooException("Provided namespace '" + ns.get_name() +
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
        throw RooException("Provided namespace '" + ns.get_name() +
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
      throw RooException("Cannot register null namespace.");
    }
    if (ns->get_type() != Namespace::Type::USER)
    {
      throw RooException("Provided namespace '" + ns->get_name() +
                         "' is of an invalid type.");
    }

    const std::string name = ns->get_name();
    auto result = namespaces.emplace(name, std::move(*ns));
    if (!result.second)
    {
      throw RooException("Namespace '" + name + "' is already registered.");
    }
  }

  void Runtime::set_namespace_roots(std::vector<NamespaceRoot> namespace_roots)
  {
    if (!default_ns_source)
    {
      if (namespace_roots.empty())
      {
        return;
      }
      throw RooException("Cannot configure namespace roots without a namespace source.");
    }

    auto* configurable = dynamic_cast<NamespaceRootConfigurable*>(default_ns_source.get());
    if (!configurable)
    {
      throw RooException("Runtime namespace source does not support namespace roots.");
    }
    configurable->set_namespace_roots(std::move(namespace_roots));
  }

  void Runtime::set_namespace_source(std::unique_ptr<NamespaceSource> namespace_source)
  {
    default_ns_source = std::move(namespace_source);
    if (default_ns_source)
    {
      namespace_loader = std::make_unique<NamespaceLoader>(default_ns_source.get());
    }
    else
    {
      namespace_loader.reset();
    }
  }

  bool Runtime::has_file_system_access() const
  {
    return file_system_access;
  }

  bool Runtime::has_namespace_loading() const
  {
    return namespace_loader != nullptr;
  }

  Namespace& Runtime::get_current_namespace()
  {
    return *current_namespace;
  }

  void Runtime::seed_random(int seed)
  {
    random_state.seed(static_cast<RandomState::result_type>(seed));
  }

  int Runtime::random_int(int min, int max)
  {
    if (max < min)
    {
      throw RooException("rnd: max must be greater than or equal to min.");
    }
    if (min == max)
    {
      return min;
    }

    const uint64_t range =
      static_cast<uint64_t>(static_cast<int64_t>(max) - static_cast<int64_t>(min));
    const uint64_t engine_range =
      static_cast<uint64_t>(RandomState::max()) - RandomState::min() + 1;
    const uint64_t limit = engine_range - (engine_range % range);

    uint64_t value = 0;
    do
    {
      value = static_cast<uint64_t>(random_state()) - RandomState::min();
    } while (value >= limit);

    return static_cast<int>(static_cast<int64_t>(min) + static_cast<int64_t>(value % range));
  }

  RandomState Runtime::get_random_state() const
  {
    return random_state;
  }

  void Runtime::set_random_state(const RandomState& state)
  {
    random_state = state;
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
    catch (const Roo::ParseException& e)
    {
      restore_file_context();
      if (has_parse_file_context(e))
      {
        throw;
      }
      throw Roo::ParseException("Error parsing '" + file_name + "': " + e.what());
    }
    catch (const Roo::RooException& e)
    {
      restore_file_context();
      throw Roo::RooException("Error reading '" + file_name + "': " + e.what());
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
    return Roo::exec(ctx, *exec_node);
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

} // namespace Roo
