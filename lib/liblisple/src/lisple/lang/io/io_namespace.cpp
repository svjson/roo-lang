#include "lisple/lang/io/io_namespace.h"

#include <map>
#include <string>

#include <lisple/lang/io/file.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  Namespace make_io_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("slurp!", SlurpBangFunction::make());
    symbols.emplace("spit!", SpitBangFunction::make());
    symbols.emplace("slurp-edn!", SlurpEdnBangFunction::make());
    symbols.emplace("spit-edn!", SpitEdnBangFunction::make());

    return Namespace::make_lang("lisple.io", std::move(symbols));
  }
} // namespace Lisple
