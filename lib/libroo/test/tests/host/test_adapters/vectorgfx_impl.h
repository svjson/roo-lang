#ifndef ROOTEST__HOST__VECTORGFX_IMPL_H
#define ROOTEST__HOST__VECTORGFX_IMPL_H

#include <string>

#include <roo/form.h>
#include <roo/host/type.h>

namespace RooTest
{
  // ===============================================================
  // Vector graphics example
  // ===============================================================

  struct Point
  {
    Point() = default;
    Point(float x, float y)
      : x(x)
      , y(y)
    {
    }

    float x;
    float y;
  };

} // namespace RooTest

#endif /* ROOTEST__HOST__VECTORGFX_IMPL_H */
