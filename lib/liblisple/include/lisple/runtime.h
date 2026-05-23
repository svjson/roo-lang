
#ifndef __RUNTIME_H_
#define __RUNTIME_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <lisple/namespace.h>
#include <lisple/namespace_source.h>
#include <lisple/random.h>
#include <lisple/reader.h>
#include <lisple/source.h>
#include <lisple/type.h>

namespace Lisple
{
  class Context;
  namespace AST
  {
    class Vector;
    class List;
    class Map;
    class Seq;
    class Symbol;
  } // namespace AST
  class FileSystem;
  class NamespaceLoader;
  class NamespaceSource;

  class Runtime
  {
   private:
    /*!
     * @brief The Lisple language namespace, containing all language built-in
     * functions and identifiers
     */
    Namespace lang;
    FileSystem* fs;
    bool file_system_access = false;
    Reader sexp_reader;
    RuntimeOptions options;
    SourceMap source_map;
    RandomState random_state;

    std::map<const std::string, Namespace> namespaces;

    Namespace* current_namespace = nullptr;

    std::unique_ptr<NamespaceSource> default_ns_source;
    std::unique_ptr<NamespaceLoader> namespace_loader;

   public:
    ~Runtime();
    Runtime(Runtime&&);
    /*!
     * @brief Creates a vanilla Lisple runtime with only the language built-in
     * functions and no file system access.
     */
    Runtime();

    /*!
     * @brief Creates a vanilla Lisple runtime with only the language built-in
     * functions and file system access via the provided FileSystem
     * implementation
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(FileSystem* fs);

    /*!
     * @brief Creates a vanilla Lisple runtime with independently configured
     * application file system access and namespace loading.
     *
     * The FileSystem is used by application-visible file IO and read_file.
     * The NamespaceSource is used only for on-demand namespace loading. Passing
     * nullptr as the FileSystem disables application file system access while
     * still allowing namespace loading through the provided source.
     */
    Runtime(FileSystem* fs, std::unique_ptr<NamespaceSource> namespace_source);

    /*!
     * @brief Creates a vanilla Lisple runtime with no application file system
     * access, but with on-demand namespace loading through the provided source.
     */
    explicit Runtime(std::unique_ptr<NamespaceSource> namespace_source);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespace in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespace will typically be the host application namespace.
     * The Runtime instance will assume ownership of the Namespace instance
     */
    Runtime(Namespace& ns);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespace in
     * addition to the language built-in functions and file system access via
     * the provided FileSystem implementation.
     *
     * The provided namespace will typically be the host application namespace.
     * The Runtime instance will assume ownership of the Namespace instance
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(Namespace& host_namespace, FileSystem* fs);

    /*!
     * @brief Creates a Lisple runtime with host-provided namespaces in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     */
    Runtime(std::map<const std::string, Namespace> namespaces);

    /*!
     * @brief Creates a Lisple runtime with host-provided namespaces in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     */
    Runtime(const std::string& main_ns, std::map<const std::string, Namespace> namespaces);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespaces in
     * addition to the language built-in functions and file system access via
     * the provided FileSystem implementation.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(const std::string& main_ns,
            std::map<const std::string, Namespace> namespaces,
            FileSystem* fs);

    Runtime(std::vector<std::unique_ptr<Namespace>> namespaces, FileSystem* fs);

    Runtime(const std::string& main_ns,
            std::vector<std::unique_ptr<Namespace>> namespaces,
            FileSystem* fs);

    /*!
     * @brief Switch the namespace in which the lisple engine currently operates.
     * If the namespace does not currently exist, it will be created.
     *
     * If the previous namespace is empty, it will be discarded.
     */
    void switch_namespace(const std::string& ns);

    /*!
     * @brief Returns a reference to a namespace without switching the namespace
     * within the reader.
     *
     * Optionally, create it if it exists. If this is done, it is up to
     * the client/host program to evict it if it is empty.
     *
     * If not created, this method will return nullptr.
     */
    Namespace* ns(const std::string& name, bool create_if_absent = false);

    /*!
     * @brief Register a fully constructed user namespace with this runtime.
     *
     * This is intended for host/native integrations that create namespaces after
     * runtime construction, including dynamically loaded native packages.
     */
    void register_namespace(std::unique_ptr<Namespace> ns);

    /*!
     * @brief Configure explicit namespace prefix roots for file namespace loading.
     *
     * Namespace roots are consulted before relative inference and before the
     * default dotted namespace-to-path mapping.
     */
    void set_namespace_roots(std::vector<NamespaceRoot> namespace_roots);

    /*!
     * @brief Replace the runtime namespace source.
     *
     * Namespace sources are used only for namespace resolution/imports. They do
     * not grant application-visible file IO access.
     */
    void set_namespace_source(std::unique_ptr<NamespaceSource> namespace_source);

