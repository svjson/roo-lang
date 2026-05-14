
#include "runtime_fixture.h"

#include "host/test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest.h>


using ConcatFunction = LispleTest::RuntimeTestFixture;

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

TEST_F(ConcatFunction, host_objects_and_primitives)
{
  // Given
  runtime.get_current_namespace().store(
    "cessna",
    LispleTest::Native::VehicleModelAdapter::make_unique("Cessna", 2));

  // When
  auto retval = runtime.eval("(concat [] cessna {:a 2} \"rawk n rawl\")");

  EXPECT_EQ(retval->to_string(),
            R"([{:model-name "Cessna" :seats 2} {:a 2} "rawk n rawl"])");
}
