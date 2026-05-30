

#include <functional>
#include <memory>

#include "gmock/gmock.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include "roo/runtime/node.h"
#include "roo/runtime/value.h"
#include "runtime_fixture.h"
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/lang.h>
#include <roo/lang/base.h>
#include <roo/lang/loop.h>
#include <roo/lang/operator.h>
#include <roo/lang/seq_func.h>
#include <roo/type.h>

using Signature = RooTest::RuntimeTestFixture;
using namespace ::testing;

namespace SignatureTest
{
  Roo::sptr_val STRING = Roo::Value::string("str");
  Roo::sptr_val NUMBER = Roo::Value::number(5);
  Roo::sptr_val ARRAY = Roo::Value::vector({});
  Roo::sptr_val FUNCTION = Roo::MapFunction::make();

  Roo::uptr_exec_node NODE_STRING = std::make_unique<Roo::ExecNode>(STRING);
  Roo::uptr_exec_node NODE_NUMBER = std::make_unique<Roo::ExecNode>(NUMBER);
  Roo::uptr_exec_node NODE_ARRAY = std::make_unique<Roo::ExecNode>(ARRAY);
  Roo::uptr_exec_node NODE_FUNCTION = std::make_unique<Roo::ExecNode>(FUNCTION);

} // namespace SignatureTest

using SignatureTest::ARRAY;
using SignatureTest::FUNCTION;
using SignatureTest::NUMBER;
using SignatureTest::STRING;

using SignatureTest::NODE_ARRAY;
using SignatureTest::NODE_FUNCTION;
using SignatureTest::NODE_NUMBER;
using SignatureTest::NODE_STRING;

Roo::sptr_val signature_noop([[maybe_unused]] Roo::Context& ctx,
                             [[maybe_unused]] Roo::sptr_val_v& args)
{
  return Roo::Constant::NIL;
}

Roo::uptr_exec_node_v node_list(const std::vector<Roo::uptr_exec_node*>& elements)
{
  Roo::uptr_exec_node_v nodes;
  for (auto* node : elements)
  {
    nodes.push_back((*node)->clone());
  }
  return nodes;
}

TEST_F(Signature, matches_value_arguments)
{
  // Given
  Roo::Signature signature(
    Roo::arg_v{Roo::arg(&Roo::Type::STRING), Roo::arg(&Roo::Type::NUMBER)},
    Roo::exec_val_fn{signature_noop});

  // Then
  EXPECT_TRUE(signature.matches({STRING, NUMBER}));

  EXPECT_FALSE(signature.matches({STRING, STRING}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER, NUMBER}));
}

TEST_F(Signature, matches_node_arguments)
{
  // Given
  Roo::Signature signature(
    Roo::arg_v{Roo::arg(&Roo::Type::STRING), Roo::arg(&Roo::Type::NUMBER)},
    Roo::exec_val_fn{signature_noop});

  // Then
  ;
  EXPECT_TRUE(signature.matches(node_list({&NODE_STRING, &NODE_NUMBER})));

  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING, &NODE_STRING})));
  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING})));
  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING, &NODE_NUMBER, &NODE_NUMBER})));
}

TEST_F(Signature, no_arg_signature_matches_only_empty_arglist)
{
  // Given
  Roo::Signature signature(Roo::arg_v{}, Roo::exec_val_fn{signature_noop});

  // Then
  EXPECT_TRUE(signature.matches(Roo::sptr_val_v{}));

  EXPECT_FALSE(signature.matches({STRING, NUMBER}));
  EXPECT_FALSE(signature.matches({STRING, STRING}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER, NUMBER}));
}

TEST_F(Signature, matches_varargs)
{
  // Given
  Roo::Signature signature(Roo::arg_v{Roo::arg(Roo::VARARG, &Roo::Type::STRING)},
                           Roo::exec_val_fn{signature_noop});

  // Then
  EXPECT_TRUE(signature.matches(Roo::sptr_val_v{}));
  EXPECT_TRUE(signature.matches({STRING}));
  EXPECT_TRUE(signature.matches({STRING, STRING}));
  EXPECT_TRUE(signature.matches({STRING, STRING, STRING}));
  EXPECT_TRUE(signature.matches({STRING, STRING, STRING, STRING}));
  EXPECT_TRUE(signature.matches({STRING, STRING, STRING, STRING, STRING}));
  EXPECT_TRUE(signature.matches({STRING, STRING, STRING, STRING, STRING, STRING}));

  EXPECT_FALSE(signature.matches({NUMBER, STRING}));
  EXPECT_FALSE(signature.matches({NUMBER, STRING, STRING}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER, NUMBER}));
}

TEST_F(Signature, matches__leading_varargs)
{
  // Given
  Roo::MapFunction dummy_func;
  Roo::Signature signature(
    Roo::arg_v{Roo::arg(Roo::VARARG, &Roo::Type::VECTOR), Roo::arg(&Roo::Type::FUNCTION)},
    std::bind(&Roo::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY, FUNCTION}));

  EXPECT_FALSE(signature.matches(Roo::sptr_val_v{}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({FUNCTION, ARRAY}));
}

