#include "test_host_objects.h"

#include <cmath>
#include <iostream>
#include <lisple/context.h>
#include <lisple/impl.h>
#include <lisple/runtime/dict.h>

#include "lisple/runtime/value.h"

namespace Tests
{
  // ===============================================================
  // Vehicle example - with nested type RegNumber
  // ===============================================================

  /* RegNumber */
  RegNumber::RegNumber(const std::string& letters, const std::string& numbers)
    : letters(letters)
    , numbers(numbers)
  {
  }

  const std::string& RegNumber::get_letters() const
  {
    return letters;
  }

  const std::string& RegNumber::get_numbers() const
  {
    return numbers;
  }

  bool RegNumber::operator<(const RegNumber& other) const
  {
    return this->letters < other.letters ||
           (this->letters == other.letters && this->numbers < other.numbers);
  }

  /* Vehicle */
  Vehicle::Vehicle(const std::string& model_name, int seats)
    : model_name(model_name)
    , seats(seats)
  {
  }

  const std::string& Vehicle::get_model_name() const
  {
    return model_name;
  }

  int Vehicle::get_seats() const
  {
    return seats;
  }

  void Vehicle::set_seats(int seats)
  {
    this->seats = seats;
  }

  bool Vehicle::operator==(const Vehicle& other) const
  {
    return this->model_name == other.model_name && this->seats == other.seats;
  }

  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================

  SHKEY(MODEL_NAME, "model-name")
  SHKEY(LETTERS, "letters")
  SHKEY(NUMBERS, "numbers")
  SHKEY(SEATS, "seats")

