#ifndef ROOTEST__HOST__VEHICLE_NATIVE_ADAPTERS_H
#define ROOTEST__HOST__VEHICLE_NATIVE_ADAPTERS_H

#include "vehicle_impl.h"
#include <roo/adapter.h>
#include <roo/exec.h>
#include <roo/host/object.h>
#include <roo/runtime/value.h>

namespace RooTest
{
  inline Roo::HostTypeRef VEHICLE_TYPE("vehicle", "vehicle/make-vehicle");
  inline Roo::HostTypeRef VEHICLE_MODEL_TYPE("vehicle-model", "vehicle/make-vehicle-model");
  inline Roo::HostTypeRef VEHICLE_TYPE__NO_COERCE("vehicle");
  inline Roo::HostTypeRef VEHICLE_MODEL_TYPE__NO_COERCE("vehicle-model");
  inline Roo::HostTypeRef REGNUM_TYPE("regnum", "vehicle/make-reg-number");

  inline Roo::SeqRef VECTOR_OF_VEHICLE(&Roo::Type::VECTOR, &VEHICLE_TYPE, "[vehicle]");
  inline Roo::SeqRef VECTOR_OF_VEHICLE_MODEL(&Roo::Type::VECTOR,
                                             &VEHICLE_MODEL_TYPE,
                                             "[vehicle]");
  inline Roo::SeqRef VECTOR_OF_VECTOR_OF_VEHICLE(&Roo::Type::VECTOR,
                                                 &VECTOR_OF_VEHICLE,
                                                 "[[vehicle]]");
  inline Roo::SeqRef VECTOR_OF_VECTOR_OF_VEHICLE_MODEL(&Roo::Type::VECTOR,
                                                       &VECTOR_OF_VEHICLE_MODEL,
                                                       "[[vehicle]]");
} // namespace RooTest

namespace RooTest::Native
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

  class VehicleNamespace : public Roo::Namespace
  {
   public:
    VehicleNamespace();
  };

} // namespace RooTest::Native

DEFINE_ROO_TYPE(const RooTest::VehicleModel, RooTest::VEHICLE_MODEL_TYPE);
DEFINE_ROO_TYPE(RooTest::VehicleModel, RooTest::VEHICLE_MODEL_TYPE);
DEFINE_ROO_TYPE(RooTest::RegNumber, RooTest::REGNUM_TYPE);

#endif /* ROOTEST__HOST__VEHICLE_NATIVE_ADAPTERS_H */
