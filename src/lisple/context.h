
#ifndef __LISPLE_CONTEXT_H_
#define __LISPLE_CONTEXT_H_

#include <algorithm>
#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include "form.h"
#include "scope.h"
#include "type.h"

namespace Lisple
{
  class LispReader;
  class Namespace;

  class ContextFrame
  {
   public:
    bool evaluation_mode = true;
    Lisple::Scope scope;

    ContextFrame(bool evaluation_mode, Lisple::Scope& scope);
    ContextFrame(bool evaluation_mode);

    bool evalp() const;
    sptr_sobject lookup(const Lisple::Word& word) const;
    bool has(const Lisple::Word& word) const;
  };

  typedef std::vector<std::unique_ptr<ContextFrame>> frame_stack_t;

  class Context
  {
   private:
    frame_stack_t frame_stack;
    LispReader& reader;

   public:
    Context(LispReader& reader);
    Context(LispReader& reader, frame_stack_t& frame_stack);

    std::shared_ptr<Context> detach() const;

    bool evalp() const;

    size_t stack_size() const;

    std::vector<std::unique_ptr<ContextFrame>>& get_stack_frames();
    std::shared_ptr<Lisple::Array> get_scope_identifiers();

    Lisple::sptr_sobject eval(const std::string& str);
    Lisple::sptr_sobject eval(const Lisple::sptr_sobject& list);

    Scope& get_scope_of(const Lisple::Word& identifier) const;

    void read_file(const std::string& file_name);
    void store_namespace(Lisple::Word key, Lisple::sptr_sobject value);

    /*!
     * \brief Switch the underlying namespace in which execution and unqualified
     * identifier lookup occurs.
     *
     * This switches the current namespace of lisp reader instance, and any code
     * that uses this function to temporarily switch namespace implicitly takes
     * on the responsibility of switching it back.
     *
     * Should typically not be called by client code.
     */
    void switch_namespace(const std::string& namespace_name);

    /*!
     * \brief Get a pointer to the currently active namespace of the underlying
     * lisp reader instance.
     */
    Namespace* get_current_namespace();

    sptr_sobject call(const std::string& fn_name, sptr_sobject& args);
    sptr_sobject lookup(const Lisple::Word& word) const;

    void push_context(bool evaluation_mode);
    void push_context(bool evaluation_mode, Lisple::Scope& scope);

    void pop_context();

   private:
    /**
     * \brief Import another namespace into the current namespace, making all
     * identifiers of that namespace available to the current.
     *
     * @see LispReader::import_namespace for more information.
     */
    void import_namespace(const std::string& ns_name);

    /**
     * \brief Define an alias for another namespace within the current
     * namespace.
     *
     * @see LispReader::define_namespace_alias for more information.
     */
    void define_namespace_alias(const std::string& ns_name, const std::string& alias);

    friend class NsMacro;
  };
}

#endif
