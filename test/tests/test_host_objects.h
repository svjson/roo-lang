
#ifndef __TEST_HOST_OBJECTS_H_
#define __TEST_HOST_OBJECTS_H_

#include <lisple/adapter.h>
#include <lisple/exec.h>
#include <lisple/host.h>
#include <lisple/namespace.h>
#include <lisple/type.h>
#include <string>

namespace Tests
{
  inline Lisple::HostTypeRef MAP_INT_TO_CONST_STRING("map<int, const string>");
  inline Lisple::HostTypeRef MAP_LONG_TO_VEHICLE("map<long, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_VEHICLE("map<regnum, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_VEHICLE("map<regnum, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_SHORT("map<regnum, short>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_INT("map<regnum, const int>");

  // ===============================================================
  // Vehicle example - with nested type RegNumber
  // ===============================================================

  inline Lisple::HostTypeRef VEHICLE_TYPE("vehicle", "vehicle/make-vehicle");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE("vehicle");
  inline Lisple::HostTypeRef REGNUM_TYPE("regnum");
  inline Lisple::HostTypeRef VECTOR_VEHICLE("vector<Vehicle>");

  inline Lisple::SeqRef ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &VEHICLE_TYPE, "[vehicle]");
  inline Lisple::SeqRef ARRAY_OF_ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY,
                                                  &ARRAY_OF_VEHICLE,
                                                  "[[vehicle]]");

  class RegNumber
  {
    const std::string letters;
    const std::string numbers;

   public:
    RegNumber(const std::string& letters, const std::string& numbers);

    const std::string& get_letters() const;
    const std::string& get_numbers() const;

    bool operator<(const RegNumber& other) const;
  };

  class Vehicle
  {
    const std::string model_name;
    int seats;

   public:
    Vehicle(const std::string& model_name, int seats);

    const std::string& get_model_name() const;
    int get_seats() const;

    void set_seats(int seats);

    bool operator==(const Vehicle& other) const;
  };

  template <typename T> const Lisple::AdapterTraits& resolve_traits();

  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================
  HOST_ADAPTER(VehicleAdapter, Vehicle, (model_name, seats), (seats));

  HOST_ADAPTER(RegNumberAdapter, RegNumber, (letters, numbers));

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================
  FUNC_DECL(VehicleMakeFunction, make);

  FUNC_DECL(PrnVehicle, prn);
  FUNC_DECL(DoubleSizeVehicle, zoom);
  FUNC_DECL(CountVehicleSeats, count);

  FUNC_DECL(ArrayOfArrayTaker, accept);

  // ===============================================================
  // Product example - With sub types
  // ===============================================================

  inline Lisple::HostTypeRef PRODUCT("Product");
  inline Lisple::HostTypeRef BOOK("Book", &PRODUCT);
  inline Lisple::HostTypeRef CLOTHING("Clothing", &PRODUCT);

  class Product
  {
   public:
    Product(const std::string& name, float price, int sku);
    virtual ~Product() = default;

    const std::string& get_name();
    float get_price();
    int get_sku();

    void set_price(float price);
    void set_sku(int sku);

   protected:
    const std::string name;
    float price;
    int sku;
  };

  class Book : public Product
  {
   public:
    Book(const std::string& name,
         float price,
         int sku,
         const std::string& author,
         const std::string& isbn);

    const std::string& get_author();
    const std::string& get_isbn();

   protected:
    const std::string author;
    const std::string isbn;
  };

  class Clothing : public Product
  {

   public:
    Clothing(const std::string& name,
             float price,
             int sku,
             const std::string& size,
             const std::string& material);

    const std::string size;
    const std::string material;
  };

  // ===============================================================
  // Product example - adapters
  // ===============================================================

  HOST_ADAPTER(ProductAdapter, Product, (name, price, sku), (price, sku));

  HOST_SUB_ADAPTER(BookAdapter, Book, ProductAdapter, Product, (author, isbn));

  HOST_SUB_ADAPTER(ClothingAdapter, Clothing, ProductAdapter, Product, (size, material));

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

  // ===============================================================
  // Vector graphics example - adapters - RTValue & AST-based
  // ===============================================================

  inline const Lisple::HostTypeRef POINT("point", "pixils.point/make-point");

  HOST_ADAPTER(PointAdapter, Point, (x, y));

  FUNC(MakePointFunction, make_point);

  FUNC(PointRotateFunction, rotate_point);

  FUNC(PointAddFunction, plus);

  FUNC(PointDivideFunction, divide);

  FUNC(PointDistanceFunction, distance);

  FUNC_DECL(MakePointASTFunction, make_point);

  FUNC_DECL(PointRotateASTFunction, rotate_point);

  FUNC_DECL(PointAddASTFunction, plus);

  FUNC_DECL(PointDivideASTFunction, divide);

  FUNC_DECL(PointDistanceASTFunction, distance);

  class PointNamespace : public Lisple::Namespace
  {
   public:
    PointNamespace(const std::string& name);
  };

  class PointASTBasedNamespace : public Lisple::Namespace
  {
   public:
    PointASTBasedNamespace(const std::string& name);
  };

} // namespace Tests

DEFINE_LISPLE_TYPE(const Tests::Vehicle, Tests::VEHICLE_TYPE);
DEFINE_LISPLE_TYPE(Tests::Vehicle, Tests::VEHICLE_TYPE);
DEFINE_LISPLE_TYPE(Tests::RegNumber, Tests::REGNUM_TYPE);

DEFINE_VECTOR_TYPE(Tests::VECTOR_VEHICLE, Tests::Vehicle, Tests::VEHICLE_TYPE);

DEFINE_MAP_TYPE(Tests::MAP_INT_TO_CONST_STRING, int, const std::string);
DEFINE_MAP_TYPE(Tests::MAP_LONG_TO_VEHICLE, long, Tests::Vehicle);
DEFINE_MAP_TYPE(Tests::MAP_REGNUM_TO_SHORT, Tests::RegNumber, short);
DEFINE_MAP_TYPE(Tests::MAP_REGNUM_TO_CONST_INT, Tests::RegNumber, const int);
DEFINE_MAP_TYPE(Tests::MAP_REGNUM_TO_CONST_VEHICLE, Tests::RegNumber, Tests::Vehicle);
DEFINE_MAP_TYPE(Tests::MAP_REGNUM_TO_CONST_VEHICLE, Tests::RegNumber, const Tests::Vehicle);

#endif
