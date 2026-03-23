
#include "string.h"

namespace Lisple
{
  /*
   * StrFunction - str
   */
  FUNC_IMPL(StrFunction,
            SIG((FN_ARGS((VARARG, &Type::ANY)), EXEC_DISPATCH(&StrFunction::exec_str))))

  EXEC_BODY(StrFunction, exec_str)
  {
    std::string result = "";

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
