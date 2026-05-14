#ifndef LISPLETEST__HOST__VECTORGFX_NATIVE_ADAPTERS_H
#define LISPLETEST__HOST__VECTORGFX_NATIVE_ADAPTERS_H

#include <lisple/exec.h>
#include <lisple/host/object.h>
#include <lisple/namespace.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

#include "vectorgfx_impl.h"

namespace LispleTest::Native
{
  // ===============================================================
  // Vector graphics example - adapters - Value-based
  // ===============================================================

  inline const Lisple::HostTypeRef POINT("point", "pixils.point/make-point");

  NATIVE_ADAPTER(PointAdapter, Point, (x, y));

  FUNC(MakePointFunction, make_point);

  FUNC(PointRotateFunction, rotate_point);

  FUNC(PointAddFunction, plus);

  FUNC(PointDivideFunction, divide);

  FUNC(PointDistanceFunction, distance);

  class PointNamespace : public Lisple::Namespace
  {
   public:
    PointNamespace(const std::string& name);
  };

} // namespace LispleTest::Native

#endif
