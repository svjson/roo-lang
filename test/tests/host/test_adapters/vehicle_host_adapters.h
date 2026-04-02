
#ifndef LISPLETEST__HOST__VEHICLE_HOST_ADAPTERS_H
#define LISPLETEST__HOST__VEHICLE_HOST_ADAPTERS_H

#include <lisple/adapter.h>
#include <lisple/exec.h>
#include <lisple/host.h>

#include "vehicle_impl.h"

namespace LispleTest
{
  // ===============================================================
  // Vehicle example - Host Types
  // ===============================================================

  inline Lisple::HostTypeRef VEHICLE_TYPE("vehicle", "vehicle/make-vehicle");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE("vehicle");
  inline Lisple::HostTypeRef REGNUM_TYPE("regnum");
  inline Lisple::HostTypeRef VECTOR_VEHICLE("vector<Vehicle>");

  inline Lisple::SeqRef ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &VEHICLE_TYPE, "[vehicle]");
  inline Lisple::SeqRef ARRAY_OF_ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY,
                                                  &ARRAY_OF_VEHICLE,
                                                  "[[vehicle]]");

  // ===============================================================
  // Vehicle example - Native Collections Host Types
  // ===============================================================

  inline Lisple::HostTypeRef MAP_INT_TO_CONST_STRING("map<int, const string>");
  inline Lisple::HostTypeRef MAP_LONG_TO_VEHICLE("map<long, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_VEHICLE("map<regnum, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_VEHICLE("map<regnum, vehicle>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_SHORT("map<regnum, short>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_INT("map<regnum, const int>");

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

} // namespace LispleTest

DEFINE_LISPLE_TYPE(const LispleTest::Vehicle, LispleTest::VEHICLE_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::Vehicle, LispleTest::VEHICLE_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::RegNumber, LispleTest::REGNUM_TYPE);

DEFINE_VECTOR_TYPE(LispleTest::VECTOR_VEHICLE,
                   LispleTest::Vehicle,
                   LispleTest::VEHICLE_TYPE);

DEFINE_MAP_TYPE(LispleTest::MAP_INT_TO_CONST_STRING, int, const std::string);
DEFINE_MAP_TYPE(LispleTest::MAP_LONG_TO_VEHICLE, long, LispleTest::Vehicle);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_SHORT, LispleTest::RegNumber, short);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_CONST_INT, LispleTest::RegNumber, const int);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_CONST_VEHICLE,
                LispleTest::RegNumber,
                LispleTest::Vehicle);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_CONST_VEHICLE,
                LispleTest::RegNumber,
                const LispleTest::Vehicle);

#endif /* LISPLETEST__HOST__VEHICLE_HOST_ADAPTERS_H */
