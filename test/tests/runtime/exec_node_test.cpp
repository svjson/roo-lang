
#include <gtest/gtest.h>

#include "lisple/form.h"
#include "lisple/runtime/exec_node.h"

TEST(ExecNodeTest, lower_expr__number__literal_node)
{
  // Given
  Lisple::sptr_sobject number = Lisple::Number::make(4);

  // When
  auto node = Lisple::lower_expr(number);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::LiteralNode>(node->data));
}
