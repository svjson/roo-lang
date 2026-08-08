
#ifndef __ROO_CONTEXT_H_
#define __ROO_CONTEXT_H_

#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include <roo/form.h>
#include <roo/random.h>
#include <roo/scope.h>
#include <roo/source.h>
#include <roo/type.h>

namespace Roo
{
  class Runtime;
  class Namespace;
  class FileSystem;

  class ContextFrame
  {
   public:
    bool evaluation_mode = true;
    Scope scope;

    ContextFrame(bool evaluation_mode, Scope& scope);
    ContextFrame(bool evaluation_mode);

    bool evalp() const;
    const sptr_val* find(const std::string& identifier) const;
    const sptr_val* find(const Value& identifier) const;
    const sptr_val& lookup(const std::string& identifier) const;
    const sptr_val& lookup(const Value& identifier) const;
    bool has(const std::string& identifier) const;

    std::string to_string() const;
  };

  typedef std::vector<std::unique_ptr<ContextFrame>> frame_stack_t;

  class Context
  {
   private:
    frame_stack_t frame_stack;
    Runtime& runtime;

   public:
    Context(const Context& other);
    Context(Runtime& runtime);
    Context(Runtime& runtime, frame_stack_t& frame_stack);

    std::shared_ptr<Context> detach() const;
    std::unique_ptr<Context> isolate() const;

    bool evalp() const;

    size_t stack_size() const;

    std::vector<std::unique_ptr<ContextFrame>>& get_stack_frames();
    std::shared_ptr<AST::Vector> get_scope_identifiers();

    sptr_val eval(const std::string& str);
    sptr_val eval(const sptr_val& list);
    sptr_val eval(const sptr_ast_node& list);

    /*!
     * @brief Call a named function with a single Value argument.
     *
     * @param fn_name The name of the function to look up and execute.
     *
     * @return The execution result as an Value.
     *
     * The lookup of the named function occurs in this context, so local
     * bindings, the current namespace, imported namespaces, and aliased
     * namespaces are honored.
     *
     * If the identifier is qualified with a known global namespace, the
     * current namespace will be ignored.
     */
    sptr_val call(const std::string& fn_name, const sptr_val& arg);

    /*!
     * @brief Call a named function with a list of Value arguments.
     *
     * @param fn_name The name of the function to look up and execute.
     *
     * @return The execution result as an Value.
     *
     * The lookup of the named function occurs in this context, so local
     * bindings, the current namespace, imported namespaces, and aliased
     * namespaces are honored.
     *
     * If the identifier is qualified with a known global namespace, the
     * current namespace will be ignored.
     */
    sptr_val call(const std::string& fn_name, const sptr_val_v& args);

    Scope& current_scope();
    Scope& get_scope_of(const std::string& identifier) const;

    void read_file(const std::string& file_name);
    FileSystem& file_system() const;
    void store_namespace(const std::string& symbol, const sptr_val& value);

    /*!
     * @brief Switch the underlying namespace in which execution and unqualified
     * identifier lookup occurs.
     *
     * @param namespace_name The name of the namespace to switch to.
     *
     * This switches the current namespace of  instance, and any code
     * that uses this function to temporarily switch namespace implicitly takes
     * on the responsibility of switching it back.
     *
     * Should typically not be called by client code.
     */
    void switch_namespace(const std::string& namespace_name);

    /*!
     * @brief Get a pointer to the currently active namespace of the underlying
     * lisp runtime.
     *
     * @return A pointer to the current namespace
     */
    Namespace* get_current_namespace();

    Namespace& lang();
    void seed_random(int seed);
    int random_int(int min, int max);
    RandomState get_random_state() const;
    void set_random_state(const RandomState& state);
    bool source_diagnostics_enabled() const;
    bool call_stack_diagnostics_enabled() const;
    std::string describe_source(const SourceRef& source) const;

    const sptr_val* find(const std::string& identifier) const;
    const sptr_val* find(const Value& identifier) const;
    const sptr_val& lookup(const std::string& identifier) const;
    const sptr_val& lookup(const Value& identifier) const;

    void push_context(bool evaluation_mode);
    void push_context(bool evaluation_mode, Scope& scope);

    void pop_context();

   private:
    /*!
     * @brief Import another namespace into the current namespace, making all
     * identifiers of that namespace available to the current.
     *
     * @see Runtime::import_namespace for more information.
     */
    void import_namespace(const std::string& ns_name);

    /*!
     * @brief Define an alias for another namespace within the current
     * namespace.
     *
     * @see Runtime::define_namespace_alias for more information.
     */
    void define_namespace_alias(const std::string& ns_name, const std::string& alias);

    friend class NsForm;
  };
} // namespace Roo

#endif
