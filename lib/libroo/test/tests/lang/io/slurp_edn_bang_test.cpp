#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SlurpEdnBang = RooTest::RuntimeTestFixture;

TEST_F(SlurpEdnBang, reads_single_data_form)
{
  fs.add_file("data.edn", "{:name \"Roo\" :numbers [1 2 3] :ok true}");

  EXPECT_EQ(runtime.eval(R"((roo.io/slurp-edn! "data.edn"))")->to_string(),
            R"({:name "Roo" :numbers [1 2 3] :ok true})");
}

TEST_F(SlurpEdnBang, reads_true_and_false_as_booleans)
{
  fs.add_file("data.edn", "{:enabled? true :disabled? false}");

  EXPECT_EQ(
    runtime.eval(R"((type-of (:enabled? (roo.io/slurp-edn! "data.edn"))))")->to_string(),
    R"("boolean")");
  EXPECT_EQ(
    runtime.eval(R"((type-of (:disabled? (roo.io/slurp-edn! "data.edn"))))")->to_string(),
    R"("boolean")");
  EXPECT_EQ(
    runtime.eval(R"((not (:disabled? (roo.io/slurp-edn! "data.edn"))))")->to_string(),
    "true");
}

TEST_F(SlurpEdnBang, reads_nil_as_nil)
{
  fs.add_file("data.edn", "{:value nil}");

  EXPECT_EQ(
    runtime.eval(R"((type-of (:value (roo.io/slurp-edn! "data.edn"))))")->to_string(),
    R"("nil")");
  EXPECT_EQ(runtime.eval(R"((nil? (:value (roo.io/slurp-edn! "data.edn"))))")->to_string(),
            "true");
}

TEST_F(SlurpEdnBang, keeps_other_symbols_as_symbols)
{
  fs.add_file("data.edn", "{:status pending}");

  EXPECT_EQ(
    runtime.eval(R"((type-of (:status (roo.io/slurp-edn! "data.edn"))))")->to_string(),
    R"("symbol")");
  EXPECT_EQ(runtime.eval(R"((str (:status (roo.io/slurp-edn! "data.edn"))))")->to_string(),
            R"("pending")");
}
