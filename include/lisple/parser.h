
#ifndef __LISP_PARSER_H_
#define __LISP_PARSER_H_

#include <cstdint>
#include <memory>
#include <vector>

#include <lisple/type.h>

namespace Lisple
{
  class Object;
  class Symbol;
  enum class Token : uint8_t;

  class ParseContext
  {
   public:
    sptr_sobject_v forms;
    sptr_sobject_v stack;
    std::vector<Token> closing_tokens;

    void begin_list(Token start_type);
    void begin_hash_context();
    void close_context(Token end_token);

    void append(std::shared_ptr<Object> obj);
  };

  class Parser
  {
   public:
    sptr_sobject_v parse_sexps(std::vector<Symbol> symbols) const;
  };
} // namespace Lisple

#endif
