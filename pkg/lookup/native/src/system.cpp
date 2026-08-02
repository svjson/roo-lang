#include "system.h"

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/runtime/value.h>

#include <lookup/native.h>

namespace Roo::Lookup
{
  /** FailBangFunction - lookup.system/fail! */
  FUNC_IMPL(FailBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&FailBangFunction::exec_fail))))

  EXEC_BODY(FailBangFunction, exec_fail)
  {
    throw RooException(args[0]->str());
  }

  std::unique_ptr<Namespace> make_system_namespace()
  {
    auto ns = std::make_unique<Namespace>("lookup.system");
    ns->set_origin(Namespace::Origin::native());
    ns->store("fail!", FailBangFunction::make());
    return ns;
  }
} // namespace Roo::Lookup
