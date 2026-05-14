

#include "runtime_fixture.h"

#include <gtest/gtest.h>


using CaseMacro = LispleTest::RuntimeTestFixture;
TEST_F(CaseMacro, constants)
{
  // Given
  auto& reader = runtime;
  auto result =
    reader.eval(R"((case 20 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zwanzig"));
}

TEST_F(CaseMacro, expressions)
{
  // Given
  auto& reader = runtime;
  auto result =
    reader.eval(R"((case (- 20 10) (- 10 10) "Zilch" (+ 5 5) "Zen" :default "Zillions"))");
  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zen"));
}

TEST_F(CaseMacro, no_match_with_default)
{
  // Given
  auto& reader = runtime;
  auto result =
    reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zillions"));
}

TEST_F(CaseMacro, no_match_without_default)
{
  // Given
  auto& reader = runtime;
  auto result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}
