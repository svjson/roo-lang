
#ifndef __LISPLE_CONTEXT_H_
#define __LISPLE_CONTEXT_H_

#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include <lisple/form.h>
#include <lisple/scope.h>
#include <lisple/type.h>

namespace Lisple
{
  class Runtime;
  class Namespace;

  class ContextFrame
  {
   public:
    bool evaluation_mode = true;
    Scope scope;

    ContextFrame(bool evaluation_mode, Scope& scope);
    ContextFrame(bool evaluation_mode);

    bool evalp() const;
    sptr_rtval lookup_value(const Word& word) const;
    sptr_sobject lookup(const Word& word) const;
    bool has(const Word& word) const;

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

    bool evalp() const;

    size_t stack_size() const;

    std::vector<std::unique_ptr<ContextFrame>>& get_stack_frames();
    std::shared_ptr<Array> get_scope_identifiers();

    sptr_rtval eval(const std::string& str);
    sptr_rtval eval(const sptr_sobject& list);
    sptr_sobject eval_ast(const sptr_sobject& list);

    Scope& current_scope();
    Scope& get_scope_of(const Word& identifier) const;

    void read_file(const std::string& file_name);
    void store_namespace(Word key, sptr_sobject value);
    void store_namespace(const std::string& symbol, sptr_rtval& value);

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

    /*!
     * @brief Call a named function with a single argument
     *
     * @param fn_name The name of the function to look up and execute
     *
     * @return The execution result as a shared object.
     *
     * The lookup of the named function occurs in the current namespace of
     * the context, which means that aliased namespaces in the current
     * namespace will be honored.
     *
     * If the identifier is qualified with a known global namespace, the
     * current namespace will be ignored.
     */
    sptr_sobject call(const std::string& fn_name, sptr_sobject& arg);
    /*!
     * @brief Call a named function with a list of arguments.
     *
     * @param fn_name The name of the function to look up and execute
     *
     * @return The execution result as a shared object.
     *
     * The lookup of the named function occurs in the current namespace of
     * the context, which means that aliased namespaces in the current
     * namespace will be honored.
     *
     * If the identifier is qualified with a known global namespace, the
     * current namespace will be ignored.
     */
    sptr_sobject call(const std::string& fn_name, const sptr_sobject_v& args);
    sptr_sobject lookup(const Word& word) const;
    sptr_rtval lookup_value(const Word& word) const;
    sptr_rtval lookup_value(const std::string& identifier) const;

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
} // namespace Lisple

#endif
