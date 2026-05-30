#ifndef ROOTEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H
#define ROOTEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H

#include "optional_impl.h"
#include <roo/host/object.h>
#include <roo/runtime/value.h>

namespace RooTest::Native
{
  inline Roo::HostTypeRef OPTIONAL_BOX("optional-box");
  inline Roo::HostTypeRef OPTIONAL_CHILD("optional-child");
  inline Roo::HostTypeRef OPTIONAL_CHILD_BOX("optional-child-box");
  inline Roo::HostTypeRef OPTIONAL_CHILD_PTR_BOX("optional-child-ptr-box");

  NATIVE_ADAPTER(OptionalBoxAdapter, OptionalBox, (count), (count));
  NATIVE_ADAPTER(OptionalChildAdapter, OptionalChild, (value), (value));
  NATIVE_ADAPTER(OptionalChildBoxAdapter, OptionalChildBox, (child), (child));
  NATIVE_ADAPTER(OptionalChildPtrBoxAdapter, OptionalChildPtrBox, (child), (child));
} // namespace RooTest::Native

#endif /* ROOTEST__HOST__OPTIONAL_NATIVE_ADAPTERS_H */
