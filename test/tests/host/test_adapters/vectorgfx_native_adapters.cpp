#include "vectorgfx_native_adapters.h"

#include <cmath>

#include <lisple/host/object.h>
#include <lisple/runtime/dict.h>

namespace LispleTest::Native
{
  const Lisple::sptr_rtval X = Lisple::RTValue::keyword("x");
  const Lisple::sptr_rtval Y = Lisple::RTValue::keyword("y");

  NATIVE_ADAPTER_IMPL(PointAdapter,
                      Point,
                      &POINT,
                      ({NOBJ_GET(PointAdapter, X, x), NOBJ_GET(PointAdapter, Y, y)}));

  NOBJ_NOCACHE_PROP_GET__FIELD(PointAdapter, x, number);
  NOBJ_NOCACHE_PROP_GET__FIELD(PointAdapter, y, number);

  /** MakePointFunction -  make-point */
  FUNC_IMPL(MakePointFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&MakePointFunction::exec_make_point))));

  EXEC_BODY(MakePointFunction, exec_make_point)
  {
    if (args[0]->type != Lisple::RTValue::Type::MAP ||
        Lisple::Dict::get_property(args[0], X)->type != Lisple::RTValue::Type::NUMBER ||
        Lisple::Dict::get_property(args[0], Y)->type != Lisple::RTValue::Type::NUMBER)
      throw Lisple::LispleException("Invalid input");

    return PointAdapter::make_unique(
      std::get<Lisple::RTValue::Number>(Lisple::Dict::get_property(args[0], X)->value)
        .get_float(),
      std::get<Lisple::RTValue::Number>(Lisple::Dict::get_property(args[0], Y)->value)
        .get_float());
  }

  /** PointRotateFunction - pixils.point/rotate */
  FUNC_IMPL(PointRotateFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointRotateFunction::exec_rotate_point))));

  EXEC_BODY(PointRotateFunction, exec_rotate_point)
  {
    const Point& point = args.front()->adapter<PointAdapter>().get_object();
    const Point origin = {0, 0};
    float amount = std::get<Lisple::RTValue::Number>(args[1]->value).get_float();

    if (amount == 0.0) return args[0];

    float s = std::sin(amount);
    float c = std::cos(amount);

    float x = point.x - origin.x;
    float y = point.x - origin.y;

    float x_new = x * c - y * s;
    float y_new = x * s + y * c;

    return PointAdapter::make_unique(x_new + origin.x, y_new + origin.y);
  }

  /** PointAddFunction - pixils.point/plus */
  FUNC_IMPL(PointAddFunction,
            SIG((FN_ARGS((&POINT), (&POINT)), EXEC_DISPATCH(&PointAddFunction::exec_plus))));

  EXEC_BODY(PointAddFunction, exec_plus)
  {
    const Point& a = args[0]->adapter<PointAdapter>().get_object();
    const Point& b = args[1]->adapter<PointAdapter>().get_object();

    return PointAdapter::make_unique(a.x + b.x, a.y + b.y);
  }

  /** PointDivideFunction - pixils.point/div */
  FUNC_IMPL(PointDivideFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointDivideFunction::exec_divide))));

  EXEC_BODY(PointDivideFunction, exec_divide)
  {
    const Point& a = args[0]->adapter<PointAdapter>().get_object();
    float amount = std::get<Lisple::RTValue::Number>(args[1]->value).get_float();

    return PointAdapter::make_unique(a.x / amount, a.y / amount);
  }

  FUNC_IMPL(PointDistanceFunction,
            SIG((FN_ARGS((&POINT), (&POINT)),
                 EXEC_DISPATCH(&PointDistanceFunction::exec_distance))));

  EXEC_BODY(PointDistanceFunction, exec_distance)
  {
    const Point& a = args[0]->adapter<PointAdapter>().get_object();
    const Point& b = args[1]->adapter<PointAdapter>().get_object();

    return Lisple::RTValue::number(
      std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
  }

  PointNamespace::PointNamespace(const std::string& name)
    : Namespace(name)
  {
    values.emplace("plus", PointAddFunction::make());
    values.emplace("div", PointDivideFunction::make());
    values.emplace("rotate", PointRotateFunction::make());
    values.emplace("distance", PointDistanceFunction::make());
    values.emplace("make-point", MakePointFunction::make());
  }

} // namespace LispleTest::Native
