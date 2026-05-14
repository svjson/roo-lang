#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include "runtime_fixture.h"
#include <lisple/type.h>

#include <gtest/gtest.h>


using ReduceKeyValueFunction = LispleTest::RuntimeTestFixture;
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
