
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

  // ===============================================================
  // VehicleModel example - functions
  // ===============================================================

  /* VehicleModel Make-function */
  FUNC_IMPL(VehicleModelMakeFunction,
            SIG((FN_ARGS((&Lisple::Type::MAP)),
                 EXEC_DISPATCH(&VehicleModelMakeFunction::make))))

  FUNC_BODY(VehicleModelMakeFunction, make)
  {
    const std::string model_name =
      args.front()->get_sptr_property(*MODEL_NAME)->as<Lisple::String>().value;
    int seats = args.front()->get_sptr_property(*SEATS)->as<Lisple::Number>().int_value();

    return VehicleModelAdapter::make<VehicleModel>(model_name, seats);
  }

  FUNC_IMPL(PrnVehicle,
            SIG((FN_ARGS((&VEHICLE_MODEL_TYPE)), EXEC_DISPATCH(&PrnVehicle::prn))))

  /** PrnVehicle - prn */
  FUNC_BODY(PrnVehicle, prn)
  {
    VehicleModel* v = nullptr;
    if (auto* wrapper = dynamic_cast<Lisple::RuntimeValueWrapper*>(args.front().get()))
    {
      if (wrapper->val->type == Lisple::RTValue::Type::NATIVE_OBJECT)
      {
        v = &wrapper->val->adapter<LispleTest::Native::VehicleModelAdapter>().get_object();
      }
    }

    if (v == nullptr)
    {
      v = &args.front()->as<VehicleModelAdapter>().get_object();
    }
    ctx.eval("(prn \"The vehicle " + v->get_model_name() + " has " +
             std::to_string(v->get_seats()) + " seats\")");

    return args.front();
  }

  FUNC_IMPL(DoubleSizeVehicle,
            SIG((FN_ARGS((&VEHICLE_MODEL_TYPE__NO_COERCE)),
                 EXEC_DISPATCH(&DoubleSizeVehicle::zoom))))

  FUNC_BODY(DoubleSizeVehicle, zoom)
  {
    VehicleModel& v = args.front()->as<VehicleModelAdapter>().get_object();

    return VehicleModelAdapter::make<VehicleModel>(v.get_model_name(), v.get_seats() * 2);
  }

  FUNC_IMPL(CountVehicleSeats,
            SIG((FN_ARGS((&ARRAY_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&CountVehicleSeats::count))))

  FUNC_BODY(CountVehicleSeats, count)
  {
    int count = 0;
    for (Lisple::sptr_sobject& element : args.front()->get_children())
    {
      count += element->as<VehicleModelAdapter>().get_object().get_seats();
    }
    return Lisple::Number::make(count);
  }

  FUNC_IMPL(ArrayOfArrayTaker,
            SIG((FN_ARGS((&ARRAY_OF_ARRAY_OF_VEHICLE_MODEL)),
                 EXEC_DISPATCH(&ArrayOfArrayTaker::accept))))

  FUNC_BODY(ArrayOfArrayTaker, accept)
  {
    return args.front();
  }

} // namespace LispleTest
