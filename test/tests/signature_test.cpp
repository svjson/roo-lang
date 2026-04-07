

#include "lisple/runtime/node.h"
#include "lisple/runtime/value.h"

#include <functional>
#include <memory>

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/host.h>
#include <lisple/lang.h>
#include <lisple/lang/base.h>
#include <lisple/lang/loop.h>
#include <lisple/lang/operator.h>
#include <lisple/lang/seq_func.h>
#include <lisple/runtime.h>
#include <lisple/type.h>

#include "gmock/gmock.h"
#include "host/test_adapters/vehicle_host_adapters.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

using namespace ::testing;

namespace SignatureTest
{
  Lisple::sptr_rtval STRING = Lisple::RTValue::string("str");
  Lisple::sptr_rtval NUMBER = Lisple::RTValue::number(5);
  Lisple::sptr_rtval ARRAY = Lisple::RTValue::vector({});
  Lisple::sptr_rtval FUNCTION = Lisple::MapFunction::make();

  Lisple::uptr_exec_node NODE_STRING = std::make_unique<Lisple::ExecNode>(STRING);
  Lisple::uptr_exec_node NODE_NUMBER = std::make_unique<Lisple::ExecNode>(NUMBER);
  Lisple::uptr_exec_node NODE_ARRAY = std::make_unique<Lisple::ExecNode>(ARRAY);
  Lisple::uptr_exec_node NODE_FUNCTION = std::make_unique<Lisple::ExecNode>(FUNCTION);

  Lisple::sptr_sobject AST_STRING = std::make_shared<Lisple::String>("str");
  Lisple::sptr_sobject AST_NUMBER = std::make_shared<Lisple::Number>(5);
  Lisple::sptr_sobject AST_ARRAY = std::make_shared<Lisple::Array>();
  Lisple::sptr_sobject AST_FUNCTION = std::make_shared<Lisple::MapFunction>();

} // namespace SignatureTest

using SignatureTest::ARRAY;
using SignatureTest::FUNCTION;
using SignatureTest::NUMBER;
using SignatureTest::STRING;

using SignatureTest::NODE_ARRAY;
using SignatureTest::NODE_FUNCTION;
using SignatureTest::NODE_NUMBER;
using SignatureTest::NODE_STRING;

using SignatureTest::AST_ARRAY;
using SignatureTest::AST_FUNCTION;
using SignatureTest::AST_NUMBER;
using SignatureTest::AST_STRING;

Lisple::uptr_exec_node_v node_list(const std::vector<Lisple::uptr_exec_node*>& elements)
{
  Lisple::uptr_exec_node_v nodes;
  for (auto* node : elements)
  {
    nodes.push_back((*node)->clone());
  }
  return nodes;
}

TEST(Signature, matches_ast_arguments)
{
  // Given
  Lisple::AndForm dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING), Lisple::arg(&Lisple::Type::NUMBER)},
    std::bind(&Lisple::AndForm::inv_and,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  // Then
  EXPECT_TRUE(signature.matches({AST_STRING, AST_NUMBER}));

  EXPECT_FALSE(signature.matches({AST_STRING, AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_NUMBER, AST_NUMBER}));
}

TEST(Signature, matches_rtval_arguments)
{
  // Given
  Lisple::AndForm dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING), Lisple::arg(&Lisple::Type::NUMBER)},
    std::bind(&Lisple::AndForm::inv_and,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  // Then
  EXPECT_TRUE(signature.matches({STRING, NUMBER}));

  EXPECT_FALSE(signature.matches({STRING, STRING}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER, NUMBER}));
}

TEST(Signature, matches_node_arguments)
{
  // Given
  Lisple::AndForm dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING), Lisple::arg(&Lisple::Type::NUMBER)},
    std::bind(&Lisple::AndForm::inv_and,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  // Then
  ;
  EXPECT_TRUE(signature.matches(node_list({&NODE_STRING, &NODE_NUMBER})));

  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING, &NODE_STRING})));
  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING})));
  EXPECT_FALSE(signature.matches(node_list({&NODE_STRING, &NODE_NUMBER, &NODE_NUMBER})));
}

