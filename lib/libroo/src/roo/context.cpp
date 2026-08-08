
#include "roo/context.h"

#include <sstream>

#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/io/file_system.h>
#include <roo/namespace.h>
#include <roo/runtime.h>
#include <roo/runtime/value.h>
#include <roo/scope.h>
#include <roo/type.h>

namespace Roo
{
  /** ContextFrame */
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

  const sptr_val* ContextFrame::find(const std::string& identifier) const
  {
    return scope.find(identifier);
  }

  const sptr_val* ContextFrame::find(const Value& identifier) const
  {
    return scope.find(identifier);
  }

  const sptr_val& ContextFrame::lookup(const std::string& identifier) const
  {
    return scope.lookup(identifier);
  }

  const sptr_val& ContextFrame::lookup(const Value& identifier) const
  {
    return scope.lookup(identifier);
  }

  bool ContextFrame::has(const std::string& identifier) const
  {
    return scope.has(identifier);
  }

  std::string ContextFrame::to_string() const
  {
    std::stringstream s;
    s << "ContextFrame (" << this << ", eval_mode=" << evaluation_mode << ")" << std::endl;

    sptr_ast_node keys = scope.get_keys();
    for (auto& k : keys->get_children())
    {
      s << " - " << k->to_string() << ": " << scope.lookup(k->to_string())->to_string()
        << std::endl;
    }

    return s.str();
  }

  /** Context */
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

  std::unique_ptr<Context> Context::isolate() const
  {
    return std::make_unique<Context>(runtime);
  }

  std::vector<std::unique_ptr<ContextFrame>>& Context::get_stack_frames()
  {
    return frame_stack;
  }

  std::shared_ptr<AST::Vector> Context::get_scope_identifiers()
  {
    auto vector = std::make_shared<AST::Vector>();
    for (auto& frame : frame_stack)
    {
      for (auto& key : frame->scope.get_keys()->get_children())
      {
        vector->append(key);
      }
    }
    return vector;
  }

  bool Context::evalp() const
  {
    return frame_stack.back()->evalp();
  }

  size_t Context::stack_size() const
  {
    return frame_stack.size();
  }

  sptr_val Context::eval(const sptr_ast_node& form)
  {
    return runtime.eval(*this, form);
  }

  sptr_val Context::eval(const sptr_val& value)
  {
    return runtime.eval(*this, to_AST(*value));
  }

  sptr_val Context::eval(const std::string& str)
  {
    return runtime.eval(*this, str);
  }

  sptr_val Context::call(const std::string& fn_name, const sptr_val& arg)
  {
    sptr_val_v args{arg};
    return call(fn_name, args);
  }

  sptr_val Context::call(const std::string& fn_name, const sptr_val_v& args)
  {
    sptr_val inv = lookup(fn_name);
    if (inv->type != Value::Type::FUNCTION)
    {
      throw InvocationException(inv->to_string() + " is not executable.");
    }

    Executable& exec = inv->exec();
    sptr_val_v mutable_args = args;

    try
    {
      return exec.execute(*this, mutable_args);
    }
    catch (std::exception& e)
    {
      throw InvocationException("Error while calling " + fn_name + ":\n" + inv->to_string() +
                                "\n" + e.what());
    }
  }

  void Context::read_file(const std::string& file_name)
  {
    runtime.read_file(file_name);
  }

  FileSystem& Context::file_system() const
  {
    return *runtime.fs;
  }

  void Context::switch_namespace(const std::string& namespace_name)
  {
    runtime.switch_namespace(namespace_name);
  }

  Namespace* Context::get_current_namespace()
  {
    return &runtime.get_current_namespace();
  }

  Namespace& Context::lang()
  {
    return runtime.language_namespace();
  }

  void Context::seed_random(int seed)
  {
    runtime.seed_random(seed);
  }

  int Context::random_int(int min, int max)
  {
    return runtime.random_int(min, max);
  }

  RandomState Context::get_random_state() const
  {
    return runtime.get_random_state();
  }

  void Context::set_random_state(const RandomState& state)
  {
    runtime.set_random_state(state);
  }

  bool Context::source_diagnostics_enabled() const
  {
    return runtime.source_diagnostics_enabled();
  }

  bool Context::call_stack_diagnostics_enabled() const
  {
    return runtime.call_stack_diagnostics_enabled();
  }

  std::string Context::describe_source(const SourceRef& source) const
  {
    return runtime.describe_source(source);
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

  void Context::store_namespace(const std::string& symbol, const sptr_val& value)
  {
    runtime.get_current_namespace().store(symbol, value);
  }

  const sptr_val* Context::find(const std::string& identifier) const
  {
    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      const sptr_val* res = i->get()->find(identifier);
      if (res)
      {
        return res;
      }
    }

    return runtime.find(identifier);
  }

  const sptr_val* Context::find(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return find(identifier.str());
  }

  const sptr_val& Context::lookup(const std::string& identifier) const
  {
    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      if (const sptr_val* res = i->get()->find(identifier))
      {
        return *res;
      }
    }

    return runtime.lookup(identifier);
  }

  const sptr_val& Context::lookup(const Value& identifier) const
  {
    if (identifier.type != Value::Type::SYMBOL)
    {
      throw TypeError("Cannot lookup non-symbol identifier: " + identifier.to_string());
    }

    return lookup(identifier.str());
  }

  Scope& Context::current_scope()
  {
    return frame_stack.back()->scope;
  }

  Scope& Context::get_scope_of(const std::string& identifier) const
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
      throw RooException("Attempt to pop bottom stack frame");
    }
    frame_stack.pop_back();
  }
} // namespace Roo
