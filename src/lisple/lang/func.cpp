
#include "func.h"

#include "../exec.h"

namespace Lisple
{
  /* DefunForm - defun */

  SPECIAL_FORM_IMPL(
    DefunForm,
    MULTI_SIG(
      (FN_ARGS((&Type::WORD, DATA), (&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
       EXEC_DISPATCH(&DefunForm::inv_decl, &DefunForm::execnode_decl)),
      (FN_ARGS((&Type::WORD, DATA),
               (&Type::STRING, DATA),
               (&Type::ARRAY, DATA),
               (VARARG, &Type::ANY, NO_EVAL)),
       EXEC_DISPATCH(&DefunForm::inv_decl, &DefunForm::execnode_decl))))

  /** Legacy AST-based implementation */
  MACRO_BODY(DefunForm, inv_decl)
  {
    std::string fun_name = Lisple::Value<std::string>::value_of(*args[0]);
    sptr_sobject_v body;
    body.reserve(args.size() - 2);
    for (size_t i = 2; i < args.size(); i++)
    {
      body.push_back(args[i]);
    }
    auto fn = create_function(ctx.get_current_namespace(), *args[1], body);
    ctx.store_namespace(fun_name, fn);

    return fn;
  }

  EXECNODE_BODY(DefunForm, execnode_decl)
  {
    std::string& fun_name =
      std::get<std::string>(std::get<LiteralNode>(args[0]->data).value->value);

    ptr_exec_node_v body;

    body.reserve(args.size() - 2);
    for (size_t i = 2; i < args.size(); i++)
    {
      body.push_back(args[i]);
    }

    sptr_rtval_v& arg_form =
      std::get<sptr_rtval_v>(std::get<LiteralNode>(args[1]->data).value->value);

    sptr_sobject func = create_function(ctx.get_current_namespace(), arg_form, body);
    sptr_rtval fn = RTValue::executable(func);
    ctx.store_namespace(fun_name, func);

    return fn;
  }

  /** Legacy AST-based implementation */
  MACRO_BODY(DefunForm, inv_decl_docstring)
  {
    args.erase(args.begin() + 1);
    return this->inv_decl(ctx, args);
  }

  EXECNODE_BODY(DefunForm, execnode_decl_docstring)
  {
    args.erase(args.begin() + 1);
    return this->execnode_decl(ctx, args);
  }

  /** FnForm - fn */
  SPECIAL_FORM_IMPL(FnForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&FnForm::inv_decl, &FnForm::execnode_decl))))

  /**
   * Legacy AST-based implementation
   */
  MACRO_BODY(FnForm, inv_decl)
  {
    sptr_sobject_v body;
    body.reserve(args.size() - 1);
    for (size_t i = 1; i < args.size(); i++)
    {
      body.push_back(args[i]);
    }
    return create_detached_function(ctx, *args[0], body);
  }

  EXECNODE_BODY(FnForm, execnode_decl)
  {
    ptr_exec_node_v body;

    body.reserve(args.size() - 1);
    for (size_t i = 1; i < args.size(); i++)
    {
      body.push_back(args[i]);
    }

    sptr_rtval_v& arg_form =
      std::get<sptr_rtval_v>(std::get<LiteralNode>(args[0]->data).value->value);

    sptr_sobject fn = create_detached_function(ctx, arg_form, body);
    return RTValue::executable(fn);
  }

} // namespace Lisple
