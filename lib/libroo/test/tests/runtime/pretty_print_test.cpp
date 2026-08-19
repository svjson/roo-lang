#include <roo/runtime/pretty_print.h>

#include <gtest/gtest.h>

using Roo::Pretty::PrintNode;
using Roo::Pretty::render;
using Roo::Pretty::RenderOptions;

namespace
{
  PrintNode bracketed(std::vector<PrintNode> items, int indent_width = 2)
  {
    PrintNode body = PrintNode::join(std::move(items), PrintNode::line());
    return PrintNode::group(PrintNode::concat(
      {PrintNode::text("["),
       PrintNode::nest(indent_width,
                       PrintNode::concat({PrintNode::soft_line(), std::move(body)})),
       PrintNode::soft_line(),
       PrintNode::text("]")}));
  }
} // namespace

TEST(PrintNode, text_renders_verbatim)
{
  // Given
  PrintNode node = PrintNode::text("hello");

  // When
  std::string result = render(node, RenderOptions{80});

  // Then
  EXPECT_EQ(result, "hello");
}

TEST(PrintNode, group_collapses_to_one_line_when_it_fits)
{
  // Given
  PrintNode node =
    bracketed({PrintNode::text("1"), PrintNode::text("2"), PrintNode::text("3")});

  // When
  std::string result = render(node, RenderOptions{80});

  // Then
  EXPECT_EQ(result, "[1 2 3]");
}

TEST(PrintNode, group_breaks_across_lines_when_it_does_not_fit)
{
  // Given
  PrintNode node =
    bracketed({PrintNode::text("1"), PrintNode::text("2"), PrintNode::text("3")});

  // When
  std::string result = render(node, RenderOptions{5});

  // Then
  EXPECT_EQ(result, "[\n  1\n  2\n  3\n]");
}

TEST(PrintNode, zero_width_always_breaks_non_empty_groups)
{
  // Given
  PrintNode node = bracketed({PrintNode::text("1")});

  // When
  std::string result = render(node, RenderOptions{0});

  // Then
  EXPECT_EQ(result, "[\n  1\n]");
}

TEST(PrintNode, nested_group_collapses_independently_inside_a_broken_parent)
{
  // Given
  PrintNode inner = bracketed({PrintNode::text("1"), PrintNode::text("2")});
  PrintNode outer =
    bracketed({PrintNode::text("a-long-enough-value-to-force-a-break"), inner});

  // When
  std::string result = render(outer, RenderOptions{20});

  // Then
  EXPECT_EQ(result, "[\n  a-long-enough-value-to-force-a-break\n  [1 2]\n]");
}

TEST(PrintNode, hard_line_forces_its_enclosing_group_to_break)
{
  // Given
  PrintNode node = PrintNode::group(
    PrintNode::concat({PrintNode::text("a"), PrintNode::hard_line(), PrintNode::text("b")}));

  // When
  std::string result = render(node, RenderOptions{80});

  // Then
  EXPECT_EQ(result, "a\nb");
}

TEST(PrintNode, empty_group_fits_even_at_zero_width)
{
  // Given
  PrintNode node = PrintNode::group(PrintNode::concat({}));

  // When
  std::string result = render(node, RenderOptions{0});

  // Then
  EXPECT_EQ(result, "");
}

TEST(PrintNode, nest_zero_width_is_legitimate_and_adds_no_indentation)
{
  // Given: indent width lives on each Nest node itself, not on RenderOptions
  // (see render()'s doc comment) - a zero-width Nest is a valid, if flat,
  // choice, distinct from the width budget forcing a break.
  PrintNode node = bracketed({bracketed({PrintNode::text("1")}, 0)}, 0);

  // When
  std::string result = render(node, RenderOptions{0});

  // Then
  EXPECT_EQ(result, "[\n[\n1\n]\n]");
}