TEST(Signature, no_arg_signature_matches_only_empty_arglist)
{
  // Given
  Lisple::AndForm dummy_func;
  Lisple::Signature signature(Lisple::arg_v{},
                              std::bind(&Lisple::AndForm::inv_and,
                                        &dummy_func,
                                        std::placeholders::_1,
                                        std::placeholders::_2));

  // Then
  EXPECT_TRUE(signature.matches(Lisple::sptr_sobject_v{}));

  EXPECT_FALSE(signature.matches({AST_STRING, AST_NUMBER}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_NUMBER, AST_NUMBER}));
}

TEST(Signature, matches_varargs)
{
  // Given
  Lisple::AndForm dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(Lisple::VARARG, &Lisple::Type::STRING)},
    std::bind(&Lisple::AndForm::inv_and,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  // Then
  EXPECT_TRUE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_TRUE(signature.matches({AST_STRING}));
  EXPECT_TRUE(signature.matches({AST_STRING, AST_STRING}));
  EXPECT_TRUE(signature.matches({AST_STRING, AST_STRING, AST_STRING}));
  EXPECT_TRUE(signature.matches({AST_STRING, AST_STRING, AST_STRING, AST_STRING}));
  EXPECT_TRUE(
    signature.matches({AST_STRING, AST_STRING, AST_STRING, AST_STRING, AST_STRING}));
  EXPECT_TRUE(signature.matches(
    {AST_STRING, AST_STRING, AST_STRING, AST_STRING, AST_STRING, AST_STRING}));

  EXPECT_FALSE(signature.matches({AST_NUMBER, AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_NUMBER, AST_STRING, AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_NUMBER}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_NUMBER, AST_NUMBER}));
}

TEST(Signature, matches__leading_varargs)
{
  // Given
  Lisple::MapFunction dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(Lisple::VARARG, &Lisple::Type::ARRAY),
                  Lisple::arg(&Lisple::Type::FUNCTION)},
    std::bind(&Lisple::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({AST_FUNCTION}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_FUNCTION}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_ARRAY, AST_FUNCTION}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_ARRAY, AST_ARRAY, AST_FUNCTION}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_FUNCTION, AST_ARRAY}));
}

TEST(Signature, matches__trailing_varargs)
{
  // Given
  Lisple::MapFunction dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::FUNCTION),
                  Lisple::arg(Lisple::VARARG, &Lisple::Type::ARRAY)},
    std::bind(&Lisple::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({AST_FUNCTION}));
  EXPECT_TRUE(signature.matches({AST_FUNCTION, AST_ARRAY}));
  EXPECT_TRUE(signature.matches({AST_FUNCTION, AST_ARRAY, AST_ARRAY}));
  EXPECT_TRUE(signature.matches({AST_FUNCTION, AST_ARRAY, AST_ARRAY, AST_ARRAY}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_ARRAY}));
  EXPECT_FALSE(signature.matches({AST_ARRAY, AST_FUNCTION}));
  EXPECT_FALSE(signature.matches({AST_FUNCTION, AST_FUNCTION, AST_ARRAY}));
  EXPECT_FALSE(signature.matches({AST_ARRAY, AST_FUNCTION, AST_ARRAY}));
}

TEST(Signature, matches__trailing_varargs_of_same_type)
{
  // Given
  Lisple::MapFunction dummy_func;
  Lisple::Signature signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::ARRAY),
                  Lisple::arg(Lisple::VARARG, &Lisple::Type::ARRAY)},
    std::bind(&Lisple::MapFunction::exec_map,
              &dummy_func,
              std::placeholders::_1,
              std::placeholders::_2));

  EXPECT_TRUE(signature.matches({AST_ARRAY}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_ARRAY}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_ARRAY, AST_ARRAY}));
  EXPECT_TRUE(signature.matches({AST_ARRAY, AST_ARRAY, AST_ARRAY, AST_ARRAY}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({AST_ARRAY, AST_STRING}));
  EXPECT_FALSE(signature.matches({AST_STRING, AST_ARRAY}));
  EXPECT_FALSE(signature.matches({AST_ARRAY, AST_FUNCTION}));
  EXPECT_FALSE(signature.matches({AST_FUNCTION, AST_FUNCTION, AST_ARRAY}));
  EXPECT_FALSE(signature.matches({AST_ARRAY, AST_FUNCTION, AST_ARRAY}));
}

