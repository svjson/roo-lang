
#ifndef __SEXP_READER_H_
#define __SEXP_READER_H_

#include <string>

#include "lexer.h"
#include "parser.h"
#include "type.h"

namespace Lisple
{
  class Reader
  {
    Lexer lexer;
    Parser parser;

  public:
    sptr_sobject_v read_sexps(const std::string& input) const;
  };
}

#endif