    /*!
     * @brief Returns a reference to the current namespace of the reader context
     */
    Namespace& get_current_namespace();

    /*!
     * @brief Reseed the runtime-local random generator.
     *
     * Use this when deterministic random behavior is needed for tests,
     * simulations, or reproducible application runs.
     */
    void seed_random(int seed);

    /*!
     * @brief Return a random integer in the half-open range [min, max).
     */
    int random_int(int min, int max);

    /*!
     * @brief Snapshot and restore the runtime-local random generator state.
     *
     * These are primarily used by scoped random-seed forms.
     */
    RandomState get_random_state() const;
    void set_random_state(const RandomState& state);

    /*!
     * @brief Tests if this Runtime instance may access any file system
     * abstraction, and thus can read files from disk or virtual a file system.
     */
    bool has_file_system_access() const;

    /*!
     * @brief Returns true when this runtime can resolve namespaces on demand.
     */
    bool has_namespace_loading() const;

    void read_file(const std::string& file_name);
    void read_file(Context& ctx, const std::string& file_name);

    sptr_val eval(const std::string& str);
    sptr_val eval(const std::string& str, const std::string& source_name);
    sptr_val eval(Context& ctx, const std::string& str);
    sptr_val eval(Context& ctx, const std::string& str, const std::string& source_name);
    sptr_val eval(const sptr_ast_node& sexp);
    sptr_val eval(Context& ctx, const sptr_ast_node& sexp);

    sptr_val invoke(const std::string& function, sptr_val_v& args);

    /*!
     * @brief Return the mutable runtime options object.
     *
     * Runtime options control optional behavior that affects evaluation,
     * diagnostics, or host integration. The default options keep source
     * diagnostics disabled.
     */
    RuntimeOptions& get_options();

    /*!
     * @brief Return the immutable runtime options object.
     */
    const RuntimeOptions& get_options() const;

    /*!
     * @brief Enable or disable source-location diagnostics.
     *
     * When enabled, code read through this Runtime records compact source
     * references while lexing, parsing, and lowering. Invocation errors can then
     * include file, line, and column information for the failing call site.
     *
     * This is disabled by default to avoid the extra position-tracking work when
     * diagnostics are not needed.
     */
    void set_source_diagnostics(bool enabled);

    /*!
     * @brief Enable or disable call-stack diagnostics.
     *
     * When enabled, invocation errors are annotated with Lisple call context,
     * including the callee name and source location when available. This option
     * also enables source diagnostics, because call-stack diagnostics require
     * source references to report useful file and line information.
     *
     * This is disabled by default. It is intended for CLI tools, REPLs, tests,
     * and development builds where actionable error messages are more important
     * than the small amount of additional diagnostic bookkeeping.
     */
    void set_call_stack_diagnostics(bool enabled);

    /*!
     * @brief Return true if source-location diagnostics are active.
     *
     * This is true when either source diagnostics or call-stack diagnostics are
     * enabled.
     */
    bool source_diagnostics_enabled() const;

    /*!
     * @brief Return true if call-stack diagnostics are active.
     */
    bool call_stack_diagnostics_enabled() const;

    /*!
     * @brief Format a source reference as file:line:column.
     */
    std::string describe_source(const SourceRef& source) const;

    const sptr_val* find(const std::string& identifier);
    const sptr_val* find(const Value& identifier);
    const sptr_val& lookup(const std::string& identifier);
    const sptr_val& lookup(const Value& identifier);
    sptr_val lookup(const std::string& identifier, const Lisple::sptr_val& default_value);
    Namespace& get_ns_of(const std::string& identifier);

   private:
    /*!
     * @brief If a namespace loader is configured, attempt to load the named
     * namespace on demand before the caller checks for its existence.
     */
    void ensure_namespace_loaded(const std::string& ns_name);

    /*!
     * @brief Import another namespace into the current namespace, making all
     * identifiers of that namespace available to the current.
     *
     * This does not copy anything into the current namespace, but the
     * imported namespace is referenced. This means that any changes
     * to the imported namespace will be reflected in the current.
     *
     * Any duplicate identifiers between the namespace will cause the
     * the identifiers from the imported one to be shadowed and
     * inacceessible from the importing namespace.
     *
     * Importing a namespace does not trigger loading of new namespaces -
     * any namespaces to be imported must already be loaded.
     */
    void import_namespace(const std::string& namespace_name);

    /*!
     * @brief Define an alias for another namespace within the current
     * namespace.
     */
    void define_namespace_alias(const std::string& ns_name, const std::string& alias);

    friend class Context;
  };
} // namespace Lisple

#endif
