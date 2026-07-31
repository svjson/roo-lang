#include "roo/lang/string/string_namespace.h"

#include <map>
#include <string>

#include <roo/lang/string/predicate.h>
#include <roo/lang/string/replace.h>
#include <roo/runtime/value.h>

namespace Roo
{
  Namespace make_string_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("ends-with?", StringEndsWithPFunction::make());
    symbols.emplace("replace", StringReplaceFunction::make());
    symbols.emplace("replace-first", StringReplaceFirstFunction::make());
    symbols.emplace("starts-with?", StringStartsWithPFunction::make());

    return Namespace::make_lang("roo.string", std::move(symbols));
  }
} // namespace Roo
