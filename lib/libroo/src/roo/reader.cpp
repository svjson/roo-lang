
#include "roo/reader.h"

#include <vector>

#include <roo/lexer.h>
#include <roo/parser.h>
#include <roo/type.h>

namespace Roo
{
  sptr_ast_node_v Reader::read_sexps(const std::string& input,
                                     uint32_t source_file_id,
                                     bool source_diagnostics) const
  {
    auto symbols = lexer.read_symbols(input, source_diagnostics);
    return parser.parse_sexps(symbols, source_file_id, source_diagnostics);
  }
} // namespace Roo
