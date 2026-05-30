
#ifndef __LISP_PARSER_H_
#define __LISP_PARSER_H_

#include <cstdint>
#include <memory>
#include <vector>

#include <roo/source.h>
#include <roo/type.h>

namespace Roo
{
  class TokenSymbol;
  enum class Token : uint8_t;

  class ParseContext
  {
   public:
    sptr_ast_node_v forms;
    sptr_ast_node_v stack;
    std::vector<Token> closing_tokens;

    void begin_list(Token start_type, const SourceRef& source = {});
    void begin_hash_context();
    void close_context(Token end_token, const SourceSpan& end_span = {});

    void append(std::shared_ptr<AST::ASTNode> obj);
  };

  class Parser
  {
   public:
    sptr_ast_node_v parse_sexps(std::vector<TokenSymbol> symbols,
                                uint32_t source_file_id = 0,
                                bool source_diagnostics = false) const;
  };
} // namespace Roo

#endif
