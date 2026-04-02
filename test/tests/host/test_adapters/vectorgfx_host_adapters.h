#ifndef LISPLETEST__HOST__VECTORGFX_ADAPTERS_H
#define LISPLETEST__HOST__VECTORGFX_ADAPTERS_H

#include <lisple/exec.h>
#include <lisple/host.h>
#include <lisple/host/object.h>
#include <lisple/namespace.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

#include "vectorgfx_impl.h"

namespace LispleTest
{
  // ===============================================================
  // Vector graphics example - adapters - RTValue-based
  // ===============================================================

  inline const Lisple::HostTypeRef POINT("point", "pixils.point/make-point");

  HOST_ADAPTER(PointAdapter, Point, (x, y));

  FUNC_DECL(MakePointASTFunction, make_point);

  FUNC_DECL(PointRotateASTFunction, rotate_point);

  FUNC_DECL(PointAddASTFunction, plus);

  FUNC_DECL(PointDivideASTFunction, divide);

  FUNC_DECL(PointDistanceASTFunction, distance);

  class PointASTBasedNamespace : public Lisple::Namespace
  {
   public:
    PointASTBasedNamespace(const std::string& name);
  };

} // namespace LispleTest

#endif
