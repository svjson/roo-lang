
#include "lisple/lang/num.h"

#include "lisple/exec.h"
#include "lisple/runtime/value.h"

namespace Lisple
{
  FUNC_IMPL(ZeroPFunction,
            SIG((FN_ARGS((&Type::NUMBER), (VARARG, &Type::NUMBER)),
                 EXEC_DISPATCH(&ZeroPFunction::exec_zero))))

  EXEC_BODY(ZeroPFunction, exec_zero)
  {
    for (auto& arg : args)
    {
      if (arg->f64() != 0.0)
      {
        return Constant::BOOL_FALSE;
      }
    }

    return Constant::BOOL_TRUE;
  }

} // namespace Lisple
