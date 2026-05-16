
#ifndef __SEXP_READER_H_
#define __SEXP_READER_H_

#include <string>

#include <lisple/lexer.h>
#include <lisple/parser.h>
#include <lisple/source.h>
#include <lisple/type.h>

namespace Lisple
{
  class Reader
  {
    Lexer lexer;
    Parser parser;

   public:
    sptr_ast_node_v read_sexps(const std::string& input,
                               uint32_t source_file_id = 0,
                               bool source_diagnostics = false) const;
  };
} // namespace Lisple

#endif
