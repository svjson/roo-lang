#include <filesystem>
#include <fstream>
#include <string>

#include <lisple/io/dir_root_file_system.h>
#include <lisple/runtime.h>

#include <gtest/gtest.h>
#include <lisple-package/manifest.h>
#include <lisple-package/native_loader.h>
#include <proof/native.h>

namespace
{
  std::filesystem::path fresh_proof_fixture_root(const std::string& name)
  {
    auto root = std::filesystem::temp_directory_path() / "lisple-proof-tests" / name;
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    return root;
  }

  void write_file(const std::filesystem::path& path, const std::string& contents)
  {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path);
    out << contents;
  }
} // namespace

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

TEST(ProofPackage, reports_each_test_result_before_running_the_next_test)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-streaming-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest first-stream-marker
      (prn "first-body"))
    (deftest second-stream-marker
      (prn "second-body"))
  )");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(run)");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(results->to_string(),
            "[{:name first-stream-marker :status :pass} "
            "{:name second-stream-marker :status :pass}]");

  const auto first_body = output.find("first-body\n");
  const auto first_result = output.find("  PASS first-stream-marker\n");
  const auto second_body = output.find("second-body\n");

  ASSERT_NE(first_body, std::string::npos);
  ASSERT_NE(first_result, std::string::npos);
  ASSERT_NE(second_body, std::string::npos);
  EXPECT_LT(first_body, first_result);
  EXPECT_LT(first_result, second_body);
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

TEST(ProofPackage, runner_loads_discovered_files_through_namespace_require)
{
  const auto root = fresh_proof_fixture_root("namespace-require-discovery");
  write_file(root / "test/sample/a-test.lisple",
             R"((ns sample.a-test
  (:require proof.core
            [sample.fixture :as fixture]))

(deftest fixture-loads-once
  (is (= 42 fixture/value)))
)");
  write_file(root / "test/sample/fixture.lisple",
             R"((ns sample.fixture)

(def value 42)
)");

  Lisple::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "test").string()});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-package-test
    (:require proof.runner)))");

  auto results = runtime.eval("(proof.runner/run {:package-root \"" + root.string() +
                              "\" :config {:test-roots [\"test\"]}})");

  EXPECT_EQ(results->to_string(), "[{:name fixture-loads-once :status :pass}]");
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
  EXPECT_EQ(runtime.eval("(pass-count [{:status :pass} {:status :fail}])")->to_string(),
            "1");
  EXPECT_EQ(runtime.eval("(fail-count [{:status :pass} {:status :fail}])")->to_string(),
            "1");
  EXPECT_EQ(runtime.eval(R"((summary-text {:total 2 :passed 1 :failed 1}))")->to_string(),
            R"("proof: 1 passed, 1 failed, 2 total")");
  EXPECT_EQ(runtime.eval(R"((summary-divider {:total 2 :passed 1 :failed 1}))")->to_string(),
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

TEST(ProofPackage, ScenarioFormsPassResultsBetweenPhases)
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
        {:value 4
         :step 1})
      (when [{:keys [value step]}]
        (+ value step))
      (then [result {:keys [value step]}]
        (should (= result 5))
        (should (= value 4))
        (should (= step 1))))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name scenario :status :pass}]");
}

TEST(ProofPackage, ScenarioThenCanOmitGivenResult)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-scenario-omit-given-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest scenario
      (given
        4)
      (when [value]
        (+ value 1))
      (then [result]
        (should (= result 5))))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name scenario :status :pass}]");
}

TEST(ProofPackage, ScenarioWhenAndThenSupportEmptyArgVectors)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-scenario-empty-args-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest scenario
      (given
        4)
      (when []
        5)
      (then []
        (should true)))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name scenario :status :pass}]");
}

TEST(ProofPackage, ScenarioFormsExecuteSurroundingAndInterspersedForms)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-scenario-order-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (def events [])
    (deftest scenario-order
      (append! events :before)
      (given
        (append! events :given)
        4)
      (append! events :after-given)
      (when [value]
        (append! events :when)
        (+ value 1))
      (append! events :after-when)
      (then [result given-value]
        (append! events :then)
        (should (= result 5))
        (should (= given-value 4)))
      (append! events :after))
  )");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(results->to_string(), "[{:name scenario-order :status :pass}]");
  EXPECT_EQ(runtime.eval("events")->to_string(),
            "[:before :given :after-given :when :after-when :then :after]");
}

TEST(ProofPackage, ScenarioFormsRejectTooManyRequiredArguments)
{
  Lisple::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Lisple::Runtime runtime(Lisple::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-scenario-arg-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");

  EXPECT_THROW(runtime.eval(R"(
    (deftest invalid-scenario-args
      (given 4)
      (when [value extra]
        value))
  )"),
               std::exception);
}
