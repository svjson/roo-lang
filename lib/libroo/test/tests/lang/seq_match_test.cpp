
#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SeqMatchFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * SeqMatchFunction - (seq-match [{...} {...}] pattern)
 * ======================================================================
 */

TEST_F(SeqMatchFunction, seq_match_single_field)
{
  // Given
  runtime.eval(R"((def my-seq [{:id 1 :name "Air Man"} {:id 2 :name "Bubble Man"}]))");

  // Then
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 1})")->to_string(),
            R"({:id 1 :name "Air Man"})");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 2})")->to_string(),
            R"({:id 2 :name "Bubble Man"})");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id 3})")->to_string(), R"(nil)");
}

TEST_F(SeqMatchFunction, seq_match_missing_field)
{
  // Given
  runtime.eval(R"((def my-seq [{:id 1 :name "Air Man"} {:id 2 :name "Bubble Man"}]))");

  // Then
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:electric-boogaloo 2})")->to_string(), "nil");
  EXPECT_EQ(runtime.eval("(seq-match my-seq {:id nil})")->to_string(), "nil");
}

TEST_F(SeqMatchFunction, seq_match_nested)
{
  // Given
  const std::string air_man =
    R"({:id 1 :name "Air Man" :stats {:weapon "Air Shooter" :weakness "Leaf Shield"}})";
  const std::string bubble_man =
    R"({:id 2 :name "Bubble Man" :stats {:weapon "Bubble Lead" :weakness "Metal Blade"}})";
  const std::string wood_man =
    R"({:id 3 :name "Wood Man" :stats {:weapon "Leaf Shield" :weakness "Metal Blade"}})";

  // When
  runtime.eval("(def my-seq [" + air_man + bubble_man + wood_man + "])");

  // Then
  EXPECT_EQ(
    runtime.eval(R"((seq-match my-seq {:stats {:weakness "Metal Blade"}}))")->to_string(),
    bubble_man);
  EXPECT_EQ(
    runtime.eval(R"((seq-match my-seq {:stats {:weapon "Leaf Shield"}}))")->to_string(),
    wood_man);
  EXPECT_EQ(runtime.eval(R"((seq-match my-seq {:id 3 :stats {:weapon "Leaf Shield"}}))")
              ->to_string(),
            wood_man);
  EXPECT_EQ(
    runtime.eval(R"((seq-match my-seq {:stats {:weakness "Bubble Lead"}}))")->to_string(),
    "nil");
}
