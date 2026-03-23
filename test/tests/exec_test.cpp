
#include "gmock/gmock.h"
#include "runtime_fixture.h"
#include "test_host_objects.h"
#include <functional>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
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
#include <memory>
#include <stdexcept>

using namespace ::testing;

namespace SignatureTest
{
  Lisple::sptr_sobject STRING = std::make_shared<Lisple::String>("str");
  Lisple::sptr_sobject NUMBER = std::make_shared<Lisple::Number>(5);
  Lisple::sptr_sobject ARRAY = std::make_shared<Lisple::Array>();
  Lisple::sptr_sobject FUNCTION = std::make_shared<Lisple::MapFunction>();
} // namespace SignatureTest

using SignatureTest::ARRAY;
using SignatureTest::FUNCTION;
using SignatureTest::NUMBER;
using SignatureTest::STRING;

TEST(Signature, matches_arguments)
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

  EXPECT_FALSE(signature.matches({STRING, NUMBER}));
  EXPECT_FALSE(signature.matches({STRING, STRING}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({STRING, NUMBER, NUMBER}));
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

  EXPECT_TRUE(signature.matches({FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, FUNCTION}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY, FUNCTION}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({FUNCTION, ARRAY}));
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

  EXPECT_TRUE(signature.matches({FUNCTION}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({FUNCTION, ARRAY, ARRAY, ARRAY}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({STRING}));
  EXPECT_FALSE(signature.matches({ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_FALSE(signature.matches({FUNCTION, FUNCTION, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION, ARRAY}));
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

  EXPECT_TRUE(signature.matches({ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY}));
  EXPECT_TRUE(signature.matches({ARRAY, ARRAY, ARRAY, ARRAY}));

  EXPECT_FALSE(signature.matches(Lisple::sptr_sobject_v{}));
  EXPECT_FALSE(signature.matches({ARRAY, STRING}));
  EXPECT_FALSE(signature.matches({STRING, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION}));
  EXPECT_FALSE(signature.matches({FUNCTION, FUNCTION, ARRAY}));
  EXPECT_FALSE(signature.matches({ARRAY, FUNCTION, ARRAY}));
}

TEST(Signature, coerce_args__map_to_host_type)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(Lisple::Word("make-vehicle"),
                                       std::make_shared<Tests::VehicleMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("prn-vehicle"),
                                       std::make_shared<Tests::PrnVehicle>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_sobject result =
    reader.eval(R"((vehicle/prn-vehicle {:model-name "Gonzo-mobile" :seats 8}))");

  // Then
  EXPECT_EQ(result->get_type(), Lisple::Form::HOST_OBJECT);
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*result));
}

TEST(Signature, coerce_args__no_coercion_available)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(Lisple::Word("make-vehicle"),
                                       std::make_shared<Tests::VehicleMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("double-size-vehicle"),
                                       std::make_shared<Tests::DoubleSizeVehicle>());
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
  reader.get_current_namespace().store(Lisple::Word("make-vehicle"),
                                       std::make_shared<Tests::VehicleMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("a-of-a-taker"),
                                       std::make_shared<Tests::ArrayOfArrayTaker>());

  // When
  Lisple::sptr_sobject result = reader.eval(R"((a-of-a-taker
[[(make-vehicle {:model-name "Batmobile" :seats 1}) (make-vehicle {:model-name "Dreamy Boom-Boom" :seats 10})]
 [(make-vehicle {:model-name "V8 Interceptor" :seats 2}) (make-vehicle {:model-name "Clown Car" :seats 1})]]
))");

  // Then
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));
  EXPECT_EQ(result->size(), 2);

  Lisple::sptr_sobject child1 = result->get_children().at(0);
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*child1));
  EXPECT_EQ(child1->size(), 2);

  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*child1->get_children().at(0)));
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*child1->get_children().at(1)));

  Lisple::sptr_sobject child2 = result->get_children().at(1);
  EXPECT_TRUE(Lisple::Type::ARRAY.is_type_of(*child2));
  EXPECT_EQ(child2->size(), 2);

  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*child2->get_children().at(0)));
  EXPECT_TRUE(Tests::VEHICLE_TYPE.is_type_of(*child2->get_children().at(1)));
}

TEST(Signature, coerce_args__coerce_array_elements)
{
  // Given
  Lisple::Runtime reader;
  reader.switch_namespace("vehicle");
  reader.get_current_namespace().store(Lisple::Word("make-vehicle"),
                                       std::make_shared<Tests::VehicleMakeFunction>());
  reader.get_current_namespace().store(Lisple::Word("count-seats"),
                                       std::make_shared<Tests::CountVehicleSeats>());
  reader.switch_namespace("user");

  // When
  Lisple::sptr_sobject result = reader.eval(
    R"((vehicle/count-seats [{:model-name "Gonzo-mobile" :seats 8} {:model-name "Dreamy Boom-Boom" :seats 6}]))");

  // Then
  EXPECT_EQ(result->get_type(), Lisple::Form::NUMBER);
  EXPECT_EQ(result->as<Lisple::Number>().int_value(), 14);
}

TEST(NamedArgument, apply)
{
  // Given
  Lisple::Scope scope;
  Lisple::NamedArgumentBinding arg("muffin");
  Lisple::sptr_sobject val = std::make_unique<Lisple::Key>("blooper");

  // When
  arg.apply(scope, val);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 1);
  ASSERT_TRUE(scope.has(Lisple::Word("muffin")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("muffin")), Lisple::Key("blooper"));
}

