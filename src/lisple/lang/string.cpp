
#include <algorithm>
#include <cctype>

#include <lisple/impl.h>
#include <lisple/lang/string.h>
#include <lisple/type.h>

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

  /** JoinFunction - join */
  FUNC_IMPL(JoinFunction,
            SIG((FN_ARGS((VARARG, &Type::STRING)), EXEC_DISPATCH(&JoinFunction::exec_join))))

  EXEC_BODY(JoinFunction, exec_join)
  {
    if (args.size() < 2) return RTValue::string("");

    const std::string& joiner = args[0]->str();
    std::string result = args[1]->str();

    for (size_t i = 2; i < args.size(); i++)
    {
      result += joiner;
      result += args[i]->str();
    }

    return RTValue::string(result);
  }

  /** UpperCaseFunction - upper-case */
  FUNC_IMPL(UpperCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&UpperCaseFunction::exec_upper_case))))

  EXEC_BODY(UpperCaseFunction, exec_upper_case)
  {
    std::string str =
      args[0]->type == RTValue::Type::STRING ? args[0]->str() : args[0]->to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return RTValue::string(str);
  }

  /** LowerCaseFunction - lower-case */
  FUNC_IMPL(LowerCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&LowerCaseFunction::exec_lower_case))))

  EXEC_BODY(LowerCaseFunction, exec_lower_case)
  {
    std::string str =
      args[0]->type == RTValue::Type::STRING ? args[0]->str() : args[0]->to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return RTValue::string(str);
  }

} // namespace Lisple
