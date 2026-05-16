
#include "lisple/reader.h"

#include <vector>

#include <lisple/lexer.h>
#include <lisple/parser.h>
#include <lisple/type.h>

namespace Lisple
{
  sptr_ast_node_v Reader::read_sexps(const std::string& input,
                                     uint32_t source_file_id,
                                     bool source_diagnostics) const
  {
    auto symbols = lexer.read_symbols(input, source_diagnostics);
    return parser.parse_sexps(symbols, source_file_id, source_diagnostics);
  }
} // namespace Lisple
