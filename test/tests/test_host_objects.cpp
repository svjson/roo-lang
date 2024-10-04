
#include "test_host_objects.h"

#include <lisple/context.h>
#include <lisple/impl.h>

#include <iostream>

namespace Tests
{
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
    return this->model_name == other.model_name &&
      this->seats == other.seats;
  }

  SHKEY(MODEL_NAME, "model-name")
  SHKEY(LETTERS, "letters")
  SHKEY(NUMBERS, "numbers")
  SHKEY(SEATS, "seats")

  /* RegNumberAdapter */
  HOST_ADAPTER_IMPL("regnum", RegNumberAdapter, RegNumber, REG_NUMBER, (
                      { K_GET(RegNumberAdapter, LETTERS, letters),
                        K_GET(RegNumberAdapter, NUMBERS, numbers) }));

  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, letters, Lisple::String, get_letters);
  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, numbers, Lisple::String, get_numbers);

  /* VehicleAdapter */
  HOST_ADAPTER_IMPL("vehicle", VehicleAdapter, Vehicle, VEHICLE, (
                      { K_GET(VehicleAdapter, MODEL_NAME, model_name),
                        K_GET(VehicleAdapter, SEATS, seats) }));

  ADAPTER_PROP_GET__METHOD(VehicleAdapter, model_name, Lisple::String, get_model_name);
  ADAPTER_PROP_GET__METHOD(VehicleAdapter, seats, Lisple::Number, get_seats);

  /* Vehicle Make-function */
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

  FUNC_IMPL(ArrayOfArrayTaker, SIG((FN_ARGS((&ARRAY_OF_ARRAY_OF_VEHICLE)),
                                    EXEC_DISPATCH(&ArrayOfArrayTaker::accept))))

  FUNC_BODY(ArrayOfArrayTaker, accept)
  {
    return args.front();
  }

}
