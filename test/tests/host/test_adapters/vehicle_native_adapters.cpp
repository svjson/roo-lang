
#include "vehicle_native_adapters.h"

#include "lisple/host/schema.h"

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

  /* VehicleAdapter */
  NATIVE_ADAPTER_IMPL(VehicleAdapter,
                      Vehicle,
                      &LispleTest::VEHICLE_TYPE,
                      (model),
                      ("reg-number", reg_number))

  NOBJ_PROP_GET_ADAPTER__METHOD(VehicleAdapter, model, VehicleModelAdapter);
  NOBJ_PROP_GET_SET_ADAPTER__METHOD(VehicleAdapter, reg_number, RegNumberAdapter);

  /* RegNumberAdapter */
  NATIVE_ADAPTER_IMPL(RegNumberAdapter,
                      RegNumber,
                      &LispleTest::REGNUM_TYPE,
                      (letters),
                      (numbers));

  NOBJ_PROP_GET__METHOD(RegNumberAdapter, letters);
  NOBJ_PROP_GET__METHOD(RegNumberAdapter, numbers);

  /* VehicleModelAdapter */
  NATIVE_ADAPTER_IMPL(VehicleModelAdapter,
                      VehicleModel,
                      &VEHICLE_MODEL_TYPE,
                      ("model-name", model_name),
                      (rw, "seats", seats));

  NOBJ_PROP_GET__METHOD(VehicleModelAdapter, model_name);
  NOBJ_PROP_GET__METHOD(VehicleModelAdapter, seats);
  NOBJ_PROP_SET__METHOD(VehicleModelAdapter, seats);

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  /* VehicleModel Make-function */
  FUNC_IMPL(VehicleModelMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&VehicleModelMakeFunction::exec_make))))

  EXEC_BODY(VehicleModelMakeFunction, exec_make)
  {
    const std::string model_name =
      Lisple::Dict::get_property(args.front(), MODEL_NAME)->str();

    int seats = Lisple::Dict::get_property(args.front(), SEATS)->i64();

    return VehicleModelAdapter::make_unique(model_name, seats);
  }

  /* Vehicle Make-function */
  FUNC_IMPL(VehicleMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&VehicleMakeFunction::exec_make))))

  EXEC_BODY(VehicleMakeFunction, exec_make)
  {
    static Lisple::MapSchema vehicle_schema(
      {{"model", &VEHICLE_MODEL_TYPE}, {"reg-number", &REGNUM_TYPE}});

    auto opts = vehicle_schema.bind(ctx, *args[0]);

    auto vmodel = opts.obj<VehicleModel>("model");

    return VehicleAdapter::make_unique(vmodel, opts.obj<RegNumber>("reg-number"));
  }

  /* RegNumber Make-function */
  FUNC_IMPL(RegNumberMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&RegNumberMakeFunction::exec_make))))

  EXEC_BODY(RegNumberMakeFunction, exec_make)
  {
    const std::string letters = Lisple::Dict::get_property(args.front(), LETTERS)->str();
    const std::string numbers = Lisple::Dict::get_property(args.front(), NUMBERS)->str();

    return RegNumberAdapter::make_unique(letters, numbers);
  }

  /** PrnVehicle - prn-vehicle */
  FUNC_IMPL(PrnVehicle,
            SIG((FN_ARGS((&VEHICLE_MODEL_TYPE)), EXEC_DISPATCH(&PrnVehicle::exec_prn))))

  EXEC_BODY(PrnVehicle, exec_prn)
  {
    VehicleModel& v = args.front()->adapter<VehicleModelAdapter>().get_object();
    ctx.eval("(prn \"The vehicle " + v.get_model_name() + " has " +
             std::to_string(v.get_seats()) + " seats\")");

    return args.front();
  }

  FUNC_IMPL(DoubleSizeVehicle,
            SIG((FN_ARGS((&LispleTest::VEHICLE_MODEL_TYPE__NO_COERCE)),
                 EXEC_DISPATCH(&DoubleSizeVehicle::exec_zoom))))

  EXEC_BODY(DoubleSizeVehicle, exec_zoom)
  {
    VehicleModel& v = args.front()->adapter<VehicleModelAdapter>().get_object();
    return VehicleModelAdapter::make_unique(v.get_model_name(), v.get_seats() * 2);
  }

  FUNC_IMPL(CountVehicleSeats,
            SIG((FN_ARGS((&LispleTest::ARRAY_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&CountVehicleSeats::exec_count))))

  EXEC_BODY(CountVehicleSeats, exec_count)
  {
    long count = 0;
    for (auto& element : args.front()->elements())
    {
      count += element->adapter<VehicleModelAdapter>().get_object().get_seats();
    }
    return Lisple::RTValue::number(count);
  }

  FUNC_IMPL(ArrayOfArrayTaker,
            SIG((FN_ARGS((&LispleTest::ARRAY_OF_ARRAY_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&ArrayOfArrayTaker::exec_accept))))

  EXEC_BODY(ArrayOfArrayTaker, exec_accept)
  {
    return args.front();
  }

  // ===============================================================
  // Vehicle example - namespace
  // ===============================================================
  VehicleNamespace::VehicleNamespace()
    : Lisple::Namespace("vehicle")
  {
    values.emplace("make-vehicle", VehicleMakeFunction::make());
    values.emplace("make-vehicle-model", VehicleModelMakeFunction::make());
    values.emplace("make-reg-number", RegNumberMakeFunction::make());
    values.emplace("prn-vehicle-model", PrnVehicle::make());
    values.emplace("double-size-vehicle", DoubleSizeVehicle::make());
    values.emplace("count-seats", CountVehicleSeats::make());
    values.emplace("a-of-a-taker", ArrayOfArrayTaker::make());
  }

} // namespace LispleTest::Native
