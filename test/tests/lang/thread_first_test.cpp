
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(ThreadFirstForm, deep_map_traversal)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto retval = runtime.eval("(-> deep_map :file :metadata :size :mb)");

  // Then
  ASSERT_EQ(*retval, *Lisple::RTValue::number(1200));
}

TEST(ThreadFirstForm, functions)
{
  // Given
  Lisple::Runtime runtime;

  // When
  auto retval = runtime.eval("(-> 1 (* 2) (* 16) (/ 4))");

  // Then
  ASSERT_EQ(retval->i64(), 8);
}

TEST(ThreadFirstForm, retrieved_map_is_same_instance)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval("(def deep_map {:file {:metadata {:size {:mb 1200}}}})");

  // When
  auto tf_size = runtime.eval("(-> deep_map :file :metadata :size)");
  auto tf_fn_size = runtime.eval("(-> deep_map (get :file) :metadata (get :size))");
  auto size = runtime.eval("(:size (:metadata (:file deep_map)))");

  // Then
  EXPECT_EQ(*tf_size, *size);
  EXPECT_EQ(*tf_fn_size, *size);

  // Temporarily cater to RuntimeWrapper
  EXPECT_EQ(tf_size.get(), size.get());

  EXPECT_EQ(tf_size->to_string(), size->to_string());

  EXPECT_EQ(tf_fn_size.get(), size.get());
  EXPECT_EQ(tf_fn_size->to_string(), size->to_string());
}
