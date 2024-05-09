
#include "reader.h"

#include "lexer.h"
#include "parser.h"
#include "type.h"

namespace Lisple
{
  sptr_sobject_v Reader::read_sexps(const std::string& input) const
  {
    auto symbols = lexer.read_symbols(input);
    return parser.parse_sexps(symbols);
  }
}

