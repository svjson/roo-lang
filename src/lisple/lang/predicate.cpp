
#include "lisple/runtime/value.h"
#include "lisple/type.h"

#include <lisple/lang/predicate.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /** BetweenPredicateFunction - between? */
  FUNC_IMPL(BetweenPredicateFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&BetweenPredicateFunction::exec_between))))

  EXEC_BODY(BetweenPredicateFunction, exec_between)
  {
    float num = std::get<const RTValue::Number>(args[0]->value).get_float();
    float min = std::get<const RTValue::Number>(args[1]->value).get_float();
    float max = std::get<const RTValue::Number>(args[2]->value).get_float();

    return num > min && num < max ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** EmptyPFunction - empty? */
  FUNC_IMPL(EmptyPFunction,
            SIG((FN_ARGS((&Lisple::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&EmptyPFunction::exec_emptyp))))

  EXEC_BODY(EmptyPFunction, exec_emptyp)
  {
    return Lisple::count(*args[0]) == 0 ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** NotEmptyPFunction */
  FUNC_IMPL(NotEmptyPFunction,
            SIG((FN_ARGS((&Lisple::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&NotEmptyPFunction::exec_not_emptyp))))

  EXEC_BODY(NotEmptyPFunction, exec_not_emptyp)
  {
    return Lisple::count(*args[0]) == 0 ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

  /** NotEqualsFunction - not= */
  FUNC_IMPL(NotEqualsFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&NotEqualsFunction::exec_not_equals))))

  EXEC_BODY(NotEqualsFunction, exec_not_equals)
  {
    return *args[0] == *args[1] ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

  /** NilPFunction - nil? */
  FUNC_IMPL(NilPFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NilPFunction::exec_is_nil))))

  EXEC_BODY(NilPFunction, exec_is_nil)
  {
    return args[0]->type == RTValue::Type::NIL ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** NotFunction - not */
  FUNC_IMPL(NotFunction, SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NotFunction::exec_not))))

  EXEC_BODY(NotFunction, exec_not)
  {
    return Lisple::is_truthy(*args[0]) ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

} // namespace Lisple
