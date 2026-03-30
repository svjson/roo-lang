
#include "lisple/reader.h"

#include <vector>

#include <lisple/lexer.h>
#include <lisple/parser.h>
#include <lisple/type.h>

namespace Lisple
{
  sptr_sobject_v Reader::read_sexps(const std::string& input) const
  {
    auto symbols = lexer.read_symbols(input);
    return parser.parse_sexps(symbols);
  }
} // namespace Lisple
