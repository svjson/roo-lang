
#include "lisple/exec.h"

#include <memory>
#include <sstream>
#include <utility>
#include <variant>

#include <lisple/bind.h>
#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/namespace.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>
#include <lisple/runtime/node.h>
#include <lisple/runtime/value.h>
#include <lisple/scope.h>
#include <lisple/type.h>

namespace Lisple
{
  int user_functions_created = 0;
  int user_functions_ast_created = 0;
  int user_functions_rtval_created = 0;
  int user_function_ast_invocations = 0;
  int user_function_rtval_invocations = 0;
  int user_function_wrong_path_invocations = 0;

  const Key KEY_DESTR__KEYS("keys");
  const Key KEY_DESTR__AS("as");

  /** Legacy - ArgumentBinding */
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

  /* DestructuringArgumentBinding */
  DestructuringArgumentBinding::DestructuringArgumentBinding(const Map& binding_form)
    : binding_form(binding_form)
  {
    if (!binding_form.has_key(KEY_DESTR__KEYS) || binding_form.keys().size() > 2 ||
        (binding_form.keys().size() == 2 && !binding_form.has_key(KEY_DESTR__AS)) ||
        !Type::ARRAY.is_type_of(binding_form.get_property(KEY_DESTR__KEYS)))
    {
      throw TypeError("Invalid destructuring form: " + binding_form.to_string(2));
    }
  }

