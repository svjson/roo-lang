
#include "roo/exec.h"

#include <memory>
#include <sstream>
#include <utility>
#include <variant>

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/exception.h>
#include <roo/form.h>
#include <roo/namespace.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>
#include <roo/runtime/node.h>
#include <roo/runtime/value.h>
#include <roo/scope.h>
#include <roo/type.h>

namespace Roo
{
  int user_functions_created = 0;
  int user_functions_ast_created = 0;
  int user_functions_rtval_created = 0;
  int user_function_ast_invocations = 0;
  int user_function_rtval_invocations = 0;
  int user_function_wrong_path_invocations = 0;

  Argument::Argument(const TypeRef* type)
    : Argument(false, type, &Eval::DEFAULT)
  {
  }

  Argument::Argument(const TypeRef* type, const EvalMode* eval)
    : Argument(false, type, eval)
  {
  }

  Argument::Argument(vararg_mode var, const TypeRef* type)
    : Argument(var, type, &Eval::DEFAULT)
  {
  }

  Argument::Argument(vararg_mode var, const TypeRef* type, const EvalMode* eval)
    : type(type)
    , eval(eval)
    , varargs(var)
  {
  }

  bool Argument::matches(Value& obj) const
  {
    return type->is_type_of(obj);
  }

  bool Argument::matches(const uptr_exec_node& node) const
  {
    if (auto* lit_node = std::get_if<LiteralNode>(&node->data))
    {
      return matches(*lit_node->value);
    }
    return true;
  }

  CoercionResult Argument::coerce(Context& ctx, sptr_val& obj) const
  {
    return type->coerce(ctx, obj);
  }

  bool Argument::is_literal() const
  {
    return eval == &Eval::LITERAL;
  }

  bool Argument::evalp() const
  {
    return eval == &Eval::DEFAULT;
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
  Signature::Signature(arg_v args, exec_node_fn exec_func)
    : arguments(args)
    , exec_func(exec_func)
  {
    std::vector<const EvalMode*> arg_modes;
    for (auto& arg : arguments)
    {
      arg_modes.push_back(arg.eval);
      if (arg.is_vararg())
      {
        this->vararg = true;
        break;
      }
    }

    this->eval_pattern = std::make_unique<EvalPattern>(arg_modes);
  }

  Signature::Signature(arg_v args, exec_val_fn exec_func)
    : arguments(args)
    , exec_val(exec_func)
  {
    std::vector<const EvalMode*> arg_modes;
    for (auto& arg : arguments)
    {
      arg_modes.push_back(arg.eval);
      if (arg.is_vararg())
      {
        this->vararg = true;
        break;
      }
    }

    this->eval_pattern = std::make_unique<EvalPattern>(arg_modes);
  }

  Signature::Signature(arg_v args,
                       exec_val_fn exec_func,
                       size_t optional_count,
                       bool has_rest)
    : Signature(args, exec_func)
  {
    this->optional_count = optional_count;
    this->has_rest = has_rest;
  }

  const std::vector<Argument>& Signature::get_arguments() const
  {
    return arguments;
  }

  size_t Signature::get_optional_count() const
  {
    return optional_count;
  }

  bool Signature::has_rest_parameter() const
  {
    return has_rest;
  }

  sptr_val_v Signature::coerce_args(Context& ctx, sptr_val_v& args)
  {
    sptr_val_v coerced;
    coerced.reserve(args.size());

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
            break;
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

  bool Signature::supports_rt_value() const
  {
    return this->exec_val != nullptr;
  }

  bool Signature::is_lazy_eval() const
  {
    for (auto& arg : arguments)
    {
      if (!arg.evalp()) return true;
    }
    return false;
  }

  bool Signature::matches(const uptr_exec_node_v& args) const
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
          if (arguments[a].matches(args[i]))
            i++;
          else
            a++;
        }
        else
        {
          if (!arguments[a].matches(args[i])) return false;
          i++;
          a++;
        }
      }

