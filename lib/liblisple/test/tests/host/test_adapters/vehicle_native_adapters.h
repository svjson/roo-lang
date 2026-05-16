#ifndef LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H
#define LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H

#include <lisple/adapter.h>
#include <lisple/exec.h>
#include <lisple/host/object.h>
#include <lisple/runtime/value.h>

#include "vehicle_impl.h"

namespace LispleTest
{
  inline Lisple::HostTypeRef VEHICLE_TYPE("vehicle", "vehicle/make-vehicle");
  inline Lisple::HostTypeRef VEHICLE_MODEL_TYPE("vehicle-model",
                                                "vehicle/make-vehicle-model");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE("vehicle");
  inline Lisple::HostTypeRef VEHICLE_MODEL_TYPE__NO_COERCE("vehicle-model");
  inline Lisple::HostTypeRef REGNUM_TYPE("regnum", "vehicle/make-reg-number");

  inline Lisple::SeqRef VECTOR_OF_VEHICLE(&Lisple::Type::VECTOR, &VEHICLE_TYPE, "[vehicle]");
  inline Lisple::SeqRef VECTOR_OF_VEHICLE_MODEL(&Lisple::Type::VECTOR,
                                               &VEHICLE_MODEL_TYPE,
                                               "[vehicle]");
  inline Lisple::SeqRef VECTOR_OF_VECTOR_OF_VEHICLE(&Lisple::Type::VECTOR,
                                                  &VECTOR_OF_VEHICLE,
                                                  "[[vehicle]]");
  inline Lisple::SeqRef VECTOR_OF_VECTOR_OF_VEHICLE_MODEL(&Lisple::Type::VECTOR,
                                                        &VECTOR_OF_VEHICLE_MODEL,
                                                        "[[vehicle]]");
} // namespace LispleTest

namespace LispleTest::Native
{
  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================

  NATIVE_ADAPTER(VehicleAdapter, Vehicle, (model, reg_number), (reg_number));
  NATIVE_ADAPTER(VehicleModelAdapter, VehicleModel, (model_name, seats), (seats));
  NATIVE_ADAPTER(RegNumberAdapter, RegNumber, (letters, numbers));

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  FUNC(VehicleMakeFunction, make);
  FUNC(VehicleModelMakeFunction, make);
  FUNC(RegNumberMakeFunction, make);

  FUNC(PrnVehicle, prn);
  FUNC(DoubleSizeVehicle, zoom);
  FUNC(CountVehicleSeats, count);
  FUNC(VectorOfVectorTaker, accept);

  // ===============================================================
  // Vehicle namespace - functions
  // ===============================================================

  class VehicleNamespace : public Lisple::Namespace
  {
   public:
    VehicleNamespace();
  };

} // namespace LispleTest::Native

DEFINE_LISPLE_TYPE(const LispleTest::VehicleModel, LispleTest::VEHICLE_MODEL_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::VehicleModel, LispleTest::VEHICLE_MODEL_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::RegNumber, LispleTest::REGNUM_TYPE);

#endif /* LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H */
