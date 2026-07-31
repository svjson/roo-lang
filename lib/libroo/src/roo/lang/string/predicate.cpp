#include "roo/lang/string/predicate.h"

#include <string>

#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    bool starts_with(const std::string& text, const std::string& prefix)
    {
      return text.size() >= prefix.size() &&
             text.compare(0, prefix.size(), prefix) == 0;
    }

    bool ends_with(const std::string& text, const std::string& suffix)
    {
      return text.size() >= suffix.size() &&
             text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    sptr_val bool_value(bool value)
    {
      return value ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
    }
  } // namespace

  /** StringStartsWithPFunction - roo.string/starts-with? */
  FUNC_IMPL(StringStartsWithPFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&StringStartsWithPFunction::exec_starts_with_p))))

  EXEC_BODY(StringStartsWithPFunction, exec_starts_with_p)
  {
    return bool_value(starts_with(args[0]->str(), args[1]->str()));
  }

  /** StringEndsWithPFunction - roo.string/ends-with? */
  FUNC_IMPL(StringEndsWithPFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&StringEndsWithPFunction::exec_ends_with_p))))

  EXEC_BODY(StringEndsWithPFunction, exec_ends_with_p)
  {
    return bool_value(ends_with(args[0]->str(), args[1]->str()));
  }
} // namespace Roo
