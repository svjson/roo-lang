#include <roo/context.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/lang.h>
#include <roo/namespace.h>
#include <roo/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ReduceKeyValueFunction = RooTest::RuntimeTestFixture;
/*
 * ======================================================================
 * ReduceKeyValueFunction - (reduce-kv [...] reducer-fn)
 * ======================================================================
 */

TEST_F(ReduceKeyValueFunction, recreate_map)
{
  // Given
  runtime.eval("(def original-map {:a [1 2 3 4 5] :b [1 2 3]})");

  // when
  auto retval =
    runtime.eval("(reduce-kv original-map {} (fn [r k v] (assoc r k (count v))))");

  // Then
  EXPECT_EQ(*retval, *runtime.eval("{:a 5 :b 3}"));
}

TEST_F(ReduceKeyValueFunction, accepts_function_first_arg_order)
{
  auto retval =
    runtime.eval("(reduce-kv (fn [r k v] (assoc r k (count v))) {:a [1 2] :b [1 2 3]} {})");

  EXPECT_EQ(*retval, *runtime.eval("{:a 2 :b 3}"));
}

TEST_F(ReduceKeyValueFunction, treats_nil_as_map_in_map_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(reduce-kv nil {:seen false} (fn [r k v] (assoc r k v)))"),
            *runtime.eval("{:seen false}"));
}

TEST_F(ReduceKeyValueFunction, treats_nil_as_map_in_function_first_arg_order)
{
  EXPECT_EQ(*runtime.eval("(reduce-kv (fn [r k v] (assoc r k v)) nil {:seen false})"),
            *runtime.eval("{:seen false}"));
}
