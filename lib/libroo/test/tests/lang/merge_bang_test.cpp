#include "runtime_fixture.h"
#include <gtest/gtest.h>

using MergeBangFunction = RooTest::RuntimeTestFixture;

TEST_F(MergeBangFunction, MutatesTargetAndReturnsIt)
{
  runtime.eval("(def target {:a 1 :b 2})");

  auto result = runtime.eval("(merge! target {:b 3 :c 4})");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 3 :c 4}"));
}

TEST_F(MergeBangFunction, MergesSourcesBeforeMutatingTarget)
{
  runtime.eval("(def target {:a 1 :b 2})");

  auto result = runtime.eval("(merge! target {:b 3 :c 4} {:c 5 :d 6})");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 3 :c 5 :d 6}"));
}

TEST_F(MergeBangFunction, CreatesMapForNilTarget)
{
  auto result = runtime.eval("(merge! nil {:a 1} {:b 2})");

  EXPECT_EQ(result->type, Roo::Value::Type::MAP);
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2}"));
}

TEST_F(MergeBangFunction, IgnoresNilSources)
{
  runtime.eval("(def target {:a 1})");

  auto result = runtime.eval("(merge! target nil {:b 2} nil)");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 2}"));
}

TEST_F(MergeBangFunction, ReadsAliasedSourceBeforeMutatingTarget)
{
  runtime.eval("(def target {:a 1})");

  auto result = runtime.eval("(merge! target {:a 2 :b 3} target)");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1 :b 3}"));
}

TEST_F(MergeBangFunction, ReturnsSoleTarget)
{
  runtime.eval("(def target {:a 1})");

  auto result = runtime.eval("(merge! target)");

  EXPECT_EQ(result, runtime.lookup("target"));
  EXPECT_EQ(*result, *runtime.eval("{:a 1}"));
}
