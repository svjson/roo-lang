
#include "lisple/exception.h"
#include "lisple/runtime/lower.h"

#include <lisple/exec.h>
#include <lisple/impl.h>
#include <lisple/lang/func.h>

namespace Lisple
{
  /** ApplyFunction - apply */
  FUNC_IMPL(ApplyFunction,
            SIG((FN_ARGS((&Lisple::Type::EXEC), (&Lisple::Type::SEQ)),
                 EXEC_DISPATCH(&ApplyFunction::exec_apply))))

  EXEC_BODY(ApplyFunction, exec_apply)
  {
    auto& fn = args[0]->exec();
    sptr_rtval_v& fn_args = std::get<sptr_rtval_v>(args.back()->value);
    return fn.execute(ctx, fn_args);
  }

  /** DefunForm - defun */
  SPECIAL_FORM_IMPL(
    DefunForm,
    MULTI_SIG(
      (FN_ARGS((&Type::WORD, DATA), (&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
       EXEC_DISPATCH(&DefunForm::execnode_decl)),
      (FN_ARGS((&Type::WORD, DATA),
               (&Type::STRING, DATA),
               (&Type::ARRAY, DATA),
               (VARARG, &Type::ANY, NO_EVAL)),
       EXEC_DISPATCH(&DefunForm::execnode_decl_docstring))))

  SFORM_LOWER_IMPL(DefunForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements.size() < 3)
    {
      throw LispleException("Invalid defun form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Lisple::Form::WORD)
    {
      throw TypeError("Invalid function name: " + elements[0]->to_string());
    }
    if (elements[2]->get_type() != Lisple::Form::ARRAY &&
        !(elements.size() >= 4 && (elements[2]->get_type() == Lisple::Form::STRING ||
                                   elements[3]->get_type() == Lisple::Form::ARRAY)))
    {
      throw LispleException("Invalid defun form: " + ast_node->to_string());
    }

    Lisple::Word& fn_name = elements[1]->as<Lisple::Word>();
    sptr_sobject doc_string =
      elements[2]->get_type() == Lisple::Form::STRING ? elements[2] : nullptr;
    sptr_sobject arg_vec = !doc_string ? elements[2] : elements[3];

    sptr_sobject_v body;
    body.reserve(elements.size() - (!doc_string ? 3 : 4));

    for (size_t i = !doc_string ? 3 : 4; i < elements.size(); i++)
    {
      body.push_back(elements[i]);
    }

    sptr_sobject func = create_function(fn_name.value,
                                        *ctx.ctx,
                                        ctx.ctx->get_current_namespace(),
                                        *arg_vec,
                                        body);
    sptr_rtval fn = RTValue::executable(func);
    ctx.ctx->store_namespace(fn_name.value, fn);

    return std::make_unique<ExecNode>(RTValue::executable(func));
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

  /** FnForm - fn */
  SPECIAL_FORM_IMPL(FnForm,
                    SIG((FN_ARGS((&Type::ARRAY, DATA), (VARARG, &Type::ANY, NO_EVAL)),
                         EXEC_DISPATCH(&FnForm::execnode_decl))))

  SFORM_LOWER_IMPL(FnForm)
  {
    sptr_sobject_v& elements = ast_node->get_children();
    if (elements.size() < 2)
    {
      throw LispleException("Invalid fn form: " + ast_node->to_string());
    }
    if (elements[1]->get_type() != Lisple::Form::ARRAY)
    {
      throw LispleException("Invalid fn form: " + ast_node->to_string());
    }

    sptr_sobject arg_vec = elements[1];
    sptr_sobject_v body;
    body.reserve(elements.size() - 2);

    for (size_t i = 2; i < elements.size(); i++)
    {
      body.push_back(elements[i]);
    }

    sptr_sobject func = create_function("<lambda>",
                                        *ctx.ctx,
                                        ctx.ctx->get_current_namespace(),
                                        *arg_vec,
                                        body);
    sptr_rtval fn = RTValue::executable(func);

    return std::make_unique<ExecNode>(LambdaNode(sptr_sobject_cast<Function>(func)));
  }

  EXECNODE_BODY(FnForm, execnode_decl)
  {
    deprecated_special_form_invocations++;

    return Constant::NIL;
  }

} // namespace Lisple
