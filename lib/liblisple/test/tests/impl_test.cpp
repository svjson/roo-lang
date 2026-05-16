
#include "lisple/runtime/value.h"

#include <lisple/form.h>
#include <lisple/impl.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using str_val = LispleTest::RuntimeTestFixture;
using subst_sexp_lmnt = LispleTest::RuntimeTestFixture;
TEST_F(str_val, returns_value_of_string_based_forms)
{
  EXPECT_EQ(Lisple::str_val(Lisple::AST::Keyword("some-key")), "some-key");
  EXPECT_EQ(Lisple::str_val(Lisple::AST::Symbol("bird")), "bird");
  EXPECT_EQ(Lisple::str_val(Lisple::AST::QuotedSymbol("ARROW")), "ARROW");
  EXPECT_EQ(Lisple::str_val(Lisple::AST::String("boing-boing")), "boing-boing");
}

TEST_F(str_val, returns_value_of_number_as_string)
{
  EXPECT_EQ(Lisple::str_val(Lisple::AST::Number(242)), "242");
  EXPECT_EQ(Lisple::str_val(Lisple::AST::Number(18.65f)), "18.650000");
}

TEST_F(subst_sexp_lmnt, substitute_first_element_of_vector)
{
  // Given
  ;
  Lisple::AST::Vector vector =
    Lisple::to_AST(*runtime.eval("[1 2 3 4]"))->as<Lisple::AST::Vector>();

  // When
  auto result =
    Lisple::subst_sexp_lmnt(vector, 0, std::make_shared<Lisple::AST::String>("one"));

  // Then
  ASSERT_TRUE(Lisple::Type::VECTOR.is_type_of(*result));
  ASSERT_EQ(result->size(), 4);

  EXPECT_EQ(result->to_string(), "[\"one\" 2 3 4]");
  EXPECT_EQ(vector.to_string(), "[1 2 3 4]");
}
