
#ifndef __TEST_HOST_OBJECTS_H_
#define __TEST_HOST_OBJECTS_H_

#include <string>

#include <lisple/host.h>
#include <lisple/exec.h>

namespace Lisple
{
  enum class HostObjectType : short
  {
    VEHICLE
  };
}

namespace Tests
{
  inline Lisple::HostTypeRef VEHICLE_TYPE(Lisple::HostObjectType::VEHICLE, "vehicle", "vehicle/make-vehicle");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE(Lisple::HostObjectType::VEHICLE, "vehicle");
  
  inline Lisple::SeqRef ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &VEHICLE_TYPE, "[vehicle]");

  class Vehicle
  {
    const std::string model_name;
    int seats;

   public:
    Vehicle(const std::string& model_name, int seats);

    const std::string& get_model_name() const;
    int get_seats() const;

    void set_seats(int seats);
  };

  HOST_ADAPTER(VehicleAdapter, Vehicle, (model_name, seats), (seats));

  FUNC_DECL(VehicleMakeFunction, make);

  FUNC_DECL(PrnVehicle, prn);
  FUNC_DECL(DoubleSizeVehicle, zoom);
  FUNC_DECL(CountVehicleSeats, count);
}


#endif
