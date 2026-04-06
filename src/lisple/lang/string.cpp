
#include <lisple/lang/string.h>

namespace Lisple
{
  /**
   * StrFunction - str
   */
  FUNC_IMPL(StrFunction,
            SIG((FN_ARGS((VARARG, &Type::ANY)), EXEC_DISPATCH(&StrFunction::exec_str))))

  /**
   * Uses regular string concatenation with += which forces re-allocation as the string
   * content grows.
   *
   * Attempts to pre-reserve by computing the total length up front were
   * benchmarked and found to be consistently slower, as the cost of the measurement
   * pass outweighs the savings from avoiding reallocations - the allocator's
   * geometric growth strategy wins at every arg count tested (5–1000 args).
   */
  EXEC_BODY(StrFunction, exec_str)
  {
    std::string result;

    for (auto& arg : args)
    {
      switch (arg->type)
      {
      case RTValue::Type::STRING:
        result += std::get<std::string>(arg->value);
        break;
      case RTValue::Type::CHAR:
        result += std::get<char>(arg->value);
        break;
      default:
        result += arg->to_string();
        break;
      }
    }

    return RTValue::string(result);
  }

} // namespace Lisple
