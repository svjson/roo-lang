#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>
#include <lisple-package/manifest.h>
#include <lisple-package/native_loader.h>

#include <gtest/gtest.h>
#include <proof/native.h>

TEST(ProofPackage, registers_and_runs_tests_from_load_path)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval("(deftest addition (is (= 4 (+ 2 2))))");
  runtime.eval("(deftest should-alias (should true))");
  runtime.eval("(deftest assert-alias (assert true))");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(),
            "[{:name addition :status :pass} {:name should-alias :status :pass} "
            "{:name assert-alias :status :pass}]");
}

TEST(ProofPackage, dynamically_loads_native_syntax_from_package_manifest)
{
  Lisple::DirRootFileSystem manifest_fs("/");
  auto plan = Lisple::Package::resolve_load_plan(manifest_fs, PROOF_PACKAGE_DIR);

  auto fs = Lisple::Package::make_load_path_file_system(plan);
  Lisple::Package::LoadedNativePackages native_packages;
  Lisple::Runtime runtime(fs.get());
  native_packages = Lisple::Package::load_native_libraries(runtime, plan);

  runtime.eval(R"(
    (ns proof.package-dynamic-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval("(deftest dynamic-addition (is (= 4 (+ 2 2))))");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name dynamic-addition :status :pass}]");
}

TEST(ProofPackage, summarizes_result_sets_in_lisple)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-summary-test
      (:require proof.core))
  )");

  auto summary = runtime.eval(R"(
    (result-summary
      [{:name :passing-test :status :pass}
       {:name :failing-test
        :status :fail
        :message "Expected true."
        :failures [{:message "Expected true."}]}])
  )");

  EXPECT_EQ(summary->to_string(), "{:total 2 :passed 1 :failed 1}");
  EXPECT_EQ(runtime.eval("(pass-count [{:status :pass} {:status :fail}])")
              ->to_string(),
            "1");
  EXPECT_EQ(runtime.eval("(fail-count [{:status :pass} {:status :fail}])")
              ->to_string(),
            "1");
  EXPECT_EQ(runtime.eval(R"((summary-text {:total 2 :passed 1 :failed 1}))")
              ->to_string(),
            R"("proof: 1 passed, 1 failed, 2 total")");
  EXPECT_EQ(runtime.eval(R"((summary-divider {:total 2 :passed 1 :failed 1}))")
              ->to_string(),
            R"("----------------------------------")");
}

TEST(ProofPackage, returns_failure_results)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-failure-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval("(deftest bad-math (is (= 5 (+ 2 2))))");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(),
            "[{:name bad-math :status :fail :message \"Expected 5, got 4.\" "
            ":failures [{:message \"Expected 5, got 4.\"}]}]");
}

TEST(ProofPackage, records_failed_assertions_before_later_passing_expressions)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-hidden-failure-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest hidden-failure
      (is (= 1 2))
      (expect (= 3 4)))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(),
            "[{:name hidden-failure :status :fail :message \"Expected 1, got 2.\" "
            ":failures [{:message \"Expected 1, got 2.\"}]}]");
}

TEST(ProofPackage, expect_records_failures_and_continues)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-expect-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest multiple-expectations
      (expect (= 1 2))
      (expect (= 3 4)))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(),
            "[{:name multiple-expectations :status :fail "
            ":message \"Expected 1, got 2.\" "
            ":failures [{:message \"Expected 1, got 2.\"} "
            "{:message \"Expected 3, got 4.\"}]}]");
}

TEST(ProofPackage, scenario_forms_execute_ordinary_lisple_code)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-scenario-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest scenario
      (given
        (def value 4))
      (when
        (set! [value] (+ value 1)))
      (then
        (should (= value 5))))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name scenario :status :pass}]");
}
