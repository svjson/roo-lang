
#include "exec.h"

#include "context.h"
#include "exception.h"
#include "form.h"
#include "namespace.h"
#include "scope.h"
#include "type.h"
#include <memory>
#include <sstream>
#include <utility>

#include "lisple/runtime/exec_tree.h"

namespace Lisple
{
  std::unique_ptr<ArgumentBinding> ArgumentBinding::create(Object& arg_declaration)
  {
    if (arg_declaration.get_type() == Form::WORD)
    {
      return std::make_unique<NamedArgumentBinding>(
        Value<std::string>::value_of(arg_declaration));
    }

    if (arg_declaration.get_type() == Form::MAP)
    {
      return std::make_unique<DestructuringArgumentBinding>(arg_declaration.as<Map>());
    }

    if (arg_declaration.get_type() == Form::ARRAY)
    {
      return std::make_unique<ArrayArgumentBinding>(arg_declaration.as<Array>());
    }

    throw LispleException("Invalid argument declaration: " + arg_declaration.to_string(2));
  }

  /* NamedArgumentBinding */
  NamedArgumentBinding::NamedArgumentBinding(const std::string& name)
    : arg_name(name)
  {
  }

  void NamedArgumentBinding::apply(Scope& scope, sptr_sobject& arg_val)
  {
    scope.store(Word(arg_name), arg_val);
  }

  const Key keys("keys");
  const Key as("as");

  /* DestructuringArgumentBinding */
  DestructuringArgumentBinding::DestructuringArgumentBinding(const Map& binding_form)
    : binding_form(binding_form)
  {
    if (!binding_form.has_key(keys) || binding_form.keys().size() > 2 ||
        (binding_form.keys().size() == 2 && !binding_form.has_key(as)) ||
        !Type::ARRAY.is_type_of(binding_form.get_property(keys)))
    {
      throw TypeError("Invalid destructuring form: " + binding_form.to_string(2));
    }
  }

  void DestructuringArgumentBinding::apply(Scope& scope, sptr_sobject& arg_val)
  {
    for (auto& key_obj : binding_form.get_sptr_property(keys)->as<Array>().get_children())
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

    if (binding_form.has_key(as))
    {
      Object& alias = binding_form.get_property(as);
      if (alias.get_type() != Form::WORD)
      {
        throw LispleException("Invalid alias destructuring form: " +
                              binding_form.to_string(2));
      }
      Word& alias_word = alias.as<Word>();
      scope.store(alias_word, arg_val);
    }
  }

  /* ArrayArgumentBinding */
  ArrayArgumentBinding::ArrayArgumentBinding(const Array& binding_form)
    : binding_form(binding_form)
  {
  }

