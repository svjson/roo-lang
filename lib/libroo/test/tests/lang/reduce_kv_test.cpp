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
