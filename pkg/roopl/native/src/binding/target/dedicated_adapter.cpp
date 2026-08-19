#include <binding/target/dedicated_adapter.h>

namespace Roo::Roopl
{
  NATIVE_ADAPTER_IMPL(DedicatedTargetAdapter,
                      Target::Dedicated,
                      &ROOPL_DEDICATED_TARGET_TYPE);
} // namespace Roo::Roopl
