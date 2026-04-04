
#include "lisple/type.h"

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>

#include <gtest/gtest.h>

TEST(LowerTest, number__literal_node)
{
  // Given
  Lisple::LowerContext lctx;
  Lisple::sptr_sobject number = Lisple::Number::make(4);

  // When
  auto node = Lisple::lower_expr(lctx, number);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::LiteralNode>(node->data));
}

TEST(LowerTest, list__call_node__no_ctx__lookup_node_callee)
{
  Lisple::LowerContext lctx;
  Lisple::sptr_sobject expr = Lisple::List::make(
    {Lisple::Word::make("+"), Lisple::Number::make(1), Lisple::Number::make(1)});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::CallNode>(node->data));
  Lisple::CallNode& call_node = std::get<Lisple::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Lisple::LookupNode>(call_node.callee->data));
}

TEST(LowerTest, list__call_node__ctx__literal_node_callee)
{
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject expr = Lisple::List::make(
    {Lisple::Word::make("+"), Lisple::Number::make(1), Lisple::Number::make(1)});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::CallNode>(node->data));
  Lisple::CallNode& call_node = std::get<Lisple::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Lisple::LookupNode>(call_node.callee->data));
}
