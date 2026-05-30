#ifndef ROOTEST__HOST__VECTORGFX_NATIVE_ADAPTERS_H
#define ROOTEST__HOST__VECTORGFX_NATIVE_ADAPTERS_H

#include <roo/exec.h>
#include <roo/host/object.h>
#include <roo/namespace.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

#include "vectorgfx_impl.h"

namespace RooTest::Native
{
  // ===============================================================
  // Vector graphics example - adapters - Value-based
  // ===============================================================

  inline const Roo::HostTypeRef POINT("point", "pixils.point/make-point");

  NATIVE_ADAPTER(PointAdapter, Point, (x, y));

  FUNC(MakePointFunction, make_point);

  FUNC(PointRotateFunction, rotate_point);

  FUNC(PointAddFunction, plus);

  FUNC(PointDivideFunction, divide);

  FUNC(PointDistanceFunction, distance);

  class PointNamespace : public Roo::Namespace
  {
   public:
    PointNamespace(const std::string& name);
  };

} // namespace RooTest::Native

#endif
