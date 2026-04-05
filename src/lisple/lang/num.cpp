
#include "lisple/lang/num.h"

#include "lisple/exec.h"
#include "lisple/runtime/value.h"

namespace Lisple
{

  /* EvenPFunction - even? */
  FUNC_IMPL(EvenPFunction,
            SIG((FN_ARGS((&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&EvenPFunction::exec_even))))

  EXEC_BODY(EvenPFunction, exec_even)
  {
    return std::get<const RTValue::Number>(args[0]->value).get_int() % 2 == 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /* OddPFunction - odd? */
  FUNC_IMPL(OddPFunction,
            SIG((FN_ARGS((&Lisple::Type::NUMBER)), EXEC_DISPATCH(&OddPFunction::exec_odd))))

  EXEC_BODY(OddPFunction, exec_odd)
  {
    return std::get<const RTValue::Number>(args[0]->value).get_int() % 2 != 0
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

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
