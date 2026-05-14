
#include "host/test_adapters/vectorgfx_native_adapters.h"
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MergeFunction = LispleTest::RuntimeTestFixture;
TEST_F(MergeFunction, merge_two_maps_with_unique_keys)
{
  // Given
  EXPECT_EQ(runtime.eval("(merge {:x 10 :y 20} {:w 100 :h 150})")->to_string(),
            "{:x 10 :y 20 :w 100 :h 150}");
}

TEST_F(MergeFunction, merge_two_maps_with_overlapping_keys)
{
  // Given
  EXPECT_EQ(runtime.eval("(merge {:x 10 :y 20 :w 12} {:w 100 :y 50 :h 150})")->to_string(),
            "{:x 10 :y 50 :w 100 :h 150}");
}

TEST_F(MergeFunction, merge_empty_map_with_non_empty_map)
{
  // Given
  EXPECT_EQ(runtime.eval("(merge {} {:w 100 :y 50 :h 150})")->to_string(),
            "{:w 100 :y 50 :h 150}");
}

TEST_F(MergeFunction, merge_adapter_with_map)
{
  // Given
  runtime.get_current_namespace().store(
    "my-point",
    LispleTest::Native::PointAdapter::make_unique(15, 30));

  // When
  auto retval = runtime.eval("(merge my-point {:w 100 :h 150})");

  // Then
  EXPECT_EQ(retval->to_string(), "{:x 15 :y 30 :w 100 :h 150}");
  EXPECT_EQ(retval->type, Lisple::Value::Type::MAP);
}
