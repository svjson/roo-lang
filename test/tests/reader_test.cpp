#include <lisple/form.h>
#include <lisple/reader.h>

#include <gtest/gtest.h>

using namespace Lisple;

TEST(Reader, parses_word_with_colon_in_body)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("window:focus-within");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), Word("window:focus-within"));
}

TEST(Reader, parses_quoted_symbol_with_colon_in_body)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps("'window:focus-within");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), QSymbol("window:focus-within"));
}

TEST(Reader, parses_keyword_with_namespace_and_colon_in_identifier)
{
  // Given
  Reader reader;

  // When
  auto sexps = reader.read_sexps(":ui/menu-item:focus");

  // Then
  ASSERT_EQ(sexps.size(), 1);
  EXPECT_EQ(*sexps.at(0), Key("ui/menu-item:focus"));
}
