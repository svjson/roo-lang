
#include "vehicle_host_adapters.h"

#include "vehicle_native_adapters.h"

namespace LispleTest
{

  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================

  SHKEY(MODEL_NAME, "model-name")
  SHKEY(LETTERS, "letters")
  SHKEY(NUMBERS, "numbers")
  SHKEY(SEATS, "seats")

  /* RegNumberAdapter */
  HOST_ADAPTER_IMPL(RegNumberAdapter,
                    RegNumber,
                    &REGNUM_TYPE,
                    ({K_GET(RegNumberAdapter, LETTERS, letters),
                      K_GET(RegNumberAdapter, NUMBERS, numbers)}));

  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, letters, Lisple::String, get_letters);
  ADAPTER_PROP_GET__METHOD(RegNumberAdapter, numbers, Lisple::String, get_numbers);

  /* VehicleAdapter */
  HOST_ADAPTER_IMPL(VehicleModelAdapter,
                    VehicleModel,
                    &VEHICLE_MODEL_TYPE,
                    ({K_GET(VehicleModelAdapter, MODEL_NAME, model_name),
                      K_GET(VehicleModelAdapter, SEATS, seats)}));

  Lisple ::sptr_sobject VehicleModelAdapter ::get_model_name() const
  {
    return std ::make_shared<Lisple ::String>(get_self_object().get_model_name());
  };
  ADAPTER_PROP_GET__METHOD(VehicleModelAdapter, seats, Lisple::Number, get_seats);
} // namespace LispleTest
