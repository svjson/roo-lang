
#include <gtest/gtest.h>

#include <lisple/impl.h>
#include <lisple/form.h>
#include <lisple/runtime.h>

TEST(str_val, returns_value_of_string_based_forms)
{
  EXPECT_EQ(Lisple::str_val(Lisple::Key("some-key")), "some-key");
  EXPECT_EQ(Lisple::str_val(Lisple::Word("bird")), "bird");
  EXPECT_EQ(Lisple::str_val(Lisple::QSymbol("ARROW")), "ARROW");
  EXPECT_EQ(Lisple::str_val(Lisple::String("boing-boing")), "boing-boing");
}

TEST(str_val, returns_value_of_number_as_string)
{
  EXPECT_EQ(Lisple::str_val(Lisple::Number(242)), "242");
  EXPECT_EQ(Lisple::str_val(Lisple::Number(18.65f)), "18.650000");
}

TEST(subst_sexp_lmnt, substitute_first_element_of_array)
{
  // Given
  Lisple::Runtime runtime;;
  Lisple::Array array = runtime.eval("[1 2 3 4]")->as<Lisple::Array>();

  // When
  auto result = Lisple::subst_sexp_lmnt(array, 0, std::make_shared<Lisple::String>("one"));

  // Then
  ASSERT_TRUE(Lisple::Type::ARRAY.is_type_of(*result));
  ASSERT_EQ(result->size(), 4);

  EXPECT_EQ(result->to_string(), "[\"one\" 2 3 4]");
  EXPECT_EQ(array.to_string(), "[1 2 3 4]");
}
