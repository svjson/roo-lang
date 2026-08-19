#ifndef ROOPL__BINDING__TARGET__DEDICATED_ADAPTER_H
#define ROOPL__BINDING__TARGET__DEDICATED_ADAPTER_H

#include <roo/host/object.h>
#include <roo/host/type.h>

#include <target/dedicated.h>

namespace Roo::Roopl
{
  inline const Roo::HostTypeRef ROOPL_DEDICATED_TARGET_TYPE("roopl-dedicated-target");

  NATIVE_ADAPTER(DedicatedTargetAdapter, Target::Dedicated);
} // namespace Roo::Roopl

#endif /* ROOPL__BINDING__TARGET__DEDICATED_ADAPTER_H */