  void ArrayArgumentBinding::apply(Scope& scope, sptr_sobject& init_expr)
  {
    if (!Type::SEQ.is_type_of(*init_expr))
    {
      throw TypeError("Invalid init expression: " + init_expr->to_string(2) +
                      ". Must be Seq for binding form: " + binding_form.to_string(2));
    }

    for (size_t b = 0; b < binding_form.size(); b++)
    {
      sptr_sobject& b_form = binding_form.get_children()[b];
      if (!Type::WORD.is_type_of(*b_form))
      {
        throw TypeError("Invalid binding form: " + b_form->to_string() + " in " +
                        binding_form.to_string());
      }
      sptr_sobject value = b < init_expr->size() ? init_expr->get_children()[b] : NIL;
      scope.store(b_form->as<Word>(), value);
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
    return this->is_vararg() ? type->to_string() + "..." : type->to_string();
  }

  /* Signature */
  Signature::Signature(arg_v args, exec_fn target_func)
    : arguments(args)
    , target_func(target_func)
  {
    for (auto& arg : arguments)
    {
      if (arg.is_vararg())
      {
        this->vararg = true;
        break;
      }
    }
  }

  Signature::Signature(arg_v args, exec_fn target_func, exec_node_fn exec_func)
    : Signature(args, target_func)
  {
    this->exec_func = exec_func;
  }

  const std::vector<Argument>& Signature::get_arguments() const
  {
    return arguments;
  }

  sptr_sobject_v Signature::coerce_args(Context& ctx, sptr_sobject_v& args)
  {
    sptr_sobject_v coerced;
    coerced.reserve(args.size());

    // Non-vararg signatures
    if (args.size() == arguments.size())
    {
      for (size_t i = 0; i < arguments.size(); i++)
      {
        const Argument& arg = arguments[i];
        if (arg.matches(*args[i]))
        {
          coerced.push_back(args[i]);
          continue;
        }
        else
        {
          CoercionResult coercion = arg.coerce(ctx, args[i]);
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

  bool Signature::supports_exec_tree() const
  {
    return this->exec_func != nullptr;
  }

  bool Signature::matches(const sptr_sobject_v& args) const
  {
    size_t args_size = args.size();
    size_t arguments_size = arguments.size();
    if (this->vararg)
    {
      size_t i = 0;
      size_t a = 0;
      while (i < args_size && a < arguments_size)
      {
        if (arguments[a].is_vararg())
        {
          if (arguments[a].matches(*args[i]))
            i++;
          else
            a++;
        }
        else
        {
          if (!arguments[a].matches(*args[i])) return false;
          i++;
          a++;
        }
      }

      if (a < arguments_size - 1 || i < args_size) return false;
    }
    else
    {
      if (arguments_size != args_size)
      {
        return false;
      }

      for (size_t i = 0; i < args_size; i++)
      {
        if (!arguments[i].matches(*args[i]))
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
      return arguments[index].evalp();
    }
    else if (arguments.back().is_vararg())
    {
      return arguments.back().evalp();
    }
    throw InvocationException("Invalid argument index: " + std::to_string(index) +
                              ". Are varargs correcly applied?");
  }

  sptr_sobject Signature::invoke(Context& ctx, sptr_sobject_v& args)
  {
    return target_func(ctx, args);
  }

  sptr_sobject Signature::invoke(Context& ctx, ptr_exec_node_v& args)
  {
    return exec_func(ctx, args);
  }

  std::string Signature::to_string() const
  {
    std::stringstream ss;
    ss << "[";

    for (size_t i = 0; i < arguments.size(); i++)
    {
      if (i > 0)
      {
        ss << ", ";
      }
      ss << arguments[i].to_string();
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

  bool Executable::supports_exec_tree() const
  {
    if (signatures.size() == 1)
    {
      return signatures[0]->supports_exec_tree();
    }

    return false;
  }

  bool Executable::operator==(const Object& other) const
  {
    return this == &other;
  }

  sptr_sobject Executable::execute(Context& ctx, sptr_sobject_v& args)
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
      expected = "Expected: " + signatures[0]->to_string();
    }
    else
    {
      expected = "Expected one of:";
      for (auto& signature : signatures)
      {
        expected += " " + signature->to_string();
      }
    }

    throw InvocationException("No matching signature: " + Array(args).to_string(3) + ". " +
                              expected + ", but got: " + Array(args).to_string(2) + ".");
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

  std::string Function::to_string(int) const
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

  std::shared_ptr<DetachedFunction> DetachedFunction::make_detached(
    Context& ctx,
    std::shared_ptr<Object> fun_obj,
    sptr_sobject_v bound_args)
  {
    std::shared_ptr<Function> fun = std::dynamic_pointer_cast<Function>(fun_obj);
    return make_detached(ctx, fun, bound_args);
  }

  std::shared_ptr<DetachedFunction> DetachedFunction::make_detached(
    Context& ctx,
    std::shared_ptr<Function> fun,
    sptr_sobject_v bound_args)
  {
    return fun ? std::make_shared<DetachedFunction>(ctx.detach(), fun, bound_args)
               : std::shared_ptr<DetachedFunction>();
  }

  std::vector<std::unique_ptr<Signature>> DetachedFunction::make_detached_signature(
    Function& target)
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

  UserFunction::UserFunction(const std::string& home_ns,
                             arg_v args,
                             std::vector<std::unique_ptr<ArgumentBinding>>& arg_bindings,
                             sptr_sobject_v& body)
    : Function(std::make_unique<sig>(args, LEGACY_DISPATCH(&UserFunction::exec_body)))
    , home_ns(home_ns)
    , arg_bindings(std::move(arg_bindings))
    , body(body)
  {
  }

  sptr_sobject UserFunction::exec_body(Context& ctx, sptr_sobject_v& args)
  {
    const std::string current_namespace = ctx.get_current_namespace()->get_name();
    ctx.switch_namespace(home_ns);
    Scope fn_scope;
    for (size_t i = 0; i < args.size(); i++)
    {
      arg_bindings[i]->apply(fn_scope, args[i]);
    }
    ctx.push_context(true, fn_scope);
    sptr_sobject retval = body.empty() ? NIL : nullptr;

    for (auto& form : body)
    {
      retval = ctx.eval(form);
    }
    ctx.pop_context();
    ctx.switch_namespace(current_namespace);

    return retval;
  }

  const sptr_sobject_v& UserFunction::get_body() const
  {
    return body;
  }

  const std::vector<std::unique_ptr<ArgumentBinding>>& UserFunction::get_argument_bindings()
    const
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

  Signature* Macro::get_signature(Context& ctx, sptr_sobject_v& args)
  {
    for (auto& sig : signatures)
    {
      if (sig->matches(args))
      {
        return sig.get();
      }
    }

    if (args.empty())
    {
      return nullptr;
    }

    for (auto& sig : signatures)
    {
      sptr_sobject_v coerced = sig->coerce_args(ctx, args);
      if (!coerced.empty())
      {
        return sig.get();
      }
    }

    return nullptr;
  }

  [[deprecated("Cannot handle coercion")]]
  Signature& Macro::get_signature(sptr_sobject_v& args)
  {
    for (auto& sig : signatures)
    {
      if (sig->matches(args))
      {
        return *sig;
      }
    }

    throw InvocationException("No matching form for arguments: " + Array(args).to_string());
  }

  std::string Macro::to_string(int) const
  {
    return "<macro>";
  }
} // namespace Lisple
