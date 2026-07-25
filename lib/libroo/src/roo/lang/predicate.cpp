
#include "roo/runtime/value.h"
#include "roo/type.h"

#include <roo/lang/predicate.h>
#include <roo/runtime/seq.h>

namespace Roo
{
  /** BetweenPredicateFunction - between? */
  FUNC_IMPL(BetweenPredicateFunction,
            SIG((FN_ARGS((&Type::NUMBER), (&Type::NUMBER), (&Type::NUMBER)),
                 EXEC_DISPATCH(&BetweenPredicateFunction::exec_between))))

  EXEC_BODY(BetweenPredicateFunction, exec_between)
  {
    float num = std::get<const Value::Number>(args[0]->value).get_float();
    float min = std::get<const Value::Number>(args[1]->value).get_float();
    float max = std::get<const Value::Number>(args[2]->value).get_float();

    return num > min && num < max ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** EmptyPFunction - empty? */
  FUNC_IMPL(EmptyPFunction,
            SIG((FN_ARGS((&Roo::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&EmptyPFunction::exec_emptyp))))

  EXEC_BODY(EmptyPFunction, exec_emptyp)
  {
    return Roo::count(*args[0]) == 0 ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** CallablePFunction - callable? */
  FUNC_IMPL(CallablePFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&CallablePFunction::exec_callable))))

  EXEC_BODY(CallablePFunction, exec_callable)
  {
    return args[0]->type == Value::Type::FUNCTION || args[0]->type == Value::Type::KEYWORD
             ? Constant::BOOL_TRUE
             : Constant::BOOL_FALSE;
  }

  /** FnPFunction - fn? */
  FUNC_IMPL(FnPFunction, SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&FnPFunction::exec_fn))))

  EXEC_BODY(FnPFunction, exec_fn)
  {
    return args[0]->type == Value::Type::FUNCTION ? Constant::BOOL_TRUE
                                                  : Constant::BOOL_FALSE;
  }

  /** NotEmptyPFunction */
  FUNC_IMPL(NotEmptyPFunction,
            SIG((FN_ARGS((&Roo::Type::SEQ_OR_STRING)),
                 EXEC_DISPATCH(&NotEmptyPFunction::exec_not_emptyp))))

  EXEC_BODY(NotEmptyPFunction, exec_not_emptyp)
  {
    return Roo::count(*args[0]) == 0 ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

  /** NotEqualsFunction - not= */
  FUNC_IMPL(NotEqualsFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&NotEqualsFunction::exec_not_equals))))

  EXEC_BODY(NotEqualsFunction, exec_not_equals)
  {
    return *args[0] == *args[1] ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

  /** SeqPFunction - seq? */
  FUNC_IMPL(SeqPFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&SeqPFunction::exec_seq))))

  EXEC_BODY(SeqPFunction, exec_seq)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::BOOL_FALSE;
    return Type::SEQ.is_type_of(*args[0]) ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** SeqLikePFunction - seq-like? */
  FUNC_IMPL(SeqLikePFunction,
            SIG((FN_ARGS((&Type::ANY)),
                 EXEC_DISPATCH(&SeqLikePFunction::exec_seq_like))))

  EXEC_BODY(SeqLikePFunction, exec_seq_like)
  {
    if (args[0]->type == Value::Type::NIL) return Constant::BOOL_FALSE;
    return Type::SEQ_OR_STRING.is_type_of(*args[0]) ? Constant::BOOL_TRUE
                                                    : Constant::BOOL_FALSE;
  }

  /** NilPFunction - nil? */
  FUNC_IMPL(NilPFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NilPFunction::exec_is_nil))))

  EXEC_BODY(NilPFunction, exec_is_nil)
  {
    return args[0]->type == Value::Type::NIL ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** NotFunction - not */
  FUNC_IMPL(NotFunction, SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&NotFunction::exec_not))))

  EXEC_BODY(NotFunction, exec_not)
  {
    return Roo::is_truthy(*args[0]) ? Constant::BOOL_FALSE : Constant::BOOL_TRUE;
  }

} // namespace Roo
