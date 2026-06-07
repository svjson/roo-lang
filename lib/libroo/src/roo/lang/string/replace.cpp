#include "roo/lang/string/replace.h"

#include <string>

#include <roo/context.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    std::string replace_all(std::string source,
                            const std::string& search,
                            const std::string& replacement)
    {
      if (search.empty())
      {
        return source;
      }

      size_t pos = 0;
      while ((pos = source.find(search, pos)) != std::string::npos)
      {
        source.replace(pos, search.size(), replacement);
        pos += replacement.size();
      }
      return source;
    }

    std::string replace_first(std::string source,
                              const std::string& search,
                              const std::string& replacement)
    {
      if (search.empty())
      {
        return source;
      }

      const size_t pos = source.find(search);
      if (pos == std::string::npos)
      {
        return source;
      }

      source.replace(pos, search.size(), replacement);
      return source;
    }
  } // namespace

  /** StringReplaceFunction - roo.string/replace */
  FUNC_IMPL(StringReplaceFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&StringReplaceFunction::exec_replace))))

  EXEC_BODY(StringReplaceFunction, exec_replace)
  {
    return Value::string(replace_all(args[0]->str(), args[1]->str(), args[2]->str()));
  }

  /** StringReplaceFirstFunction - roo.string/replace-first */
  FUNC_IMPL(StringReplaceFirstFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&StringReplaceFirstFunction::exec_replace_first))))

  EXEC_BODY(StringReplaceFirstFunction, exec_replace_first)
  {
    return Value::string(replace_first(args[0]->str(), args[1]->str(), args[2]->str()));
  }
} // namespace Roo
