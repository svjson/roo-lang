
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
  inline Lisple::HostTypeRef VEHICLE_MODEL_TYPE("vehicle-model",
                                                "vehicle/make-vehicle-model");
  inline Lisple::HostTypeRef VEHICLE_TYPE__NO_COERCE("vehicle");
  inline Lisple::HostTypeRef VEHICLE_MODEL_TYPE__NO_COERCE("vehicle-model");
  inline Lisple::HostTypeRef REGNUM_TYPE("regnum", "vehicle/make-reg-number");
  inline Lisple::HostTypeRef VECTOR_VEHICLE("vector<Vehicle>");
  inline Lisple::HostTypeRef VECTOR_VEHICLE_MODEL("vector<VehicleModel>");

  inline Lisple::SeqRef ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY, &VEHICLE_TYPE, "[vehicle]");
  inline Lisple::SeqRef ARRAY_OF_VEHICLE_MODEL(&Lisple::Type::ARRAY,
                                               &VEHICLE_MODEL_TYPE,
                                               "[vehicle]");
  inline Lisple::SeqRef ARRAY_OF_ARRAY_OF_VEHICLE(&Lisple::Type::ARRAY,
                                                  &ARRAY_OF_VEHICLE,
                                                  "[[vehicle]]");
  inline Lisple::SeqRef ARRAY_OF_ARRAY_OF_VEHICLE_MODEL(&Lisple::Type::ARRAY,
                                                        &ARRAY_OF_VEHICLE_MODEL,
                                                        "[[vehicle]]");

  // ===============================================================
  // Vehicle example - Native Collections Host Types
  // ===============================================================

  inline Lisple::HostTypeRef MAP_INT_TO_CONST_STRING("map<int, const string>");
  inline Lisple::HostTypeRef MAP_LONG_TO_VEHICLE_MODEL("map<long, vehicle-model>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_VEHICLE_MODEL("map<regnum, vehicle-model>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_VEHICLE_MODEL("map<regnum, vehicle-model>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_SHORT("map<regnum, short>");
  inline Lisple::HostTypeRef MAP_REGNUM_TO_CONST_INT("map<regnum, const int>");

  template <typename T> const Lisple::AdapterTraits& resolve_traits();

  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================
  HOST_ADAPTER(VehicleModelAdapter, VehicleModel, (model_name, seats), (seats));

  HOST_ADAPTER(RegNumberAdapter, RegNumber, (letters, numbers));

} // namespace LispleTest

DEFINE_LISPLE_TYPE(const LispleTest::VehicleModel, LispleTest::VEHICLE_MODEL_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::VehicleModel, LispleTest::VEHICLE_MODEL_TYPE);
DEFINE_LISPLE_TYPE(LispleTest::RegNumber, LispleTest::REGNUM_TYPE);

DEFINE_VECTOR_TYPE(LispleTest::VECTOR_VEHICLE_MODEL,
                   LispleTest::VehicleModel,
                   LispleTest::VEHICLE_MODEL_TYPE);

DEFINE_MAP_TYPE(LispleTest::MAP_INT_TO_CONST_STRING, int, const std::string);
DEFINE_MAP_TYPE(LispleTest::MAP_LONG_TO_VEHICLE_MODEL, long, LispleTest::VehicleModel);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_SHORT, LispleTest::RegNumber, short);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_CONST_INT, LispleTest::RegNumber, const int);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_VEHICLE_MODEL,
                LispleTest::RegNumber,
                LispleTest::VehicleModel);
DEFINE_MAP_TYPE(LispleTest::MAP_REGNUM_TO_CONST_VEHICLE_MODEL,
                LispleTest::RegNumber,
                const LispleTest::VehicleModel);

#endif /* LISPLETEST__HOST__VEHICLE_HOST_ADAPTERS_H */
