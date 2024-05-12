
#include "test_host_objects.h"

#include <lisple/context.h>
#include <lisple/impl.h>

#include <iostream>

namespace Tests
{
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

  SHKEY(MODEL_NAME, "model-name");
  SHKEY(SEATS, "seats");

  HOST_ADAPTER_IMPL("vehicle", VehicleAdapter, Vehicle, VEHICLE, (
                      { K_GET(VehicleAdapter, MODEL_NAME, model_name),
                        K_GET(VehicleAdapter, SEATS, seats)}))

  ADAPTER_PROP_GET__METHOD(VehicleAdapter, model_name, Lisple::String, get_model_name);
  ADAPTER_PROP_GET__METHOD(VehicleAdapter, seats, Lisple::Number, get_seats);

  FUNC_IMPL(VehicleMakeFunction, SIG((FN_ARGS((&Lisple::Type::MAP)),
                                      EXEC_DISPATCH(&VehicleMakeFunction::make))))

  FUNC_BODY(VehicleMakeFunction, make)
  {
    const std::string model_name = args.front()->as<Lisple::Map>().get_property(*MODEL_NAME).as<Lisple::String>().value;
    int seats = args.front()->as<Lisple::Map>().get_property(*SEATS).as<Lisple::Number>().int_value();

    return VehicleAdapter::make<Vehicle>(model_name, seats);
  }

  FUNC_IMPL(PrnVehicle, SIG((FN_ARGS((&VEHICLE_TYPE)),
                             EXEC_DISPATCH(&PrnVehicle::prn))))

  FUNC_BODY(PrnVehicle, prn)
  {
    Vehicle& v = args.front()->as<VehicleAdapter>().get_object();
    ctx.eval("(prn \"The vehicle " +
             v.get_model_name() +
             " has " +
             std::to_string(v.get_seats()) +
             " seats\")");

    return args.front();
  }

  FUNC_IMPL(DoubleSizeVehicle, SIG((FN_ARGS((&VEHICLE_TYPE__NO_COERCE)),
                                    EXEC_DISPATCH(&DoubleSizeVehicle::zoom))))

  FUNC_BODY(DoubleSizeVehicle, zoom)
  {
    Vehicle& v = args.front()->as<VehicleAdapter>().get_object();

    return VehicleAdapter::make<Vehicle>(v.get_model_name(), v.get_seats()*2);
  }

  FUNC_IMPL(CountVehicleSeats, SIG((FN_ARGS((&ARRAY_OF_VEHICLE)),
                                    EXEC_DISPATCH(&CountVehicleSeats::count))))

  FUNC_BODY(CountVehicleSeats, count)
  {
    int count = 0;
    for (Lisple::sptr_sobject& element : args.front()->get_children())
    {
      count += element->as<VehicleAdapter>().get_object().get_seats();
    }
    return Lisple::Number::make(count);
  }

}
