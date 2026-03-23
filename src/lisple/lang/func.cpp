
#include "func.h"

#include "../exec.h"

namespace Lisple
{
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
    return RTValue::object(fn);
  }

} // namespace Lisple
