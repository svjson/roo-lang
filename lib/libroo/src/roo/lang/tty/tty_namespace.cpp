#include "roo/lang/tty/tty_namespace.h"

#include <map>
#include <string>

#include <roo/lang/tty/tty.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.tty namespace.
   */
  Namespace make_tty_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("tty?", TtyPFunction::make());
    symbols.emplace("dimensions!", DimensionsBangFunction::make());

    return Namespace::make_lang("roo.tty", std::move(symbols));
  }
} // namespace Roo
