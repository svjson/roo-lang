
#include "context.h"

#include "exception.h"
#include "form.h"
#include "namespace.h"
#include "runtime.h"
#include "scope.h"
#include "type.h"
#include <utility>

namespace Lisple
{
  ContextFrame::ContextFrame(bool evaluation_mode, Scope& scope)
    : evaluation_mode(evaluation_mode)
    , scope(std::move(scope))
  {
  }

  ContextFrame::ContextFrame(bool evaluation_mode)
    : evaluation_mode(evaluation_mode)
  {
  }

  bool ContextFrame::evalp() const
  {
    return evaluation_mode;
  }

  sptr_sobject ContextFrame::lookup(const Word& word) const
  {
    return scope.lookup(word);
  }

  bool ContextFrame::has(const Word& word) const
  {
    return scope.has(word);
  }

  Context::Context(Runtime& runtime)
    : runtime(runtime)
  {
    push_context(true);
  }

  Context::Context(const Context& other)
    : runtime(other.runtime)
  {
    for (auto& frame : other.frame_stack)
    {
      Scope clone(frame->scope);
      this->frame_stack.push_back(
        std::make_unique<ContextFrame>(frame->evaluation_mode, clone));
    }
  }

  Context::Context(Runtime& runtime, frame_stack_t& frame_stack)
    : frame_stack(std::move(frame_stack))
    , runtime(runtime)
  {
  }

  std::shared_ptr<Context> Context::detach() const
  {
    return std::make_shared<Context>(*this);
  }

  std::vector<std::unique_ptr<ContextFrame>>& Context::get_stack_frames()
  {
    return frame_stack;
  }

  std::shared_ptr<Array> Context::get_scope_identifiers()
  {
    auto array = std::make_shared<Array>();
    for (auto& frame : frame_stack)
    {
      for (auto& key : frame->scope.get_keys()->get_children())
      {
        array->append(key);
      }
    }
    return array;
  }

  bool Context::evalp() const
  {
    return frame_stack.back()->evalp();
  }

  size_t Context::stack_size() const
  {
    return frame_stack.size();
  }

  sptr_sobject Context::eval(const sptr_sobject& form)
  {
    return runtime.eval(*this, form);
  }

  sptr_sobject Context::eval(const std::string& str)
  {
    return runtime.eval(str);
  }

  void Context::read_file(const std::string& file_name)
  {
    runtime.read_file(file_name);
  }

  void Context::switch_namespace(const std::string& namespace_name)
  {
    runtime.switch_namespace(namespace_name);
  }

  Namespace* Context::get_current_namespace()
  {
    return &runtime.get_current_namespace();
  }

  void Context::import_namespace(const std::string& namespace_name)
  {
    runtime.import_namespace(namespace_name);
  }

  void Context::define_namespace_alias(const std::string& namespace_name,
                                       const std::string& alias)
  {
    runtime.define_namespace_alias(namespace_name, alias);
  }

  void Context::store_namespace(Word key, sptr_sobject value)
  {
    runtime.get_current_namespace().store(key.value, value);
  }

  void Context::store_namespace(const std::string& symbol, sptr_rtval& value)
  {
    runtime.get_current_namespace().store(symbol, value);
  }

  sptr_sobject Context::call(const std::string& fn_name, sptr_sobject& arg1)
  {
    return this->call(fn_name, sptr_sobject_v{arg1});
  }

  sptr_sobject Context::call(const std::string& fn_name, const sptr_sobject_v& args)
  {
    sptr_sobject exec = lookup(Word(fn_name));

    if (*exec == *NIL)
    {
      throw IdentifierException("Unknown identifier: '" + fn_name + "'");
    }
    return exec->execute(*this, const_cast<sptr_sobject_v&>(args));
  }

  sptr_sobject Context::lookup(const Word& identifier) const
  {
    if (identifier.is_qualified())
    {
      sptr_sobject result = runtime.lookup(identifier);
      return result ? result : NIL;
    }

    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      sptr_sobject res = i->get()->lookup(identifier);
      if (res.get())
      {
        return res;
      }
    }

    return runtime.lookup(identifier);
  }

  Scope& Context::current_scope()
  {
    return frame_stack.back()->scope;
  }

  Scope& Context::get_scope_of(const Word& identifier) const
  {
    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      if (i->get()->has(identifier))
      {
        return i->get()->scope;
      }
    }

    return runtime.get_ns_of(identifier);
  }

  void Context::push_context(bool evaluation_mode)
  {
    frame_stack.push_back(std::make_unique<ContextFrame>(evaluation_mode));
  }

  void Context::push_context(bool evaluation_mode, Scope& scope)
  {
    frame_stack.push_back(std::make_unique<ContextFrame>(evaluation_mode, scope));
  }

  void Context::pop_context()
  {
    if (frame_stack.size() == 1)
    {
      throw LispleException("Attempt to pop bottom stack frame");
    }
    frame_stack.pop_back();
  }
} // namespace Lisple
