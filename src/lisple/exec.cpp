
#include "exec.h"

#include <memory>
#include <sstream>
#include <utility>

#include "form.h"
#include "context.h"
#include "namespace.h"
#include "scope.h"
#include "type.h"
#include "lisple_exception.h"

namespace Lisple
{
  ArgumentBinding::ArgumentBinding() {}

  std::unique_ptr<ArgumentBinding> ArgumentBinding::create(Object& arg_declaration)
  {
    if (arg_declaration.get_type() == Form::WORD)
    {
      return std::make_unique<NamedArgumentBinding>(Value<std::string>::value_of(arg_declaration));
    }

    if (arg_declaration.get_type() == Form::MAP)
    {
      return std::make_unique<DestructuringArgumentBinding>(arg_declaration.as<Map>());
    }

    throw LispleException("Invalid argument declaration: " + arg_declaration.to_string());
  }

  NamedArgumentBinding::NamedArgumentBinding(const std::string& name)
    : ArgumentBinding()
    , arg_name(name)
  {}

  void NamedArgumentBinding::apply(Scope& scope, sptr_sobject& arg_val)
  {
    scope.store(Word(arg_name), arg_val);
  }

  const Key keys("keys");
  const Key as("as");

  DestructuringArgumentBinding::DestructuringArgumentBinding(Map& destr_map)
    : ArgumentBinding()
    , destr_map(destr_map)
  {
    if (!destr_map.has_key(keys) ||
        destr_map.keys().size() > 2 ||
        (destr_map.keys().size() == 2 && !destr_map.has_key(as)))
    {
      throw Lisple::TypeError("Invalid destructuring map: " + destr_map.to_string());
    }
  }

  void DestructuringArgumentBinding::apply(Scope& scope, sptr_sobject& arg_val)
  {
    for (auto& key_obj : destr_map.get_sptr_property(keys)->as<Array>().get_children())
    {
      Word& key_name = key_obj->as<Word>();
      Key key(key_name.to_string());
      if (arg_val->has_key(key))
      {
        scope.store(key_name, arg_val->get_sptr_property(key));
      }
      else
      {
        scope.store(key_name, NIL);
      }
    }

    if (destr_map.has_key(as))
    {
      Object& alias = destr_map.get_property(as);
      if (alias.get_type() != Form::WORD)
      {
        throw LispleException("Invalid alias destructuring form: " + destr_map.to_string());
      }
      Word& alias_word = alias.as<Word>();
      scope.store(alias_word, arg_val);
    }
  }

  Argument::Argument(const TypeRef* type)
    : Argument(false, type, EVAL)
  {
  }

  Argument::Argument(const TypeRef* type, eval_mode eval)
    : Argument(false, type, eval)
  {

  }

  Argument::Argument(vararg_mode var, const TypeRef* type)
    : Argument(var, type, EVAL)
  {
  }

  Argument::Argument(vararg_mode var, const TypeRef* type, bool eval)
    : type(type)
    , eval(eval)
    , varargs(var)
  {
  }

  bool Argument::matches(Object& obj) const
  {
    return type->is_type_of(obj);
  }

  CoercionResult Argument::coerce(Context& ctx, sptr_sobject& obj) const
  {
    return type->coerce(ctx, obj);
  }

  bool Argument::evalp() const
  {
    return eval;
  }

  bool Argument::is_vararg() const
  {
    return varargs;
  }

  std::string Argument::to_string() const
  {
    return this->is_vararg()
      ? type->to_string() + "..."
      : type->to_string();
  }

  Signature::Signature(arg_v args, exec_fn target_func)
    : arguments(args)
    , target_func(target_func)
  {
  }

  const std::vector<Argument>& Signature::get_arguments() const
  {
    return arguments;
  }

  sptr_sobject_v Signature::coerce_args(Context& ctx, sptr_sobject_v& args)
  {
    sptr_sobject_v coerced;

    // Non-vararg signatures
    if (args.size() == arguments.size())
    {
      for (size_t i = 0; i < arguments.size(); i++)
      {
        const Argument& arg = arguments.at(i);
        if (arg.matches(*args.at(i)))
        {
          coerced.push_back(args.at(i));
          continue;
        }
        else
        {
          CoercionResult coercion = arg.coerce(ctx, args.at(i));
          if (coercion.success)
          {
            coerced.push_back(coercion.result);
          }
          else
          {
            coerced.clear();
            return coerced;
          }
        }
      }
    }

    return coerced;
  }