TEST(DestructuringArgumentBinding, invalid_binding_form_throws_exception)
{
  // Given
  Lisple::Runtime runtime;

  auto arg_form = Lisple::Map::make(
    {Lisple::sptr_sobject_v{Lisple::Key::make("keys"),
                            Lisple::Map::make({// Invalid - this should be Array
                                               Lisple::Word::make("var1"),
                                               Lisple::Word::make("var2")})}});

  // When / Then
  try
  {
    Lisple::DestructuringArgumentBinding binding{*arg_form};

    FAIL() << "Expected TypeError to be thrown for destructuring binding: "
           << arg_form->to_string();
  }
  catch (Lisple::TypeError& e)
  {
    EXPECT_THAT(std::string{e.what()},
                HasSubstr("Invalid destructuring form: {:keys {var1 var2}}"));
  }
  catch (std::runtime_error& e)
  {
    FAIL() << "Expected TypeError exception, but got something else: " << e.what();
  }
}

TEST(DestructuringArgumentBinding, apply__keys_only)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_sobject map = std::make_shared<Lisple::Map>(
    Lisple::sptr_sobject_v{std::make_shared<Lisple::Key>("source"),
                           std::make_shared<Lisple::Word>("The Thing"),
                           std::make_shared<Lisple::Key>("target"),
                           std::make_shared<Lisple::Word>("The Thang")});

  Lisple::Map arg_map{
    Lisple::sptr_sobject_v{std::make_shared<Lisple::Key>("keys"),
                           std::make_shared<Lisple::Array>(Lisple::sptr_sobject_v{
                             std::make_shared<Lisple::Word>("source"),
                             std::make_shared<Lisple::Word>("target")})}};
  Lisple::DestructuringArgumentBinding arg{arg_map};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 2);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("The Thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::Word("The Thang"));
}

TEST(DestructuringArgumentBinding, apply__keys_and_alias)
{
  // Given
  Lisple::Scope scope;

  Lisple::sptr_sobject map =
    Lisple::Map::make({std::make_shared<Lisple::Key>("source"),
                       std::make_shared<Lisple::Word>("The Thing"),
                       std::make_shared<Lisple::Key>("target"),
                       std::make_shared<Lisple::Word>("The Thang")});

  Lisple::Map arg_map({std::make_shared<Lisple::Key>("keys"),
                       Lisple::Array::make({std::make_shared<Lisple::Word>("source"),
                                            std::make_shared<Lisple::Word>("target")}),
                       std::make_shared<Lisple::Key>("as"),
                       std::make_shared<Lisple::Word>("context")});

  Lisple::DestructuringArgumentBinding arg{arg_map};

  // When
  arg.apply(scope, map);

  // Then
  ASSERT_EQ(scope.get_keys()->size(), 3);
  ASSERT_TRUE(scope.has(Lisple::Word("source")));
  ASSERT_TRUE(scope.has(Lisple::Word("target")));
  ASSERT_TRUE(scope.has(Lisple::Word("context")));
  ASSERT_EQ(*scope.lookup(Lisple::Word("source")), Lisple::Word("The Thing"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("target")), Lisple::Word("The Thang"));
  ASSERT_EQ(*scope.lookup(Lisple::Word("context")), *map);
}

TEST(Executable, invocation_with_incorrect_argument_types_throws_exception)
{
  // Given
  LispleTest::RuntimeFixture fixture;
  Lisple::PlusFunction plus_func;
  Lisple::sptr_sobject_v args;

  args.push_back(std::make_shared<Lisple::String>("4"));
  args.push_back(std::make_shared<Lisple::Number>(4));

  Lisple::sptr_sobject result = nullptr;

  // When
  std::string msg;
  try
  {
    auto result = plus_func.execute(fixture.ctx, args);
  }
  catch (std::exception& e)
  {
    msg = e.what();
  }

  // Then
  EXPECT_FALSE(result.get());
  EXPECT_THAT(msg, HasSubstr("No matching signature"));
}

TEST(UserFunction, invocation_of_empty_function_returns_nil)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(defun my-fn [arg])");

  auto fn_sptr = runtime.lookup(Lisple::Word("my-fn"));
  Lisple::UserFunction& user_fn = fn_sptr->as<Lisple::UserFunction>();

  Lisple::sptr_sobject_v args = {Lisple::String::make("A string!")};
  Lisple::Context ctx(runtime);

  // When
  auto retval = user_fn.execute(ctx, args);

  // Then
  ASSERT_EQ(*retval, *Lisple::NIL);
}

TEST(Macro, get_signature__sig_with_varargs__array__form)
{
  // Given
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::DoTimesForm dotimes;

  Lisple::sptr_sobject_v array__form{
    Lisple::Array::make({Lisple::Word::make("n"), Lisple::Number::make(4)}),
    Lisple::List::make(
      {Lisple::Word::make("+"), Lisple::Number::make(2), Lisple::Number::make(10)})};

  // When
  Lisple::Signature* sig = dotimes.get_signature(ctx, array__form);

  // Then
  ASSERT_NE(sig, nullptr);

  ASSERT_EQ(sig->get_arguments().size(), 2);
  EXPECT_EQ(sig->to_string(), "[Vector, <any>...]");
}

TEST(create_function__RTValue_semantics, function_should_support_rtvalue_execution)
{
  // Given
  Lisple::Runtime runtime;

  std::cout << " ---------- TEST BEGINS ---------------" << std::endl;

  Lisple::sptr_rtval_v param_array{Lisple::RTValue::symbol("n")};
  auto body_node = std::make_unique<Lisple::ExecNode>(Lisple::RTValue::number(10));
  Lisple::ptr_exec_node_v body = {body_node.get()};

  // When
  auto func = Lisple::create_function(&runtime.get_current_namespace(), param_array, body);

  // Then
  EXPECT_FALSE(func->supports_exec_tree());
  EXPECT_EQ(func->get_signatures().size(), 1);
  EXPECT_TRUE(func->get_signatures()[0]->supports_rt_value());
}