TEST_F(Signature, matches__trailing_varargs)
{
  // Given
  Roo::MapFunction dummy_func;
  Roo::Signature signature(
    Roo::arg_v{Roo::arg(&Roo::Type::FUNCTION), Roo::arg(Roo::VARARG, &Roo::Type::VECTOR)},
    std::bind(&Roo::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({FUNCTION}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY, ARRAY, ARRAY}));

  EXPECT_FALSE(signature.matches(Roo::sptr_val_v{}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_FALSE(signature.matches({FUNCTION, FUNCTION, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION, ARRAY}));
}

TEST_F(Signature, matches__trailing_varargs_of_same_type)
{
  // Given
  Roo::MapFunction dummy_func;
  Roo::Signature signature(
    Roo::arg_v{Roo::arg(&Roo::Type::VECTOR), Roo::arg(Roo::VARARG, &Roo::Type::VECTOR)},
    std::bind(&Roo::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY, ARRAY}));

  EXPECT_FALSE(signature.matches(Roo::sptr_val_v{}));
  EXPECT_FALSE(signature.matches({ARRAY, STRING}));
  EXPECT_FALSE(signature.matches({STRING, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_FALSE(signature.matches({FUNCTION, FUNCTION, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION, ARRAY}));
}

TEST_F(Signature, coerce_args__map_to_native_type__native_function)
{
  // Given
  auto& reader = runtime;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store("make-vehicle-model",
                                       RooTest::Native::VehicleModelMakeFunction::make());
  reader.get_current_namespace().store("prn-vehicle", RooTest::Native::PrnVehicle::make());
  reader.switch_namespace("user");

  // When
  Roo::sptr_val result =
    reader.eval(R"((vehicle/prn-vehicle {:model-name "Gonzo-mobile" :seats 8}))");

  // Then
  EXPECT_EQ(result->type, Roo::Value::Type::NATIVE_OBJECT);
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*result));
}

TEST_F(Signature, coerce_args__no_coercion_available)
{
  // Given
  auto& reader = runtime;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store("make-vehicle-model",
                                       RooTest::Native::VehicleModelMakeFunction::make());
  reader.get_current_namespace().store("double-size-vehicle",
                                       RooTest::Native::DoubleSizeVehicle::make());
  reader.switch_namespace("user");

  // When/Then
  EXPECT_THAT(
    [&]()
    {
      reader.eval(R"((vehicle/double-size-vehicle {:model-name "Gonzo-mobile" :seats 8}))");
    },
    ThrowsMessage<Roo::InvocationException>(HasSubstr("No matching")));
}

TEST_F(Signature, coerce_args__coerce_vector_of_vector_of_native_object)
{
  // Given
  auto& reader = runtime;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store("make-vehicle-model",
                                       RooTest::Native::VehicleModelMakeFunction::make());
  reader.get_current_namespace().store("v-of-v-taker",
                                       RooTest::Native::VectorOfVectorTaker::make());

  // When
  Roo::sptr_val result = reader.eval(R"((v-of-v-taker
[[(make-vehicle-model {:model-name "Batmobile" :seats 1}) (make-vehicle-model {:model-name "Dreamy Boom-Boom" :seats 10})]
 [(make-vehicle-model {:model-name "V8 Interceptor" :seats 2}) (make-vehicle-model {:model-name "Clown Car" :seats 1})]]
))");

  // Then
  EXPECT_EQ(result->type, Roo::Value::Type::VECTOR);
  EXPECT_EQ(result->elements().size(), 2);

  Roo::sptr_val child1 = result->elements().at(0);
  EXPECT_EQ(child1->type, Roo::Value::Type::VECTOR);
  EXPECT_EQ(child1->elements().size(), 2);

  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*child1->elements().at(0)));
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*child1->elements().at(1)));

  Roo::sptr_val child2 = result->elements().at(1);
  EXPECT_EQ(child2->type, Roo::Value::Type::VECTOR);
  EXPECT_EQ(child2->elements().size(), 2);

  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*child2->elements().at(0)));
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*child2->elements().at(1)));
}

TEST_F(Signature, coerce_args__coerce_vector_elements)
{
  // Given
  auto& reader = runtime;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store("make-vehicle-model",
                                       RooTest::Native::VehicleModelMakeFunction::make());
  reader.get_current_namespace().store("count-seats",
                                       RooTest::Native::CountVehicleSeats::make());
  reader.switch_namespace("user");

  // When
  Roo::sptr_val result = reader.eval(
    R"((vehicle/count-seats [{:model-name "Gonzo-mobile" :seats 8} {:model-name "Dreamy Boom-Boom" :seats 6}]))");

  // Then
  EXPECT_EQ(result->type, Roo::Value::Type::NUMBER);
  EXPECT_EQ(result->i64(), 14);
}
