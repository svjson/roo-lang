
#ifndef __TEST_HOST_OBJECTS_H_
#define __TEST_HOST_OBJECTS_H_

#include <string>

#include <lisple/host.h>
#include <lisple/exec.h>

namespace Lisple
{
  enum class HostObjectType : short
  {
    REG_NUMBER,
    VEHICLE,
    STD_MAP_INT_STRING,
    STD_MAP_REGNUM_VEHICLE
  };
}

namespace Tests
{
  inline Lisple::HostTypeRef VEHICLE_TYPE(Lisple::HostObjectType::VEHICLE, "vehicle", "vehicle/make-vehicle");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE(Lisple::HostObjectType::VEHICLE, "vehicle");
  inline Lisple::HostTypeRef REGNUM_TYPE(Lisple::HostObjectType::REG_NUMBER, "regnum");

  inline Lisple::SeqRef ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &VEHICLE_TYPE, "[vehicle]");
  inline Lisple::SeqRef ARRAY_OF_ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &ARRAY_OF_VEHICLE, "[[vehicle]]");

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

  HOST_ADAPTER(VehicleAdapter, Vehicle, (model_name, seats), (seats));

  HOST_ADAPTER(RegNumberAdapter, RegNumber, (letters, numbers));

  FUNC_DECL(VehicleMakeFunction, make);

  FUNC_DECL(PrnVehicle, prn);
  FUNC_DECL(DoubleSizeVehicle, zoom);
  FUNC_DECL(CountVehicleSeats, count);

  FUNC_DECL(ArrayOfArrayTaker, accept);

}


#endif
