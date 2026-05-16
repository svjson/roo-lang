#ifndef LISPLETEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H
#define LISPLETEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H

#include <lisple/host/object.h>
#include <lisple/runtime/value.h>

#include "optional_impl.h"

namespace LispleTest::Native
{
  inline Lisple::HostTypeRef OPTIONAL_BOX("optional-box");
  inline Lisple::HostTypeRef OPTIONAL_CHILD("optional-child");
  inline Lisple::HostTypeRef OPTIONAL_CHILD_BOX("optional-child-box");
  inline Lisple::HostTypeRef OPTIONAL_CHILD_PTR_BOX("optional-child-ptr-box");

  NATIVE_ADAPTER(OptionalBoxAdapter, OptionalBox, (count), (count));
  NATIVE_ADAPTER(OptionalChildAdapter, OptionalChild, (value), (value));
  NATIVE_ADAPTER(OptionalChildBoxAdapter, OptionalChildBox, (child), (child));
  NATIVE_ADAPTER(OptionalChildPtrBoxAdapter, OptionalChildPtrBox, (child), (child));
} // namespace LispleTest::Native

#endif /* LISPLETEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H */
