
#include <cmath>

#include <lisple/lang/math.h>

namespace Lisple
{
  /* AbsFunction - abs */
  FUNC_IMPL(AbsFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&AbsFunction::exec_abs))))

  EXEC_BODY(AbsFunction, exec_abs)
  {
    return Value::number(
      std::abs(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* CeilFunction - ceil */
  FUNC_IMPL(CeilFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&CeilFunction::exec_ceil))))

  EXEC_BODY(CeilFunction, exec_ceil)
  {
    const Value::Number& num = std::get<const Value::Number>(args[0]->value);
    return Value::number(static_cast<int>(std::ceil(num.get_float())));
  }

  /* SinFunction - sin */
  FUNC_IMPL(SinFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&SinFunction::exec_sin))))

  EXEC_BODY(SinFunction, exec_sin)
  {
    return Value::number(
      std::sin(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* CosFunction - cos */
  FUNC_IMPL(CosFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&CosFunction::exec_cos))))

  EXEC_BODY(CosFunction, exec_cos)
  {
    return Value::number(
      std::cos(std::get<const Value::Number>(args[0]->value).get_float()));
  }

  /* SqrtFunction - sqrt */
  FUNC_IMPL(SqrtFunction,
            SIG((FN_ARGS((&Type::NUMBER)), EXEC_DISPATCH(&SqrtFunction::exec_sqrt))))

  EXEC_BODY(SqrtFunction, exec_sqrt)
  {
    return Value::number(
      std::sqrt(std::get<const Value::Number>(args[0]->value).get_float()));
  }

} // namespace Lisple
