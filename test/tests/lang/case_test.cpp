

#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(CaseMacro, constants)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case 20 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zwanzig"));
}

TEST(CaseMacro, expressions)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case (- 20 10) (- 10 10) "Zilch" (+ 5 5) "Zen" :default "Zillions"))");
  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zen"));
}

TEST(CaseMacro, no_match_with_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result =
    reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig" :default "Zillions"))");

  // Then
  ASSERT_EQ(*result, *Lisple::RTValue::string("Zillions"));
}

TEST(CaseMacro, no_match_without_default)
{
  // Given
  Lisple::Runtime reader;

  // When
  auto result = reader.eval(R"((case 100 0 "Zilch" 10 "Zen" 20 "Zwanzig"))");

  // Then
  ASSERT_EQ(*result, *Lisple::Constant::NIL);
}
