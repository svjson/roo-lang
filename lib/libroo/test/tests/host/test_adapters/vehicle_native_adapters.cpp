
#include "vehicle_native_adapters.h"

#include "roo/host/schema.h"

#include <roo/host/accessor.h>
#include <roo/runtime/dict.h>

namespace RooTest::Native
{
  // ===============================================================
  // Vehicle example - adapters
  // ===============================================================

  const Roo::sptr_val MODEL_NAME = Roo::Value::keyword("model-name");
  const Roo::sptr_val LETTERS = Roo::Value::keyword("letters");
  const Roo::sptr_val NUMBERS = Roo::Value::keyword("numbers");
  const Roo::sptr_val SEATS = Roo::Value::keyword("seats");

  /* VehicleAdapter */
  NATIVE_ADAPTER_IMPL(VehicleAdapter,
                      Vehicle,
                      &RooTest::VEHICLE_TYPE,
                      (model),
                      ("reg-number", reg_number))

  ADAPTER_PROP_GET(VehicleAdapter, METHOD(model), ADAPTER(VehicleModelAdapter));
  ADAPTER_PROP_GET_SET(VehicleAdapter, METHOD(reg_number), ADAPTER(RegNumberAdapter));

  /* RegNumberAdapter */
  NATIVE_ADAPTER_IMPL(RegNumberAdapter,
                      RegNumber,
                      &RooTest::REGNUM_TYPE,
                      (letters),
                      (numbers));

  ADAPTER_PROP_GET(RegNumberAdapter, METHOD(letters));
  ADAPTER_PROP_GET(RegNumberAdapter, METHOD(numbers));

  /* VehicleModelAdapter */
  NATIVE_ADAPTER_IMPL(VehicleModelAdapter,
                      VehicleModel,
                      &VEHICLE_MODEL_TYPE,
                      ("model-name", model_name),
                      (rw, "seats", seats));

  ADAPTER_PROP_GET(VehicleModelAdapter, METHOD(model_name));
  ADAPTER_PROP_GET_SET(VehicleModelAdapter, METHOD(seats));

  // ===============================================================
  // Vehicle example - functions
  // ===============================================================

  /* VehicleModel Make-function */
  FUNC_IMPL(VehicleModelMakeFunction,
            SIG((FN_ARGS((&Roo::Type::MAP)),
                 EXEC_DISPATCH(&VehicleModelMakeFunction::exec_make))))

  EXEC_BODY(VehicleModelMakeFunction, exec_make)
  {
    const std::string model_name = Roo::Dict::get_property(args.front(), MODEL_NAME)->str();

    int seats = Roo::Dict::get_property(args.front(), SEATS)->i64();

    return VehicleModelAdapter::make_unique(model_name, seats);
  }

  /* Vehicle Make-function */
  FUNC_IMPL(VehicleMakeFunction,
            SIG((FN_ARGS((&Roo::Type::MAP)),
                 EXEC_DISPATCH(&VehicleMakeFunction::exec_make))))

  EXEC_BODY(VehicleMakeFunction, exec_make)
  {
    static Roo::MapSchema vehicle_schema(
      {{"model", &VEHICLE_MODEL_TYPE}, {"reg-number", &REGNUM_TYPE}});

    auto opts = vehicle_schema.bind(ctx, *args[0]);

    auto vmodel = opts.obj<VehicleModel>("model");

    return VehicleAdapter::make_unique(vmodel, opts.obj<RegNumber>("reg-number"));
  }

  /* RegNumber Make-function */
  FUNC_IMPL(RegNumberMakeFunction,
            SIG((FN_ARGS((&Roo::Type::MAP)),
                 EXEC_DISPATCH(&RegNumberMakeFunction::exec_make))))

  EXEC_BODY(RegNumberMakeFunction, exec_make)
  {
    const std::string letters = Roo::Dict::get_property(args.front(), LETTERS)->str();
    const std::string numbers = Roo::Dict::get_property(args.front(), NUMBERS)->str();

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
            SIG((FN_ARGS((&RooTest::VEHICLE_MODEL_TYPE__NO_COERCE)),
                 EXEC_DISPATCH(&DoubleSizeVehicle::exec_zoom))))

  EXEC_BODY(DoubleSizeVehicle, exec_zoom)
  {
    VehicleModel& v = args.front()->adapter<VehicleModelAdapter>().get_object();
    return VehicleModelAdapter::make_unique(v.get_model_name(), v.get_seats() * 2);
  }

  FUNC_IMPL(CountVehicleSeats,
            SIG((FN_ARGS((&RooTest::VECTOR_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&CountVehicleSeats::exec_count))))

  EXEC_BODY(CountVehicleSeats, exec_count)
  {
    long count = 0;
    for (auto& element : args.front()->elements())
    {
      count += element->adapter<VehicleModelAdapter>().get_object().get_seats();
    }
    return Roo::Value::number(count);
  }

  FUNC_IMPL(VectorOfVectorTaker,
            SIG((FN_ARGS((&RooTest::VECTOR_OF_VECTOR_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&VectorOfVectorTaker::exec_accept))))

  EXEC_BODY(VectorOfVectorTaker, exec_accept)
  {
    return args.front();
  }

  // ===============================================================
  // Vehicle example - namespace
  // ===============================================================
  VehicleNamespace::VehicleNamespace()
    : Roo::Namespace("vehicle")
  {
    values.emplace("make-vehicle", VehicleMakeFunction::make());
    values.emplace("make-vehicle-model", VehicleModelMakeFunction::make());
    values.emplace("make-reg-number", RegNumberMakeFunction::make());
    values.emplace("prn-vehicle-model", PrnVehicle::make());
    values.emplace("double-size-vehicle", DoubleSizeVehicle::make());
    values.emplace("count-seats", CountVehicleSeats::make());
    values.emplace("v-of-v-taker", VectorOfVectorTaker::make());
  }

} // namespace RooTest::Native