  /* RegNumberAdapter */
  HOST_ADAPTER_IMPL(RegNumberAdapter,
                    RegNumber,
                    &REGNUM_TYPE,
                    ({K_GET(RegNumberAdapter, LETTERS, letters),
                      K_GET(RegNumberAdapter, NUMBERS, numbers)}));

  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, letters, Lisple::String, get_letters);
  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, numbers, Lisple::String, get_numbers);

  /* VehicleAdapter */
  HOST_ADAPTER_IMPL(VehicleAdapter,
                    Vehicle,
                    &VEHICLE_TYPE,
                    ({K_GET(VehicleAdapter, MODEL_NAME, model_name),
                      K_GET(VehicleAdapter, SEATS, seats)}));

  Lisple ::sptr_sobject VehicleAdapter ::get_model_name() const
  {
    return std ::make_shared<Lisple ::String>(get_self_object().get_model_name());
  };
  ADAPTER_PROP_GET__METHOD(VehicleAdapter, seats, Lisple::Number, get_seats);

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  /* Vehicle Make-function */
  FUNC_IMPL(VehicleMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)), EXEC_DISPATCH(&VehicleMakeFunction::make))))

  FUNC_BODY(VehicleMakeFunction, make)
  {
    const std::string model_name =
      args.front()->as<Lisple::Map>().get_property(*MODEL_NAME).as<Lisple::String>().value;
    int seats =
      args.front()->as<Lisple::Map>().get_property(*SEATS).as<Lisple::Number>().int_value();

    return VehicleAdapter::make<Vehicle>(model_name, seats);
  }

  FUNC_IMPL(PrnVehicle, SIG((FN_ARGS((&VEHICLE_TYPE)), EXEC_DISPATCH(&PrnVehicle::prn))))

  FUNC_BODY(PrnVehicle, prn)
  {
    Vehicle& v = args.front()->as<VehicleAdapter>().get_object();
    ctx.eval("(prn \"The vehicle " + v.get_model_name() + " has " +
             std::to_string(v.get_seats()) + " seats\")");

    return args.front();
  }

  FUNC_IMPL(DoubleSizeVehicle,
            SIG((FN_ARGS((&VEHICLE_TYPE__NO_COERCE)),
                 EXEC_DISPATCH(&DoubleSizeVehicle::zoom))))

  FUNC_BODY(DoubleSizeVehicle, zoom)
  {
    Vehicle& v = args.front()->as<VehicleAdapter>().get_object();

    return VehicleAdapter::make<Vehicle>(v.get_model_name(), v.get_seats() * 2);
  }

  FUNC_IMPL(CountVehicleSeats,
            SIG((FN_ARGS((&ARRAY_OF_VEHICLE)), EXEC_DISPATCH(&CountVehicleSeats::count))))

  FUNC_BODY(CountVehicleSeats, count)
  {
    int count = 0;
    for (Lisple::sptr_sobject& element : args.front()->get_children())
    {
      count += element->as<VehicleAdapter>().get_object().get_seats();
    }
    return Lisple::Number::make(count);
  }

  FUNC_IMPL(ArrayOfArrayTaker,
            SIG((FN_ARGS((&ARRAY_OF_ARRAY_OF_VEHICLE)),
                 EXEC_DISPATCH(&ArrayOfArrayTaker::accept))))

  FUNC_BODY(ArrayOfArrayTaker, accept)
  {
    return args.front();
  }

  // ===============================================================
  // Product example - With sub types
  // ===============================================================

  Product::Product(const std::string& name, float price, int sku)
    : name(name)
    , price(price)
    , sku(sku)
  {
  }

  const std::string& Product::get_name()
  {
    return this->name;
  }
  float Product::get_price()
  {
    return this->price;
  }
  int Product::get_sku()
  {
    return this->sku;
  }

  void Product::set_price(float price)
  {
    this->price = price;
  }
  void Product::set_sku(int sku)
  {
    this->sku = sku;
  }

  Book::Book(const std::string& name,
             float price,
             int sku,
             const std::string& author,
             const std::string& isbn)
    : Product(name, price, sku)
    , author(author)
    , isbn(isbn)
  {
  }

  const std::string& Book::get_author()
  {
    return this->author;
  }
  const std::string& Book::get_isbn()
  {
    return this->isbn;
  }

  Clothing::Clothing(const std::string& name,
                     float price,
                     int sku,
                     const std::string& size,
                     const std::string& material)
    : Product(name, price, sku)
    , size(size)
    , material(material)
  {
  }

  // ===============================================================
  // Product example - adapters
  // ===============================================================

  SHKEY(NAME, "name")
  SHKEY(PRICE, "price")
  SHKEY(SKU, "sku")
  SHKEY(AUTHOR, "author")
  SHKEY(ISBN, "isbn")
  SHKEY(SIZE, "size")
  SHKEY(MATERIAL, "material")

  HOST_ADAPTER_IMPL(ProductAdapter,
                    Product,
                    &PRODUCT,
                    ({K_GET(ProductAdapter, NAME, name),
                      K_GET_SET(ProductAdapter, PRICE, price),
                      K_GET_SET(ProductAdapter, SKU, sku)}));

  ADAPTER_PROP_GET__METHOD(ProductAdapter, name, Lisple::String, get_name);
  ADAPTER_PROP_GET__METHOD(ProductAdapter, price, Lisple::Number, get_price);
  ADAPTER_PROP_SET__METHOD(ProductAdapter, price, Lisple::Number, set_price);
  ADAPTER_PROP_GET__METHOD(ProductAdapter, sku, Lisple::Number, get_sku);
  ADAPTER_PROP_SET__METHOD(ProductAdapter, sku, Lisple::Number, set_sku);

  HOST_SUB_ADAPTER_IMPL(BookAdapter,
                        Book,
                        ProductAdapter,
                        Product,
                        &BOOK,
                        ({K_GET(BookAdapter, AUTHOR, author),
                          K_GET(BookAdapter, ISBN, isbn)}));

  ADAPTER_PROP_GET__METHOD(BookAdapter, author, Lisple::String, get_author);
  ADAPTER_PROP_GET__METHOD(BookAdapter, isbn, Lisple::String, get_isbn);

  HOST_SUB_ADAPTER_IMPL(ClothingAdapter,
                        Clothing,
                        ProductAdapter,
                        Product,
                        &CLOTHING,
                        ({K_GET(ClothingAdapter, SIZE, size),
                          K_GET(ClothingAdapter, MATERIAL, material)}));

  ADAPTER_PROP_GET__FIELD(ClothingAdapter, size, Lisple::String);
  ADAPTER_PROP_GET__FIELD(ClothingAdapter, material, Lisple::String);

  // ===============================================================
  // Vector graphics example
  // ===============================================================

  // ===============================================================
  // Vector graphics example - adapters - RTValue & AST-based
  // ===============================================================

  SHKEY(KEY_X, "x");
  SHKEY(KEY_Y, "y");

  const Lisple::sptr_rtval X = Lisple::RTValue::keyword("x");
  const Lisple::sptr_rtval Y = Lisple::RTValue::keyword("y");

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

    return Lisple::RTValue::object(PointAdapter::make<Point>(
      std::get<Lisple::RTValue::Number>(Lisple::Dict::get_property(args[0], X)->value)
        .get_float(),
      std::get<Lisple::RTValue::Number>(Lisple::Dict::get_property(args[0], Y)->value)
        .get_float()));
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

  /** PointRotateFunction - pixils.point/rotate */
  FUNC_IMPL(PointRotateFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointRotateFunction::exec_rotate_point))));

  EXEC_BODY(PointRotateFunction, exec_rotate_point)
  {
    Lisple::sptr_sobject obj = std::get<Lisple::sptr_sobject>(args[0]->value);
    const Point& point = obj->as<PointAdapter>().get_object();
    const Point origin = {0, 0};
    float amount = std::get<Lisple::RTValue::Number>(args[1]->value).get_float();

    if (amount == 0.0) return args[0];

    float s = std::sin(amount);
    float c = std::cos(amount);

    float x = point.x - origin.x;
    float y = point.x - origin.y;

    float x_new = x * c - y * s;
    float y_new = x * s + y * c;

    return Lisple::RTValue::object(
      PointAdapter::make<Point>(x_new + origin.x, y_new + origin.y));
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

  /** PointAddFunction - pixils.point/plus */
  FUNC_IMPL(PointAddFunction,
            SIG((FN_ARGS((&POINT), (&POINT)), EXEC_DISPATCH(&PointAddFunction::exec_plus))));

  EXEC_BODY(PointAddFunction, exec_plus)
  {
    const Point& a =
      std::get<Lisple::sptr_sobject>(args[0]->value)->as<PointAdapter>().get_object();
    const Point& b =
      std::get<Lisple::sptr_sobject>(args[1]->value)->as<PointAdapter>().get_object();

    return Lisple::RTValue::object(PointAdapter::make<Point>(a.x + b.x, a.y + b.y));
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

  /** PointDivideFunction - pixils.point/div */
  FUNC_IMPL(PointDivideFunction,
            SIG((FN_ARGS((&POINT), (&Lisple::Type::NUMBER)),
                 EXEC_DISPATCH(&PointDivideFunction::exec_divide))));

  EXEC_BODY(PointDivideFunction, exec_divide)
  {
    const Point& a =
      std::get<Lisple::sptr_sobject>(args[0]->value)->as<PointAdapter>().get_object();
    float amount = std::get<Lisple::RTValue::Number>(args[1]->value).get_float();

    return Lisple::RTValue::object(PointAdapter::make<Point>(a.x / amount, a.y / amount));
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

  FUNC_IMPL(PointDistanceFunction,
            SIG((FN_ARGS((&POINT), (&POINT)),
                 EXEC_DISPATCH(&PointDistanceFunction::exec_distance))));

  EXEC_BODY(PointDistanceFunction, exec_distance)
  {
    const Point& a =
      std::get<Lisple::sptr_sobject>(args[0]->value)->as<PointAdapter>().get_object();
    const Point& b =
      std::get<Lisple::sptr_sobject>(args[1]->value)->as<PointAdapter>().get_object();

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

  PointASTBasedNamespace::PointASTBasedNamespace(const std::string& name)
    : Namespace(name)
  {
    objects.emplace("plus", std::make_shared<PointAddASTFunction>());
    objects.emplace("div", std::make_shared<PointDivideASTFunction>());
    objects.emplace("rotate", std::make_shared<PointRotateASTFunction>());
    objects.emplace("distance", std::make_shared<PointDistanceASTFunction>());
    objects.emplace("make-point", std::make_shared<MakePointASTFunction>());
  }

} // namespace Tests
