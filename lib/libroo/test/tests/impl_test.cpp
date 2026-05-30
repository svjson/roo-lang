
#include "roo/runtime/value.h"

#include <roo/form.h>
#include <roo/impl.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using str_val = RooTest::RuntimeTestFixture;
using subst_sexp_lmnt = RooTest::RuntimeTestFixture;
TEST_F(str_val, returns_value_of_string_based_forms)
{
  EXPECT_EQ(Roo::str_val(Roo::AST::Keyword("some-key")), "some-key");
  EXPECT_EQ(Roo::str_val(Roo::AST::Symbol("bird")), "bird");
  EXPECT_EQ(Roo::str_val(Roo::AST::QuotedSymbol("ARROW")), "ARROW");
  EXPECT_EQ(Roo::str_val(Roo::AST::String("boing-boing")), "boing-boing");
}

TEST_F(str_val, returns_value_of_number_as_string)
{
  EXPECT_EQ(Roo::str_val(Roo::AST::Number(242)), "242");
  EXPECT_EQ(Roo::str_val(Roo::AST::Number(18.65f)), "18.650000");
}

TEST_F(subst_sexp_lmnt, substitute_first_element_of_vector)
{
  // Given
  ;
  Roo::AST::Vector vector = Roo::to_AST(*runtime.eval("[1 2 3 4]"))->as<Roo::AST::Vector>();

  // When
  auto result = Roo::subst_sexp_lmnt(vector, 0, std::make_shared<Roo::AST::String>("one"));

  // Then
  ASSERT_TRUE(Roo::Type::VECTOR.is_type_of(*result));
  ASSERT_EQ(result->size(), 4);

  EXPECT_EQ(result->to_string(), "[\"one\" 2 3 4]");
  EXPECT_EQ(vector.to_string(), "[1 2 3 4]");
}
