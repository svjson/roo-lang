#ifndef LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H
#define LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H

#include <lisple/exec.h>
#include <lisple/host/object.h>
#include <lisple/runtime/value.h>

#include "vehicle_impl.h"

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

  // ===============================================================
  // Vehicle namespace - functions
  // ===============================================================

  class VehicleNamespace : public Lisple::Namespace
  {
   public:
    VehicleNamespace();
  };

} // namespace LispleTest::Native

#endif /* LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H */
