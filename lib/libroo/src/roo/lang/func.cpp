
#include "roo/exception.h"
#include "roo/runtime/lower.h"

#include <roo/exec.h>
#include <roo/impl.h>
#include <roo/lang/func.h>
#include <roo/runtime/seq.h>

namespace Roo
{
  /** ApplyFunction - roo/apply */
  FUNC_IMPL(ApplyFunction,
            SIG((FN_ARGS((&Roo::Type::EXEC), (&Roo::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&ApplyFunction::exec_apply))))

  EXEC_BODY(ApplyFunction, exec_apply)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      throw InvocationException("Cannot apply nil.");
    }

    auto& fn = args[0]->exec();
    sptr_val_v fn_args = Roo::get_children(*args.back());
    return fn.execute(ctx, fn_args);
  }

  /** DefunForm - roo/defun */
  SPECIAL_FORM_IMPL(DefunForm,
                    MULTI_SIG((FN_ARGS((&Type::SYMBOL, DATA),
                                       (&Type::VECTOR, DATA),
                                       (VARARG, &Type::ANY, NO_EVAL)),
                               EXEC_DISPATCH(&DefunForm::execnode_decl)),
                              (FN_ARGS((&Type::SYMBOL, DATA),
                                       (&Type::STRING, DATA),
                                       (&Type::VECTOR, DATA),
                                       (VARARG, &Type::ANY, NO_EVAL)),
                               EXEC_DISPATCH(&DefunForm::execnode_decl_docstring))))

  SFORM_LOWER_IMPL(DefunForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() < 3)
    {
      throw RooException("Invalid defun form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Roo::Form::SYMBOL)
    {
      throw TypeError("Invalid function name: " + elements[0]->to_string());
    }
    if (elements[2]->get_type() != Roo::Form::VECTOR &&
        !(elements.size() >= 4 && (elements[2]->get_type() == Roo::Form::STRING ||
                                   elements[3]->get_type() == Roo::Form::VECTOR)))
    {
      throw RooException("Invalid defun form: " + ast_node->to_string());
    }

    Roo::AST::Symbol& fn_name = elements[1]->as<Roo::AST::Symbol>();
    sptr_ast_node doc_string =
      elements[2]->get_type() == Roo::Form::STRING ? elements[2] : nullptr;
    sptr_ast_node arg_vec = !doc_string ? elements[2] : elements[3];

    sptr_ast_node_v body;
    body.reserve(elements.size() - (!doc_string ? 3 : 4));

    for (size_t i = !doc_string ? 3 : 4; i < elements.size(); i++)
    {
      body.push_back(elements[i]);
    }

    std::shared_ptr<UserFunction> func = create_function(fn_name.value,
                                                         *ctx.ctx,
                                                         ctx.ctx->get_current_namespace(),
                                                         *arg_vec,
                                                         body);
    sptr_val fn = Value::executable(func);
    ctx.ctx->store_namespace(fn_name.value, fn);

    return std::make_unique<ExecNode>(Value::executable(func));
  }
  EXECNODE_BODY(DefunForm, execnode_decl)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }
  EXECNODE_BODY(DefunForm, execnode_decl_docstring)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

  /** FnForm - roo/fn */
  SPECIAL_FORM_IMPL(FnForm,
                    SIG((FN_ARGS((&Type::VECTOR, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&FnForm::execnode_decl))))

  SFORM_LOWER_IMPL(FnForm)
  {
    sptr_ast_node_v& elements = ast_node->get_children();
    if (elements.size() < 2)
    {
      throw RooException("Invalid fn form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Roo::Form::VECTOR)
    {
      throw RooException("Invalid fn form: " + ast_node->to_string());
    }

    sptr_ast_node arg_vec = elements[1];
    sptr_ast_node_v body;
    body.reserve(elements.size() - 2);

    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(elements[i]);
    }

    std::shared_ptr<UserFunction> func = create_function("<lambda>",
                                                         *ctx.ctx,
                                                         ctx.ctx->get_current_namespace(),
                                                         *arg_vec,
                                                         body,
                                                         &ctx);
    sptr_val fn = Value::executable(func);

    return std::make_unique<ExecNode>(LambdaNode(func));
  }

  EXECNODE_BODY(FnForm, execnode_decl)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

} // namespace Roo
