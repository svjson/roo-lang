#include "roo/lang/ast/ast_namespace.h"

#include <map>
#include <string>

#include <roo/lang/ast/reader.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.ast namespace.
   */
  Namespace make_ast_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("slurp!", AstSlurpBangFunction::make());
    symbols.emplace("read-string", AstReadStringFunction::make());
    return Namespace::make_lang("roo.ast", std::move(symbols));
  }
} // namespace Roo
