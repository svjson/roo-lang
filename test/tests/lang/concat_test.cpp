
#include "runtime_fixture.h"

#include "gmock/gmock.h"
#include "host/test_adapters/vehicle_host_adapters.h"
#include <gtest/gtest.h>


using ConcatFunction = LispleTest::RuntimeTestFixture;
using namespace ::testing;

/*
 * ======================================================================
 * ConcatFunction - (concat [...] 5)
 * ConcatFunction - (concat [...] [...])
 * ======================================================================
 */

TEST_F(ConcatFunction, numbers)
{
  // Given
  auto retval = runtime.eval("(concat [1 2 3] [4 5 6])");

  // Then
  EXPECT_EQ(*retval, *runtime.eval("[1 2 3 4 5 6]"));
}

TEST_F(ConcatFunction, number_to_vector_int)
{
  // Given
  std::vector<int> int_v{1, 2, 3};
  runtime.get_current_namespace().store("wrapped-vec",
                                        std::make_shared<Lisple::VectorInt>(int_v));

  // When
  auto retval = runtime.eval("(concat wrapped-vec 4)");

  // Then
  EXPECT_EQ(retval->to_string(), runtime.eval("[1 2 3 4]")->to_string());
  EXPECT_EQ(runtime.lookup_value("wrapped-vec")->to_string(),
            runtime.eval("[1 2 3]")->to_string());
  EXPECT_THAT(int_v, ElementsAre(1, 2, 3));
}

TEST_F(ConcatFunction, host_objects_and_primitives)
{
  // Given
  runtime.get_current_namespace().store(
    "cessna",
    LispleTest::VehicleModelAdapter::make<LispleTest::VehicleModel>("Cessna", 2));

  // When
  auto retval = runtime.eval("(concat [] cessna {:a 2} \"rawk n rawl\")");

  EXPECT_EQ(retval->to_string(),
            R"([{:model-name "Cessna" :seats 2} {:a 2} "rawk n rawl"])");
}
