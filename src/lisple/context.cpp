
#include "context.h"

#include <utility>

#include "lisp_reader.h"

#include "form.h"
#include "namespace.h"
#include "scope.h"
#include "type.h"

#include "lisple_exception.h"

namespace Lisple
{
  ContextFrame::ContextFrame(bool evaluation_mode, Lisple::Scope& scope)
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

  sptr_sobject ContextFrame::lookup(const Lisple::Word& word) const
  {
    return scope.lookup(word);
  }

  bool ContextFrame::has(const Lisple::Word& word) const
  {
    return scope.has(word);
  }

  Context::Context(LispReader& reader)
    : reader(reader)
  {
    push_context(true);
  }

  Context::Context(const Context& other)
    : reader(other.reader)
  {
    for (auto& frame : other.frame_stack)
    {
      Scope clone(frame->scope);
      this->frame_stack.push_back(std::make_unique<ContextFrame>(frame->evaluation_mode, clone));
    }
  }

  Context::Context(LispReader& reader, frame_stack_t& frame_stack)
    : frame_stack(std::move(frame_stack))
    , reader(reader)
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

  std::shared_ptr<Lisple::Array> Context::get_scope_identifiers()
  {
    auto array = std::make_shared<Lisple::Array>();
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

  Lisple::sptr_sobject Context::eval(const sptr_sobject& form)
  {
    return reader.eval(*this, form);
  }

  Lisple::sptr_sobject Context::eval(const std::string& str)
  {
    return reader.eval(str);
  }

  void Context::read_file(const std::string& file_name)
  {
    reader.read_file(file_name);
  }

  void Context::switch_namespace(const std::string& namespace_name)
  {
    reader.switch_namespace(namespace_name);
  }

  Namespace* Context::get_current_namespace()
  {
    return &reader.get_current_namespace();
  }

  void Context::import_namespace(const std::string& namespace_name)
  {
    reader.import_namespace(namespace_name);
  }

  void Context::define_namespace_alias(const std::string& namespace_name, const std::string& alias)
  {
    reader.define_namespace_alias(namespace_name, alias);
  }

  void Context::store_namespace(Lisple::Word key, sptr_sobject value)
  {
    reader.get_current_namespace().store(key.value, value);
  }

  sptr_sobject Context::call(const std::string& fn_name, sptr_sobject& arg1)
  {
    sptr_sobject_v args = { arg1 };

    sptr_sobject exec = lookup(Lisple::Word(fn_name));

    if (*exec == *NIL)
    {
      throw InvocationException("Unknown identifier: " + fn_name);
    }
    return exec->execute(*this, args);
  }

  sptr_sobject Context::lookup(const Lisple::Word& identifier) const
  {
    if (identifier.is_qualified())
    {
      sptr_sobject result = reader.lookup(identifier);
      return result ? result : NIL;
    }

    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      Lisple::sptr_sobject res = i->get()->lookup(identifier);
      if (res.get())
      {
        return res;
      }
    }

    return reader.lookup(identifier);
  }

  Scope& Context::get_scope_of(const Lisple::Word& identifier) const
  {
    for (auto i = frame_stack.rbegin(); i != frame_stack.rend(); ++i)
    {
      if (i->get()->has(identifier))
      {
        return i->get()->scope;
      }
    }

    return reader.get_ns_of(identifier);
  }

  void Context::push_context(bool evaluation_mode)
  {
    frame_stack.push_back(std::make_unique<ContextFrame>(evaluation_mode));
  }

  void Context::push_context(bool evaluation_mode, Lisple::Scope& scope)
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
}