TEST(Signature, coerce_args__map_to_host_type)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("prn-vehicle"),
                                       std::make_shared<LispleTest::PrnVehicle>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_rtval result =
    reader.eval(R"((vehicle/prn-vehicle {:model-name "Gonzo-mobile" :seats 8}))");

  // Then
  EXPECT_EQ(result->type, Lisple::RTValue::Type::OBJECT);
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*result));
}

TEST(Signature, coerce_args__map_to_native_type__native_function)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::Native::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("prn-vehicle"),
                                       std::make_shared<LispleTest::Native::PrnVehicle>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_rtval result =
    reader.eval(R"((vehicle/prn-vehicle {:model-name "Gonzo-mobile" :seats 8}))");

  // Then
  EXPECT_EQ(result->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*result));
}

TEST(Signature, coerce_args__map_to_native_type__call_ast_based_native_function)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::Native::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("prn-vehicle"),
                                       std::make_shared<LispleTest::PrnVehicle>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_rtval result =
    reader.eval(R"((vehicle/prn-vehicle {:model-name "Gonzo-mobile" :seats 8}))");

  // Then
  EXPECT_EQ(result->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*result));
}

TEST(Signature, coerce_args__no_coercion_available)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("double-size-vehicle"),
                                       std::make_shared<LispleTest::DoubleSizeVehicle>());
  reader.switch_namespace("user");

  // When/Then
  EXPECT_THAT(
    [&reader]()
    {
      reader.eval(R"((vehicle/double-size-vehicle {:model-name "Gonzo-mobile" :seats 8}))");
    },
    ThrowsMessage<Lisple::InvocationException>(HasSubstr("No matching signature")));
}

TEST(Signature, coerce_args__coerce_array_of_array_of_host_object)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("a-of-a-taker"),
                                       std::make_shared<LispleTest::ArrayOfArrayTaker>());

  // When
  Lisple::sptr_sobject result = reader.eval_ast(R"((a-of-a-taker
[[(make-vehicle-model {:model-name "Batmobile" :seats 1}) (make-vehicle-model {:model-name "Dreamy Boom-Boom" :seats 10})]
 [(make-vehicle-model {:model-name "V8 Interceptor" :seats 2}) (make-vehicle-model {:model-name "Clown Car" :seats 1})]]
))");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));
  EXPECT_EQ(result->size(), 2);

  Lisple::sptr_sobject child1 = result->get_children().at(0);
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*child1));
  EXPECT_EQ(child1->size(), 2);

  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*child1->get_children().at(0)));
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*child1->get_children().at(1)));

  Lisple::sptr_sobject child2 = result->get_children().at(1);
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*child2));
  EXPECT_EQ(child2->size(), 2);

  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*child2->get_children().at(0)));
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*child2->get_children().at(1)));
}

TEST(Signature, coerce_args__coerce_array_elements)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(
    Lisple::Word("make-vehicle-model"),
    std::make_shared<LispleTest::VehicleModelMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("count-seats"),
                                       std::make_shared<LispleTest::CountVehicleSeats>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_rtval result = reader.eval(
    R"((vehicle/count-seats [{:model-name "Gonzo-mobile" :seats 8} {:model-name "Dreamy Boom-Boom" :seats 6}]))");

  // Then
  EXPECT_EQ(result->type, Lisple::RTValue::Type::NUMBER);
  EXPECT_EQ(result->i64(), 14);
}
