
#include <roo/data/string_literal.h>

namespace Roo
{
  std::string escaped_string_literal(const std::string& value)
  {
    std::string result = "\"";
    for (char ch : value)
    {
      switch (ch)
      {
      case '\\':
        result += "\\\\";
        break;
      case '"':
        result += "\\\"";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        result += ch;
        break;
      }
    }
    result += "\"";
    return result;
  }
} // namespace Roo
