#include "runtime_fixture.h"

#include <gtest/gtest.h>

using SlurpEdnBang = LispleTest::RuntimeTestFixture;

TEST_F(SlurpEdnBang, reads_single_data_form)
{
  fs.add_file("data.edn", "{:name \"Lisple\" :numbers [1 2 3] :ok true}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/slurp-edn! "data.edn"))")->to_string(),
            R"({:name "Lisple" :numbers [1 2 3] :ok true})");
}

TEST_F(SlurpEdnBang, reads_true_and_false_as_booleans)
{
  fs.add_file("data.edn", "{:enabled? true :disabled? false}");

  EXPECT_EQ(runtime.eval(R"((type-of (:enabled? (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            R"("boolean")");
  EXPECT_EQ(runtime.eval(R"((type-of (:disabled? (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            R"("boolean")");
  EXPECT_EQ(runtime.eval(R"((not (:disabled? (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            "true");
}

TEST_F(SlurpEdnBang, reads_nil_as_nil)
{
  fs.add_file("data.edn", "{:value nil}");

  EXPECT_EQ(runtime.eval(R"((type-of (:value (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            R"("nil")");
  EXPECT_EQ(runtime.eval(R"((nil? (:value (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            "true");
}

TEST_F(SlurpEdnBang, keeps_other_symbols_as_symbols)
{
  fs.add_file("data.edn", "{:status pending}");

  EXPECT_EQ(runtime.eval(R"((type-of (:status (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            R"("symbol")");
  EXPECT_EQ(runtime.eval(R"((str (:status (lisple.io/slurp-edn! "data.edn"))))")
              ->to_string(),
            R"("pending")");
}
