#ifndef LISPLETEST__HOST__VECTORGFX_IMPL_H
#define LISPLETEST__HOST__VECTORGFX_IMPL_H

#include <string>

#include <lisple/form.h>
#include <lisple/host/type.h>

namespace LispleTest
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

} // namespace LispleTest

#endif /* LISPLETEST__HOST__VECTORGFX_IMPL_H */