      if (a < arguments_size - 1 || i < args_size) return false;
    }
    else
    {
      if (args_size < arguments_size - optional_count) return false;
      if (!has_rest && args_size > arguments_size) return false;

      const size_t check_count = std::min(args_size, arguments_size);
      for (size_t i = 0; i < check_count; i++)
      {
        if (!arguments[i].matches(args[i]))
        {
          return false;
        }
      }
    }

    return true;
  }

  bool Signature::matches(const sptr_val_v& args) const
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
      if (args_size < arguments_size - optional_count) return false;
      if (!has_rest && args_size > arguments_size) return false;

      const size_t check_count = std::min(args_size, arguments_size);
      for (size_t i = 0; i < check_count; i++)
      {
        if (!arguments[i].matches(*args[i]))
        {
          return false;
        }
      }
    }

    return true;
  }

  bool Signature::is_literal_arg(std::size_t index) const
  {
    if (index < arguments.size())
    {
      return arguments[index].is_literal();
    }
    else if (arguments.back().is_vararg())
    {
      return arguments.back().is_literal();
    }
    throw InvocationException("Invalid argument index: " + std::to_string(index) +
                              ". Are varargs correcly applied?");
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

  sptr_val Signature::invoke(Context& ctx, SpecialFormNode& snode)
  {
    if (exec_func)
    {
      return exec_func(ctx, snode);
    }
    throw RooException("Bad execution path - Exec node execution not supported");
  }

  sptr_val Signature::invoke(Context& ctx, sptr_val_v& args)
  {
    if (!this->matches(args))
    {
      sptr_val_v coerced_args = coerce_args(ctx, args);
      if (coerced_args.size())
      {
        return exec_val(ctx, coerced_args);
      }
      else
      {
        throw InvocationException(
          "Could not apply args: " + Value::vector(args)->to_string() +
          " to signature expecting: " + this->to_string() + ".");
      }
    }
    else

      return exec_val(ctx, args);
  }

  sptr_val Signature::invoke_matched(Context& ctx, sptr_val_v& args)
  {
    return exec_val(ctx, args);
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
    : type(type)
    , signatures(std::move(signatures))
  {
  }

  Executable::Executable(Form type, uptr_sig signature)
    : type(type)
  {
    signatures.push_back(std::move(signature));
  }

  Form Executable::get_type() const
  {
    return type;
  }

  bool Executable::supports_exec_tree() const
  {
    if (signatures.size() == 1)
    {
      return signatures[0]->supports_exec_tree();
    }

    return false;
  }

  bool Executable::requires_late_binding(CallNode& call_node)
  {
    for (auto& sig : signatures)
    {
      if (sig->supports_exec_tree() || sig->is_lazy_eval()) return false;
    }

    return !call_node.is_literal_arg_list();
  }

  bool Executable::operator==(const Executable& other) const
  {
    return this == &other;
  }

  Signature* Executable::get_signature([[maybe_unused]] Context& ctx, uptr_exec_node_v& args)
  {
    if (signatures.size() == 1) return signatures[0].get();
    for (auto& sig : signatures)
    {
      if (sig->matches(args))
      {
        return sig.get();
      }
    }

    return nullptr;
  }

  const std::vector<std::unique_ptr<Signature>>& Executable::get_signatures() const
  {
    return signatures;
  }

  sptr_val Executable::execute(Context& ctx, sptr_val_v& args)
  {
    for (auto& signature : signatures)
    {
      if (signature->matches(args))
      {
        return signature->invoke_matched(ctx, args);
      }
    }

    for (auto& signature : signatures)
    {
      sptr_val_v coerced_args = signature->coerce_args(ctx, args);
      if (coerced_args.size())
      {
        return signature->invoke_matched(ctx, coerced_args);
      }
    }

    throw InvocationException("No matching signature: " +
                              Roo::Value::vector(args)->to_string());
  }

  Function::Function(uptr_sig signature)
    : Executable(Form::FUNCTION, std::move(signature))
  {
  }

  Function::Function(uptr_sig_v signatures)
    : Executable(Form::FUNCTION, std::move(signatures))
  {
  }

  std::string Function::to_string(int) const
  {
    return "<fn>";
  }

  DetachedFunction::DetachedFunction(std::shared_ptr<Context> ctx,
                                     std::shared_ptr<Function>& fun,
                                     sptr_val_v bound_args)
    : Function(make_detached_signature(*fun))
    , ctx(ctx)
    , fun(fun)
    , bound_args(bound_args)
  {
  }

  std::shared_ptr<DetachedFunction> DetachedFunction::make_detached(Context& ctx,
                                                                    sptr_executable fun_obj,
                                                                    sptr_val_v bound_args)
  {
    std::shared_ptr<Function> fun = std::dynamic_pointer_cast<Function>(fun_obj);

    return fun ? std::make_shared<DetachedFunction>(ctx.detach(), fun, bound_args) : nullptr;
  }

  std::vector<std::unique_ptr<Signature>> DetachedFunction::make_detached_signature(
    Function& target)
  {
    std::vector<std::unique_ptr<Signature>> sigs;
    for (auto& sig : target.get_signatures())
    {
      exec_val_fn disp_target = std::bind(&DetachedFunction::dispatch_detached,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2);
      sigs.push_back(std::make_unique<Signature>(sig->get_arguments(),
                                                 disp_target,
                                                 sig->get_optional_count(),
                                                 sig->has_rest_parameter()));
    }
    return sigs;
  }

  sptr_val DetachedFunction::execute(Context& ctx, sptr_val_v& args)
  {
    return dispatch_detached(ctx, args);
  }

  sptr_val DetachedFunction::dispatch_detached(Context&, sptr_val_v& args)
  {
    if (bound_args.empty())
    {
      return fun->execute(*this->ctx, args);
    }
    if (args.empty())
    {
      return fun->execute(*this->ctx, bound_args);
    }

    sptr_val_v merged_args;
    for (auto& arg : args)
    {
      merged_args.push_back(arg);
    }
    for (auto& arg : bound_args)
    {
      merged_args.push_back(arg);
    }
    return fun->execute(*this->ctx, merged_args);
  }

  sptr_val DetachedFunction::execute_bound(sptr_val_v& args)
  {
    return fun->execute(*ctx, args);
  }

  UserFunction::UserFunction(const std::string& name,
                             const std::string& home_ns,
                             arg_v args,
                             std::vector<std::unique_ptr<LexicalBinding>>& arg_binding,
                             uptr_exec_node_v&& body,
                             size_t optional_count,
                             std::unique_ptr<RestBinding> rest_binding)
    : Function(std::make_unique<sig>(args,
                                     EXEC_DISPATCH(&UserFunction::exec_body),
                                     optional_count,
                                     rest_binding != nullptr))
    , name(name)
    , home_ns(home_ns)
    , arg_binding(std::move(arg_binding))
    , uptr_body(std::move(body))
    , required_count(this->arg_binding.size() - optional_count)
    , optional_count(optional_count)
    , rest_binding(std::move(rest_binding))
  {
    user_functions_created++;
    user_functions_rtval_created++;
    this->body.reserve(body.size());
    for (auto& node : uptr_body)
    {
      this->body.push_back(node.get());
    }
  }

  std::string UserFunction::to_string([[maybe_unused]] int depth) const
  {
    return "#'" + home_ns + "/" + name;
  }

  sptr_val UserFunction::execute(Context& ctx, sptr_val_v& args)
  {
    const size_t min_count = required_count;
    const size_t max_count = arg_binding.size();
    if (args.size() < min_count || (!rest_binding && args.size() > max_count))
    {
      throw InvocationException("No matching signature: " +
                                Roo::Value::vector(args)->to_string());
    }

    return exec_body(ctx, args);
  }

  sptr_val UserFunction::exec_body(Context& ctx, sptr_val_v& args)
  {
    user_function_rtval_invocations++;
    const std::string current_namespace = ctx.get_current_namespace()->get_name();
    ctx.switch_namespace(home_ns);
    bool pushed_context = false;

    try
    {
      Scope fn_scope;
      for (size_t i = 0; i < this->required_count; i++)
      {
        arg_binding[i]->apply(fn_scope, args[i]);
      }
      for (size_t i = 0; i < this->optional_count; i++)
      {
        const size_t arg_idx = this->required_count + i;
        const sptr_val& val = arg_idx < args.size() ? args[arg_idx] : Constant::NIL;
        arg_binding[arg_idx]->apply(fn_scope, val);
      }
      if (this->rest_binding)
      {
        const size_t rest_start = this->required_count + this->optional_count;
        sptr_val_v rest_args;
        if (rest_start < args.size())
        {
          rest_args.assign(args.begin() + rest_start, args.end());
        }
        this->rest_binding->apply(fn_scope, rest_args);
      }
      ctx.push_context(true, fn_scope);
      pushed_context = true;
      sptr_val retval = body.empty() ? Constant::NIL : nullptr;

      for (auto& node : body)
      {
        retval = exec(ctx, *node);
      }
      ctx.pop_context();
      ctx.switch_namespace(current_namespace);

      return retval;
    }
    catch (...)
    {
      if (pushed_context)
      {
        ctx.pop_context();
      }
      ctx.switch_namespace(current_namespace);
      throw;
    }
  }

  const uptr_exec_node_v& UserFunction::get_body() const
  {
    return uptr_body;
  }

  const std::vector<std::unique_ptr<LexicalBinding>>& UserFunction::get_argument_bindings()
    const
  {
    return arg_binding;
  }

  /** Macro */
  Macro::Macro(uptr_sig signature)
    : Executable(Form::MACRO, std::move(signature))
  {
  }

  Macro::Macro(uptr_sig_v signatures)
    : Executable(Form::MACRO, std::move(signatures))
  {
  }

  std::string Macro::to_string(int) const
  {
    return "<macro>";
  }

  /** SpecialForm */
  SpecialForm::SpecialForm(uptr_sig signature)
    : Executable(Form::MACRO, std::move(signature))
  {
  }

  SpecialForm::SpecialForm(uptr_sig_v signatures)
    : Executable(Form::MACRO, std::move(signatures))
  {
  }

  sptr_val SpecialForm::exec_node(Context& ctx, SpecialFormNode& node) const
  {
    for (auto& sig : signatures)
    {
      if (sig->supports_exec_tree())
      {
        return sig->invoke(ctx, node);
      }
    }

    throw InvocationException("Invalid SpecialFormNode");
  }

  std::string SpecialForm::to_string(int) const
  {
    return "<special-form>";
  }

  /**
   * Function creation ergonomics
   */
  std::shared_ptr<UserFunction> create_function(const std::string& name,
                                                Context& ctx,
                                                const Namespace* home_ns,
                                                AST::ASTNode& arg_vector,
                                                sptr_ast_node_v& body)
  {
    std::vector<Argument> arg_types;
    std::vector<std::unique_ptr<LexicalBinding>> arg_bindings;
    size_t optional_count = 0;
    std::unique_ptr<RestBinding> rest_binding;
    bool in_optional = false;

    auto& children = arg_vector.get_children();
    for (size_t idx = 0; idx < children.size(); idx++)
    {
      auto& arg = children[idx];
      if (arg->get_type() != Form::SYMBOL && arg->get_type() != Form::MAP &&
          arg->get_type() != Form::VECTOR)
      {
        throw RooException("Illegal fn argument declaration: " + arg_vector.to_string());
      }

      auto rt_arg = to_rt_value(*arg);
      if (rt_arg->type == Value::Type::SYMBOL)
      {
        const std::string& sym = std::get<std::string>(rt_arg->value);
        if (sym == "&")
        {
          if (in_optional)
          {
            throw RooException("Duplicate & in argument list: " + arg_vector.to_string());
          }
          in_optional = true;
          continue;
        }
        if (sym.size() > 1 && sym[0] == '&')
        {
          if (idx != children.size() - 1)
          {
            throw RooException("Rest parameter must be last in argument list: " +
                               arg_vector.to_string());
          }
          rest_binding = std::make_unique<RestBinding>(sym.substr(1));
          break;
        }
      }

      arg_types.push_back(Roo::arg(&Type::ANY));
      arg_bindings.push_back(LexicalBinding::create(rt_arg));
      if (in_optional)
      {
        optional_count++;
      }
    }

    uptr_exec_node_v node_body;
    node_body.reserve(body.size());
    LowerContext lctx{&ctx};
    for (auto& node : body)
    {
      uptr_exec_node unode = lower_expr(lctx, node);
      node_body.push_back(std::move(unode));
    }

    return std::make_shared<UserFunction>(name,
                                          home_ns->get_name(),
                                          std::move(arg_types),
                                          arg_bindings,
                                          std::move(node_body),
                                          optional_count,
                                          std::move(rest_binding));
  }

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                sptr_val_v& arg_vector,
                                                ptr_exec_node_v& body)
  {
    std::vector<Argument> arg_types;
    std::vector<std::unique_ptr<LexicalBinding>> arg_bindings;
    size_t optional_count = 0;
    std::unique_ptr<RestBinding> rest_binding;
    bool in_optional = false;

    for (size_t idx = 0; idx < arg_vector.size(); idx++)
    {
      auto& arg = arg_vector[idx];
      if (arg->type != Value::Type::SYMBOL && arg->type != Value::Type::MAP)
      {
        throw RooException("Illegal fn argument declaration: " +
                           Value::vector(arg_vector)->to_string());
      }

      if (arg->type == Value::Type::SYMBOL)
      {
        const std::string& sym = std::get<std::string>(arg->value);
        if (sym == "&")
        {
          if (in_optional)
          {
            throw RooException("Duplicate & in argument list: " +
                               Value::vector(arg_vector)->to_string());
          }
          in_optional = true;
          continue;
        }
        if (sym.size() > 1 && sym[0] == '&')
        {
          if (idx != arg_vector.size() - 1)
          {
            throw RooException("Rest parameter must be last in argument list: " +
                               Value::vector(arg_vector)->to_string());
          }
          rest_binding = std::make_unique<RestBinding>(sym.substr(1));
          break;
        }
      }

      arg_types.push_back(Roo::arg(&Type::ANY));
      arg_bindings.push_back(LexicalBinding::create(arg));
      if (in_optional)
      {
        optional_count++;
      }
    }

    uptr_exec_node_v new_body;
    for (auto& node : body)
    {
      new_body.push_back(node->clone());
    }

    return std::make_shared<UserFunction>("<user-fn>",
                                          home_ns->get_name(),
                                          std::move(arg_types),
                                          arg_bindings,
                                          std::move(new_body),
                                          optional_count,
                                          std::move(rest_binding));
  }

} // namespace Roo