  bool Signature::matches(const sptr_sobject_v& args) const
  {
    if (arguments.empty() || (!arguments.back().is_vararg() && !arguments.front().is_vararg()))
    {
      if (arguments.size() != args.size())
      {
        return false;
      }
    }

    if (arguments.front().is_vararg() && arguments.size() != args.size() && arguments.size() > 1)
    {
      bool vararg_end = false;
      size_t arg_pos = 0;

      for (size_t i = 0; i < args.size(); i++) {
        if (!arguments.at(arg_pos).matches(*args.at(i)))
        {
          if (vararg_end)
          {
            return false;
          }
          else
          {
            vararg_end = true;
            arg_pos++;
            i--;
          }
        }
      }
      return true;
    }

    for (size_t i = 0; i < arguments.size() && i < args.size(); i++)
    {
      if (!arguments.at(i).matches(*args.at(i)))
      {
        return false;
      }
    }

    if (!args.empty() && !arguments.empty() && arguments.back().is_vararg())
    {
      for (size_t i = arguments.size(); i < args.size(); i++)
      {
        if (!arguments.back().matches(*args.at(i)))
        {
          return false;
        }
      }
    }

    return true;
  }

  bool Signature::should_eval_arg(std::size_t index) const
  {
    if (index < arguments.size())
    {
      return arguments.at(index).evalp();
    }
    else if (arguments.back().is_vararg())
    {
      return arguments.back().evalp();
    }
    throw InvocationException("Invalid argument index: " + std::to_string(index) + ". Are varargs correcly applied?");
  }

  sptr_sobject Signature::invoke(Context& ctx, sptr_sobject_v& args)
  {
    return target_func(ctx, args);
  }

  std::string Signature::to_string() const
  {
    std::stringstream ss;
    ss << "[";

    for (size_t i=0; i<arguments.size(); i++)
    {
      if (i>0)
      {
        ss << ", ";
      }
      ss << arguments.at(i).to_string();
    }

    ss << "]";

    return ss.str();
  }

  Executable::Executable(Form type, uptr_sig_v signatures)
    : Object(type)
    , signatures(std::move(signatures))
  {

  }

  Executable::Executable(Form type, uptr_sig signature)
    : Object(type)
  {
    signatures.push_back(std::move(signature));
  }

  bool Executable::operator==(const Lisple::Object& other) const
  {
    return this == &other;
  }

  Lisple::sptr_sobject Executable::execute(Context& ctx, sptr_sobject_v& args)
  {
    for (auto& signature : signatures)
    {
      if (signature->matches(args))
      {
        return signature->invoke(ctx, args);
      }
    }

    for (auto& signature : signatures)
    {
      sptr_sobject_v coerced_args = signature->coerce_args(ctx, args);
      if (coerced_args.size())
      {
        return signature->invoke(ctx, coerced_args);
      }
    }

    std::string expected;
    if (signatures.size() == 1)
    {
      expected = "Expected: " + signatures.front()->to_string();
    }
    else
    {
      expected = "Expected one of:";
      for (auto& signature : signatures)
      {
        expected += " " + signature->to_string();
      }
    }

    throw InvocationException("No matching signature: " + Array(args).to_string() + ". " + expected);
  }

  Function::Function(uptr_sig signature)
    : Executable(Form::FUNCTION, std::move(signature))
  {
  }

  Function::Function(uptr_sig_v signatures)
    : Executable(Form::FUNCTION, std::move(signatures))
  {
  }

  const std::vector<std::unique_ptr<Signature>>& Function::get_signatures() const
  {
    return signatures;
  }

  std::string Function::to_string() const
  {
    return "<fn>";
  }

  DetachedFunction::DetachedFunction(std::shared_ptr<Context> ctx,
                                     std::shared_ptr<Function>& fun,
                                     sptr_sobject_v bound_args)
    : Function(make_detached_signature(*fun))
    , ctx(ctx)
    , fun(fun)
    , bound_args(bound_args)
  {
  }

