#include "optional_native_adapters.h"

#include <lisple/host/accessor.h>
#include <lisple/host/object.h>

namespace LispleTest::Native
{
  NATIVE_ADAPTER_IMPL(OptionalBoxAdapter, OptionalBox, &OPTIONAL_BOX, (rw, "count", count));

  NOBJ_PROP_GET_SET_OPT__FIELD(OptionalBoxAdapter, count);

  NATIVE_ADAPTER_IMPL(OptionalChildAdapter,
                      OptionalChild,
                      &OPTIONAL_CHILD,
                      (rw, "value", value));

  NOBJ_PROP_GET_SET__FIELD(OptionalChildAdapter, value);

  NATIVE_ADAPTER_IMPL(OptionalChildBoxAdapter,
                      OptionalChildBox,
                      &OPTIONAL_CHILD_BOX,
                      (rw, "child", child));

  NOBJ_PROP_GET_SET_OPT_ADAPTER__FIELD(OptionalChildBoxAdapter, child, OptionalChildAdapter);

  NATIVE_ADAPTER_IMPL(OptionalChildPtrBoxAdapter,
                      OptionalChildPtrBox,
                      &OPTIONAL_CHILD_PTR_BOX,
                      (rw, "child", child));

  NOBJ_PROP_GET_SET_ADAPTER_P__FIELD(OptionalChildPtrBoxAdapter,
                                     child,
                                     OptionalChildAdapter);
} // namespace LispleTest::Native
