
#include "predicate.h"

namespace Lisple
{
  /** NotFunction - not */
  FUNC_IMPL(NotFunction, SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NotFunction::exec_not))))

  EXEC_BODY(NotFunction, exec_not)
  {
    return Lisple::is_truthy(*args[0]) ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

} // namespace Lisple