  void DestructuringArgumentBinding::apply(Scope& scope, sptr_sobject& arg_val)
  {
    for (auto& key_obj :
         binding_form.get_sptr_property(KEY_DESTR__KEYS)->as<Array>().get_children())
    {
      Word& key_name = key_obj->as<Word>();
      auto key = Key::make(key_name.to_string());
      if (arg_val->has_key(*key))
      {
        scope.store(key_name, arg_val->get_sptr_property(*key));
      }
      else
      {
        scope.store(key_name, NIL);
      }
    }

    if (binding_form.has_key(KEY_DESTR__AS))
    {
      Object& alias = binding_form.get_property(KEY_DESTR__AS);
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

  bool Argument::matches(Object& obj) const
  {
    return type->is_type_of(obj);
  }

  bool Argument::matches(RTValue& obj) const
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

  CoercionResult<Object> Argument::coerce(Context& ctx, sptr_sobject& obj) const
  {
    return type->coerce(ctx, obj);
  }

  CoercionResult<RTValue> Argument::coerce(Context& ctx, sptr_rtval& obj) const
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
  Signature::Signature(arg_v args, exec_fn target_func)
    : arguments(args)
    , target_func(target_func)
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

  Signature::Signature(arg_v args, exec_fn target_func, exec_node_fn exec_func)
    : Signature(args, target_func)
  {
    this->exec_func = exec_func;
  }

  Signature::Signature(arg_v args, exec_rtval_fn exec_func)
    : arguments(args)
    , target_func(nullptr)
    , exec_func(nullptr)
    , exec_rtval(exec_func)
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

  const std::vector<Argument>& Signature::get_arguments() const
  {
    return arguments;
  }

  sptr_rtval_v Signature::coerce_args(Context& ctx, sptr_rtval_v& args)
  {
    sptr_rtval_v coerced;
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
          CoercionResult<RTValue> coercion = arg.coerce(ctx, args[i]);
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
          CoercionResult<Object> coercion = arg.coerce(ctx, args[i]);
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

  bool Signature::supports_rt_value() const
  {
    return this->exec_rtval != nullptr;
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
      if (arguments_size != args_size)
      {
        return false;
      }

      for (size_t i = 0; i < args_size; i++)
      {
        if (!arguments[i].matches(args[i]))
        {
          return false;
        }
      }
    }

    return true;
  }

  bool Signature::matches(const sptr_rtval_v& args) const
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

  sptr_sobject Signature::invoke(Context& ctx, sptr_sobject_v& args)
  {
    if (target_func)
    {
      return target_func(ctx, args);
    }
    else if (exec_rtval)
    {
      sptr_rtval_v rt_args;
      for (auto& arg : args)
      {
        rt_args.push_back(to_rt_value(arg));
      }
      return RuntimeValueWrapper::make(exec_rtval(ctx, rt_args));
    }
    else
    {
      throw LispleException("Bad execution path. Args: " + Array::make(args)->to_string());
    }
  }

  sptr_rtval Signature::invoke(Context& ctx, ptr_exec_node_v& args)
  {
    if (exec_func)
    {
      return exec_func(ctx, args);
    }
    throw LispleException("Bad execution path - Exec node execution not supported");
  }

  sptr_rtval Signature::invoke(Context& ctx, sptr_rtval_v& args)
  {
    if (this->supports_rt_value())
    {
      if (!this->matches(args))
      {
        sptr_rtval_v coerced_args = coerce_args(ctx, args);
        if (coerced_args.size())
        {
          return exec_rtval(ctx, coerced_args);
        }
        else
        {
          throw InvocationException(
            "Could not apply args: " + RTValue::vector(args)->to_string() +
            " to signature expecting: " + this->to_string() + ".");
        }
      }
      else
      {
        return exec_rtval(ctx, args);
      }
    }

    if (exec_func != nullptr)
    {
      ptr_exec_node_v node_args;
      uptr_exec_node_v uptr_node_args;
      node_args.reserve(args.size());
      uptr_node_args.reserve(args.size());

      for (auto& val : args)
      {
        uptr_node_args.push_back(std::make_unique<ExecNode>(LiteralNode(val)));
        node_args.push_back(uptr_node_args.back().get());
      }
      return exec_func(ctx, node_args);
    }

    if (target_func != nullptr)
    {
      sptr_sobject_v obj_args;
      for (auto& val : args)
      {
        obj_args.push_back(RuntimeValueWrapper::make(val));
      }

      sptr_sobject result = target_func(ctx, obj_args);
      return Lisple::to_rt_value(result);
    }

    throw LispleException("Bad execution path");
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

  bool Executable::requires_late_binding(CallNode& call_node)
  {
    for (auto& sig : signatures)
    {
      if (sig->supports_exec_tree() || sig->is_lazy_eval()) return false;
    }

    return !call_node.is_literal_arg_list();
  }

  bool Executable::operator==(const Object& other) const
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

  Signature* Executable::get_signature(Context& ctx, sptr_sobject_v& args)
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

  const std::vector<std::unique_ptr<Signature>>& Executable::get_signatures() const
  {
    return signatures;
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

  sptr_rtval Executable::execute(Context& ctx, sptr_rtval_v& args)
  {
    for (auto& signature : signatures)
    {
      if (signature->matches(args))
      {
        if (signature->supports_rt_value() || signature->supports_exec_tree())
        {
          return signature->invoke(ctx, args);
        }
        else
        {
          sptr_sobject_v wrapped_args;
          for (auto& arg : args)
          {
            wrapped_args.push_back(RuntimeValueWrapper::make(arg));
          }

          sptr_sobject retval = signature->invoke(ctx, wrapped_args);
          return to_rt_value(retval);
        }
      }
    }

    sptr_sobject_v wrapped_args;
    for (auto& arg : args)
    {
      wrapped_args.push_back(RuntimeValueWrapper::make(arg));
    }

    sptr_sobject retval = this->execute(ctx, wrapped_args);
    return to_rt_value(retval);
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
      if (sig->supports_rt_value())
      {
        exec_rtval_fn disp_target = std::bind(&DetachedFunction::dispatch_detached,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2);
        sigs.push_back(std::make_unique<Signature>(sig->get_arguments(), disp_target));
      }
      else if (sig->supports_exec_tree())
      {
        throw LispleException("exec_node dispatch not supported!");
      }
      else
      {
        exec_fn disp_target = std::bind(&DetachedFunction::dispatch_detached_ast,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2);

        sigs.push_back(std::make_unique<Signature>(sig->get_arguments(), disp_target));
      }
    }
    return sigs;
  }

  sptr_sobject DetachedFunction::execute(Context& ctx, sptr_sobject_v& args)
  {
    return dispatch_detached_ast(ctx, args);
  }

  sptr_rtval DetachedFunction::execute(Context& ctx, sptr_rtval_v& args)
  {
    return dispatch_detached(ctx, args);
  }

  sptr_sobject DetachedFunction::dispatch_detached_ast(Context&, sptr_sobject_v& args)
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

  sptr_rtval DetachedFunction::dispatch_detached(Context&, sptr_rtval_v& args)
  {
    if (bound_args.empty())
    {
      return fun->execute(*this->ctx, args);
    }
    if (args.empty())
    {
      throw LispleException(
        "DetachedFunction::dispatch_detached with bounds args not supported for rtval");
      // return fun->execute(*this->ctx, bound_args);
    }

    throw LispleException("DetachedFunction::dispatch_detached with merged bound+literal "
                          "args not supported for rtval");

    // sptr_sobject_v merged_args;
    // for (auto& arg : args)
    // {
    //   merged_args.push_back(arg);
    // }
    // for (auto& arg : bound_args)
    // {
    //   merged_args.push_back(arg);
    // }
    // return execute_bound(merged_args);
  }

  sptr_sobject DetachedFunction::execute_bound(sptr_sobject_v& args)
  {
    return fun->execute(*ctx, args);
  }

  UserFunction::UserFunction(const std::string& home_ns,
                             arg_v args,
                             std::vector<std::unique_ptr<LexicalBinding>>& arg_binding,
                             uptr_exec_node_v&& body)
    : Function(std::make_unique<sig>(args, LEGACY_DISPATCH(&UserFunction::exec_body)))
    , home_ns(home_ns)
    , arg_binding(std::move(arg_binding))
    , uptr_body(std::move(body))
  {
    user_functions_created++;
    user_functions_rtval_created++;
    this->body.reserve(body.size());
    for (auto& node : uptr_body)
    {
      this->body.push_back(node.get());
    }
  }

  UserFunction::UserFunction(const std::string& home_ns,
                             arg_v args,
                             std::vector<std::unique_ptr<ArgumentBinding>>& arg_bindings,
                             sptr_sobject_v& body)
    : Function(std::make_unique<sig>(args, LEGACY_DISPATCH(&UserFunction::exec_ast_body)))
    , home_ns(home_ns)
    , arg_bindings(std::move(arg_bindings))
  {
    user_functions_created++;
    user_functions_ast_created++;
    this->uptr_body.reserve(body.size());
    this->body.reserve(body.size());
    for (auto& node : body)
    {
      uptr_exec_node unode = lower_expr(node);
      this->uptr_body.push_back(std::move(unode));
      this->body.push_back(uptr_body.back().get());
    }
  }

  sptr_sobject UserFunction::exec_ast_body(Context& ctx, sptr_sobject_v& args)
  {
    user_function_ast_invocations++;
    const std::string current_namespace = ctx.get_current_namespace()->get_name();
    ctx.switch_namespace(home_ns);
    Scope fn_scope;
    if (arg_bindings.size() > 0)
    {
      for (size_t i = 0; i < args.size(); i++)
      {
        arg_bindings[i]->apply(fn_scope, args[i]);
      }
    }
    else if (arg_binding.size() > 0)
    {
      user_function_wrong_path_invocations++;
      for (size_t i = 0; i < args.size(); i++)
      {
        arg_binding[i]->apply(fn_scope, to_rt_value(args[i]));
      }
    }
    ctx.push_context(true, fn_scope);
    sptr_sobject retval = body.empty() ? NIL : nullptr;

    for (auto& node : body)
    {
      retval = eval(ctx, *node);
    }
    ctx.pop_context();
    ctx.switch_namespace(current_namespace);

    return retval;
  }

  sptr_rtval UserFunction::exec_body(Context& ctx, sptr_rtval_v& args)
  {
    user_function_rtval_invocations++;
    const std::string current_namespace = ctx.get_current_namespace()->get_name();
    ctx.switch_namespace(home_ns);
    Scope fn_scope;
    if (arg_binding.size() > 0)
    {
      for (size_t i = 0; i < args.size(); i++)
      {
        arg_binding[i]->apply(fn_scope, args[i]);
      }
    }
    else if (arg_bindings.size() > 0)
    {
      user_function_wrong_path_invocations++;
      for (size_t i = 0; i < args.size(); i++)
      {
        sptr_sobject wrapped = RuntimeValueWrapper::make(args[i]);
        arg_bindings[i]->apply(fn_scope, wrapped);
      }
    }
    ctx.push_context(true, fn_scope);
    sptr_rtval retval = body.empty() ? Constant::NIL : nullptr;

    for (auto& node : body)
    {
      retval = exec(ctx, *node);
    }
    ctx.pop_context();
    ctx.switch_namespace(current_namespace);

    return retval;
  }

  const uptr_exec_node_v& UserFunction::get_body() const
  {
    return uptr_body;
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

  std::string Macro::to_string(int) const
  {
    return "<macro>";
  }

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                Object& arg_array,
                                                sptr_sobject_v& body)
  {
    std::vector<Argument> arg_types;
    std::vector<std::unique_ptr<ArgumentBinding>> arg_bindings;
    arg_types.reserve(arg_array.size());
    arg_bindings.reserve(arg_array.size());
    for (auto& arg : arg_array.get_children())
    {
      if (arg->get_type() != Form::WORD && arg->get_type() != Form::MAP)
      {
        throw LispleException("Illegal fn argument declaration: " + arg_array.to_string());
      }
      arg_types.push_back(Lisple::arg(&Type::ANY));
      arg_bindings.push_back(ArgumentBinding::create(*arg));
    }
    return std::make_shared<UserFunction>(home_ns->get_name(),
                                          std::move(arg_types),
                                          arg_bindings,
                                          body);
  }

  std::shared_ptr<UserFunction> create_function(const Namespace* home_ns,
                                                sptr_rtval_v& arg_array,
                                                ptr_exec_node_v& body)
  {
    std::vector<Argument> arg_types;
    std::vector<std::unique_ptr<LexicalBinding>> arg_bindings;
    arg_types.reserve(arg_array.size());
    arg_bindings.reserve(arg_array.size());
    for (auto& arg : arg_array)
    {
      if (arg->type != RTValue::Type::SYMBOL && arg->type != RTValue::Type::MAP)
      {
        throw LispleException("Illegal fn argument declaration: " +
                              RTValue::vector(arg_array)->to_string());
      }
      arg_types.push_back(Lisple::arg(&Type::ANY));
      arg_bindings.push_back(LexicalBinding::create(arg));
    }

    uptr_exec_node_v new_body;
    for (auto& node : body)
    {
      new_body.push_back(node->clone());
    }

    return std::make_shared<UserFunction>(home_ns->get_name(),
                                          std::move(arg_types),
                                          arg_bindings,
                                          std::move(new_body));
  }

  std::shared_ptr<DetachedFunction> create_detached_function(Context& ctx,
                                                             Object& arg_array,
                                                             sptr_sobject_v& body)
  {
    std::shared_ptr<Function> fn =
      create_function(ctx.get_current_namespace(), arg_array, body);
    return std::make_shared<Lisple::DetachedFunction>(ctx.detach(), fn);
  }

  std::shared_ptr<DetachedFunction> create_detached_function(Context& ctx,
                                                             sptr_rtval_v& arg_array,
                                                             ptr_exec_node_v& body)
  {
    std::shared_ptr<Function> fn =
      create_function(ctx.get_current_namespace(), arg_array, body);
    return std::make_shared<Lisple::DetachedFunction>(ctx.detach(), fn);
  }

} // namespace Lisple
