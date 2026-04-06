
#include "vehicle_native_adapters.h"

#include <lisple/host/accessor.h>
#include <lisple/runtime/dict.h>

#include "vehicle_host_adapters.h"

namespace LispleTest::Native
{
  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================

  const Lisple::sptr_rtval MODEL_NAME = Lisple::RTValue::keyword("model-name");
  const Lisple::sptr_rtval LETTERS = Lisple::RTValue::keyword("letters");
  const Lisple::sptr_rtval NUMBERS = Lisple::RTValue::keyword("numbers");
  const Lisple::sptr_rtval SEATS = Lisple::RTValue::keyword("seats");

  /* RegNumberAdapter */
  NATIVE_ADAPTER_IMPL(RegNumberAdapter,
                      RegNumber,
                      &LispleTest::REGNUM_TYPE,
                      (letters),
                      (numbers));

  NOBJ_PROP_GET__METHOD(RegNumberAdapter, letters);
  NOBJ_PROP_GET__METHOD(RegNumberAdapter, numbers);

  /* VehicleAdapter */
  NATIVE_ADAPTER_IMPL(VehicleAdapter,
                      Vehicle,
                      &VEHICLE_TYPE,
                      ("model-name", model_name),
                      ("seats", seats));

  NOBJ_PROP_GET__METHOD(VehicleAdapter, model_name);
  NOBJ_PROP_GET__METHOD(VehicleAdapter, seats);
  NOBJ_PROP_SET__METHOD(VehicleAdapter, seats);

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  /* Vehicle Make-function */
  FUNC_IMPL(VehicleMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&VehicleMakeFunction::exec_make))))

  EXEC_BODY(VehicleMakeFunction, exec_make)
  {
    const std::string model_name =
      Lisple::Dict::get_property(args.front(), MODEL_NAME)->str();

    int seats = Lisple::Dict::get_property(args.front(), SEATS)->i64();

    return VehicleAdapter::make_unique(model_name, seats);
  }

  /** PrnVehicle - prn-vehicle */
  FUNC_IMPL(PrnVehicle,
            SIG((FN_ARGS((&VEHICLE_TYPE)), EXEC_DISPATCH(&PrnVehicle::exec_prn))))

  EXEC_BODY(PrnVehicle, exec_prn)
  {
    Vehicle& v = args.front()->adapter<VehicleAdapter>().get_object();
    ctx.eval("(prn \"The vehicle " + v.get_model_name() + " has " +
             std::to_string(v.get_seats()) + " seats\")");

    return args.front();
  }

} // namespace LispleTest::Native