  std::shared_ptr<DetachedFunction> DetachedFunction::make_detached(Context& ctx,
                                                                    std::shared_ptr<Object> fun_obj,
                                                                    sptr_sobject_v bound_args)
  {
    std::shared_ptr<Function> fun = std::dynamic_pointer_cast<Function>(fun_obj);
    return make_detached(ctx, fun, bound_args);
  }

  std::shared_ptr<DetachedFunction> DetachedFunction::make_detached(Context& ctx,
                                                                    std::shared_ptr<Function> fun,
                                                                    sptr_sobject_v bound_args)
  {
    return fun
      ? std::make_shared<DetachedFunction>(ctx.detach(), fun, bound_args)
      : std::shared_ptr<DetachedFunction>();
  }

  std::vector<std::unique_ptr<Signature>> DetachedFunction::make_detached_signature(Function& target)
  {
    std::vector<std::unique_ptr<Signature>> sigs;
    for (auto& sig : target.get_signatures())
    {
      exec_fn disp_target = std::bind(&DetachedFunction::dispatch_detached,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2);

      sigs.push_back(std::make_unique<Signature>(sig->get_arguments(), disp_target));
    }
    return sigs;
  }

  sptr_sobject DetachedFunction::execute(Context& ctx, sptr_sobject_v& args)
  {
    return dispatch_detached(ctx, args);
  }

  sptr_sobject DetachedFunction::dispatch_detached(Context&, sptr_sobject_v& args)
  {
    if (bound_args.empty())
    {
      return execute_bound(args);
    }
    if (args.empty())
    {
      return execute_bound(bound_args);
    }
    sptr_sobject_v merged_args;
    for (auto& arg : args)
    {
      merged_args.push_back(arg);
    }
    for (auto& arg : bound_args)
    {
      merged_args.push_back(arg);
    }
    return execute_bound(merged_args);
  }

  sptr_sobject DetachedFunction::execute_bound(sptr_sobject_v& args)
  {
    return fun->execute(*ctx, args);
  }

  UserFunction::UserFunction(const Namespace* home_ns,
                             Lisple::arg_v args,
                             std::vector<std::unique_ptr<ArgumentBinding>>& arg_bindings,
                             Lisple::sptr_sobject_v& body)
    : Function(std::make_unique<Lisple::sig>(args, EXEC_DISPATCH(&UserFunction::exec_body)))
    , home_ns(home_ns)
    , arg_bindings(std::move(arg_bindings))
    , body(body)
  {

  }

  Lisple::sptr_sobject UserFunction::exec_body(Lisple::Context& ctx, Lisple::sptr_sobject_v& args)
  {
    Namespace* current_namespace = ctx.get_current_namespace();
    ctx.switch_namespace(home_ns->get_name());
    Lisple::Scope fn_scope;
    for (size_t i=0; i<args.size(); i++)
    {
      arg_bindings.at(i)->apply(fn_scope, args.at(i));
    }
    ctx.push_context(true, fn_scope);
    sptr_sobject retval;
    for (auto& form : body)
    {
      retval = ctx.eval(form);
    }
    ctx.pop_context();
    ctx.switch_namespace(current_namespace->get_name());
    return retval;
  }

  const sptr_sobject_v& UserFunction::get_body() const
  {
    return body;
  }

  const std::vector<std::unique_ptr<ArgumentBinding>>& UserFunction::get_argument_bindings() const
  {
    return arg_bindings;
  }

  Macro::Macro(uptr_sig signature)
    : Executable(Form::MACRO, std::move(signature))
  {
  }

  Macro::Macro(uptr_sig_v signatures)
    : Executable(Form::MACRO, std::move(signatures))
  {

  }

  Signature& Macro::get_signature(Lisple::sptr_sobject_v& args)
  {
    for (auto& sig : signatures)
    {
      if (sig->matches(args))
      {
        return *sig;
      }
    }
    throw InvocationException("No matching form for arguments: " + Lisple::Array(args).to_string());
  }

  std::string Macro::to_string() const
  {
    return "<macro>";
  }
}
