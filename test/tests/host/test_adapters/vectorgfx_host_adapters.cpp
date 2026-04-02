
#include "vectorgfx_host_adapters.h"

#include <cmath>

namespace LispleTest
{
  SHKEY(KEY_X, "x");
  SHKEY(KEY_Y, "y");

  HOST_ADAPTER_IMPL(PointAdapter,
                    Point,
                    &POINT,
                    ({K_GET(PointAdapter, KEY_X, x), K_GET(PointAdapter, KEY_Y, y)}));

  ADAPTER_PROP_GET__FIELD(PointAdapter, x, Lisple::Number);
  ADAPTER_PROP_GET__FIELD(PointAdapter, y, Lisple::Number);

  /** AST-based make-point */
  FUNC_IMPL(MakePointASTFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&MakePointASTFunction::make_point))));

  FUNC_BODY(MakePointASTFunction, make_point)
  {
    if (args[0]->get_type() != Lisple::Form::MAP ||
        args[0]->get_sptr_property(*KEY_X)->get_type() != Lisple::Form::NUMBER ||
        args[0]->get_sptr_property(*KEY_Y)->get_type() != Lisple::Form::NUMBER)
      throw Lisple::LispleException("Invalid input");

    return PointAdapter::make<Point>(
      args[0]->get_sptr_property(*KEY_X)->as<const Lisple::Number>().float_value(),
      args[0]->get_sptr_property(*KEY_Y)->as<const Lisple::Number>().float_value());
  }

  /** AST-based point/plus */
  FUNC_IMPL(PointAddASTFunction,
            SIG((FN_ARGS((&POINT), (&POINT)), EXEC_DISPATCH(&PointAddASTFunction::plus))));

  FUNC_BODY(PointAddASTFunction, plus)
  {
    Point& a = args[0]->as<PointAdapter>().get_object();
    Point& b = args[1]->as<PointAdapter>().get_object();

    return PointAdapter::make<Point>(a.x + b.x, a.y + b.y);
  }

  /** AST-based point/divide */
  FUNC_IMPL(PointDivideASTFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointDivideASTFunction::divide))));

  FUNC_BODY(PointDivideASTFunction, divide)
  {
    Point& a = args[0]->as<PointAdapter>().get_object();
    float div = args[1]->as<Lisple::Number>().float_value();

    return PointAdapter::make<Point>(a.x / div, a.y + div);
  }

  FUNC_IMPL(PointDistanceASTFunction,
            SIG((FN_ARGS((&POINT), (&POINT)),
                 EXEC_DISPATCH(&PointDistanceASTFunction::distance))));

  FUNC_BODY(PointDistanceASTFunction, distance)
  {
    Point& a = args[0]->as<PointAdapter>().get_object();
    Point& b = args[1]->as<PointAdapter>().get_object();

    return Lisple::Number::make(
      std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
  }

  /** AST-based point/rotate */
  FUNC_IMPL(PointRotateASTFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointRotateASTFunction::rotate_point))));

  FUNC_BODY(PointRotateASTFunction, rotate_point)
  {
    const Point& point = args[0]->as<PointAdapter>().get_object();
    const Point origin = {0, 0};
    float amount = args[1]->as<Lisple::Number>().float_value();

    if (amount == 0.0) return args[0];

    float s = std::sin(amount);
    float c = std::cos(amount);

    float x = point.x - origin.x;
    float y = point.x - origin.y;

    float x_new = x * c - y * s;
    float y_new = x * s + y * c;

    return PointAdapter::make<Point>(x_new + origin.x, y_new + origin.y);
  }

  PointASTBasedNamespace::PointASTBasedNamespace(const std::string& name)
    : Namespace(name)
  {
    objects.emplace("plus", std::make_shared<PointAddASTFunction>());
    objects.emplace("div", std::make_shared<PointDivideASTFunction>());
    objects.emplace("rotate", std::make_shared<PointRotateASTFunction>());
    objects.emplace("distance", std::make_shared<PointDistanceASTFunction>());
    objects.emplace("make-point", std::make_shared<MakePointASTFunction>());
  }

} // namespace LispleTest
