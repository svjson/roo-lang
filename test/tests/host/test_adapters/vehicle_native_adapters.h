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

  NATIVE_ADAPTER(VehicleAdapter, Vehicle, (model_name, seats), (seats));

  NATIVE_ADAPTER(RegNumberAdapter, RegNumber, (letters, numbers));

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  FUNC(VehicleMakeFunction, make);

  FUNC(PrnVehicle, prn);

} // namespace LispleTest::Native

#endif /* LISPLETEST__HOST__VEHICLE_NATIVE_ADAPTERS_H */
