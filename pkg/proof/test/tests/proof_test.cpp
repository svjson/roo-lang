#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>
#include <roo/runtime/dict.h>

#include <gtest/gtest.h>
#include <proof/native.h>
#include <roo-package/application.h>
#include <roo-package/manifest.h>
#include <roo-package/native_loader.h>

namespace
{
  std::filesystem::path fresh_proof_fixture_root(const std::string& name)
  {
    auto root = std::filesystem::temp_directory_path() / "roo-proof-tests" / name;
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

  std::string roo_string(const std::filesystem::path& path)
  {
    return Roo::Package::Application::source_string_literal(path.string());
  }

  bool elapsed_ms_key(const Roo::sptr_val& value)
  {
    return value && value->type == Roo::Value::Type::KEYWORD && value->str() == "elapsed-ms";
  }

  Roo::sptr_val without_elapsed_ms(const Roo::sptr_val& value)
  {
    if (!value)
    {
      return value;
    }

    if (value->type == Roo::Value::Type::VECTOR)
    {
      Roo::sptr_val_v elements;
      for (const auto& element : value->elements())
      {
        elements.push_back(without_elapsed_ms(element));
      }
      return Roo::Value::vector(elements);
    }

    if (value->type == Roo::Value::Type::LIST)
    {
      Roo::sptr_val_v elements;
      for (const auto& element : value->elements())
      {
        elements.push_back(without_elapsed_ms(element));
      }
      return Roo::Value::list(elements);
    }

    if (value->type == Roo::Value::Type::MAP)
    {
      Roo::sptr_val_v elements;
      const auto& map_elements = value->elements();
      for (size_t i = 0; i < map_elements.size(); i += 2)
      {
        if (elapsed_ms_key(map_elements[i]))
        {
          continue;
        }
        elements.push_back(map_elements[i]);
        if (i + 1 < map_elements.size())
        {
          elements.push_back(without_elapsed_ms(map_elements[i + 1]));
        }
      }
      return Roo::Value::map(elements);
    }

    return value;
  }

  std::string without_elapsed_ms_string(const Roo::sptr_val& value)
  {
    return without_elapsed_ms(value)->to_string();
  }

  void expect_elapsed_ms(const Roo::sptr_val& result)
  {
    auto [found, elapsed] =
      Roo::Dict::find_property(result, Roo::Value::keyword("elapsed-ms"));
    ASSERT_TRUE(found);
    ASSERT_NE(elapsed, nullptr);
    ASSERT_EQ(elapsed->type, Roo::Value::Type::NUMBER);
    EXPECT_GE(elapsed->num().get_double(), 0);
  }

  void expect_elapsed_ms_for_results(const Roo::sptr_val& results)
  {
    ASSERT_NE(results, nullptr);
    ASSERT_EQ(results->type, Roo::Value::Type::VECTOR);
    for (const auto& result : results->elements())
    {
      expect_elapsed_ms(result);
    }
  }

  void expect_duration_output_line(const std::string& output, const std::string& prefix)
  {
    const auto line_start = output.find(prefix + " (");
    ASSERT_NE(line_start, std::string::npos) << output;

    const auto line_end = output.find('\n', line_start);
    ASSERT_NE(line_end, std::string::npos) << output;

    const auto timing_end = output.find("ms)", line_start);
    ASSERT_NE(timing_end, std::string::npos) << output;
    EXPECT_LT(timing_end, line_end);
  }
} // namespace

TEST(ProofPackage, reports_each_test_result_before_running_the_next_test)
{
  Roo::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

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

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name first-stream-marker :status :pass} "
            "{:name second-stream-marker :status :pass}]");
  expect_elapsed_ms_for_results(results);

  const auto first_body = output.find("first-body\n");
  const auto first_result = output.find("  PASS first-stream-marker\n");
  const auto second_body = output.find("second-body\n");

  ASSERT_NE(first_body, std::string::npos);
  ASSERT_NE(first_result, std::string::npos);
  ASSERT_NE(second_body, std::string::npos);
  EXPECT_LT(first_body, first_result);
  EXPECT_LT(first_result, second_body);
}

TEST(ProofPackage, simple_reporter_prints_duration_when_requested)
{
  Roo::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-simple-durations-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest duration-pass
      (is true))
    (deftest duration-fail
      (is false))
    (deftest duration-error
      (/ 1 0))
  )");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(run-selected {:durations? true})");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name duration-pass :status :pass} "
            "{:name duration-fail :status :fail "
            ":message \"Expected truthy expression: false.\" "
            ":failures [{:message \"Expected truthy expression: false.\"}]} "
            "{:name duration-error :status :error :message \"Division by zero\"}]");
  expect_elapsed_ms_for_results(results);
  expect_duration_output_line(output, "  PASS duration-pass");
  expect_duration_output_line(output, "  FAIL duration-fail");
  expect_duration_output_line(output, "  ERROR duration-error");
}

TEST(ProofPackage, dynamically_loads_native_syntax_from_package_manifest)
{
  Roo::DirRootFileSystem manifest_fs("/");
  auto plan = Roo::Package::resolve_load_plan(manifest_fs, PROOF_PACKAGE_DIR);

  auto fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Package::LoadedNativePackages native_packages;
  Roo::Runtime runtime(fs.get());
  native_packages = Roo::Package::load_native_libraries(runtime, plan);

  runtime.eval(R"(
    (ns proof.package-dynamic-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval("(deftest dynamic-addition (is (= 4 (+ 2 2))))");

  auto results = runtime.eval("(run)");

  EXPECT_EQ(without_elapsed_ms_string(results), "[{:name dynamic-addition :status :pass}]");
  expect_elapsed_ms_for_results(results);
}

TEST(ProofPackage, runner_loads_discovered_files_through_namespace_require)
{
  const auto root = fresh_proof_fixture_root("namespace-require-discovery");
  write_file(root / "test/sample/a-test.roo",
             R"((ns sample.a-test
  (:require proof.core
            [sample.fixture :as fixture]))

(deftest fixture-loads-once
  (is (= 42 fixture/value)))
)");
  write_file(root / "test/sample/fixture.roo",
             R"((ns sample.fixture)

(def value 42)
)");

  Roo::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "test").string()});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-package-test
    (:require proof.runner)))");

  auto results = runtime.eval("(proof.runner/run {:package-root " + roo_string(root) +
                              " :config {:test-roots [\"test\"]}})");

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name fixture-loads-once :status :pass}]");
  expect_elapsed_ms_for_results(results);
}

TEST(ProofPackage, runner_loads_all_discovered_namespaces_before_filtering)
{
  const auto root = fresh_proof_fixture_root("filter-after-load");
  write_file(root / "test/app/checkout-test.roo",
             R"((ns app.checkout-test
  (:require proof.core))

(deftest checkout-total
  (is true))
)");
  write_file(root / "test/app/admin-test.roo",
             R"((ns app.admin-test
  (:require proof.core))

(def loaded? true)

(deftest hidden-failure
  (is false))
)");

  Roo::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "test").string()});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-filter-test
    (:require proof.runner)))");

  auto results = runtime.eval("(proof.runner/run {:package-root " + roo_string(root) +
                              " :config {:test-roots [\"test\"] "
                              ":namespace \"app.checkout-test\"}})");

  EXPECT_EQ(without_elapsed_ms_string(results), "[{:name checkout-total :status :pass}]");
  EXPECT_EQ(runtime.eval("app.admin-test/loaded?")->to_string(), "true");
}

TEST(ProofPackage, runner_supports_tree_reporter_grouped_by_test_file)
{
  const auto root = fresh_proof_fixture_root("tree-reporter");
  write_file(root / "test/app/checkout-test.roo",
             R"((ns app.checkout-test
  (:require proof.core))

(deftest checkout-total
  (is true))

(deftest checkout-discount
  (is (= 5 (+ 2 2))))
)");
  write_file(root / "test/app/profile-test.roo",
             R"((ns app.profile-test
  (:require proof.core))

(deftest profile-page
  (is true))
)");

  Roo::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "test").string()});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-tree-reporter-test
    (:require proof.runner)))");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(proof.runner/run {:package-root " + roo_string(root) +
                              " :config {:test-roots [\"test\"] "
                              ":reporter :tree}})");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name checkout-total :status :pass} "
            "{:name checkout-discount :status :fail :message \"Expected 5, got 4.\" "
            ":failures [{:message \"Expected 5, got 4.\"}]} "
            "{:name profile-page :status :pass}]");
  EXPECT_NE(output.find("test/app/checkout-test.roo\n"
                        "├── PASS - checkout-total\n"
                        "└── FAIL - checkout-discount\n"
                        "    - Expected 5, got 4.\n"),
            std::string::npos);
  EXPECT_NE(output.find("test/app/profile-test.roo\n"
                        "└── PASS - profile-page\n"),
            std::string::npos);
}

TEST(ProofPackage, tree_reporter_prints_duration_when_requested)
{
  Roo::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-tree-durations-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest duration-pass
      (is true))
    (deftest duration-fail
      (is false))
    (deftest duration-error
      (/ 1 0))
  )");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(run-selected {:reporter :tree :durations? true})");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name duration-pass :status :pass} "
            "{:name duration-fail :status :fail "
            ":message \"Expected truthy expression: false.\" "
            ":failures [{:message \"Expected truthy expression: false.\"}]} "
            "{:name duration-error :status :error :message \"Division by zero\"}]");
  expect_elapsed_ms_for_results(results);
  EXPECT_NE(output.find("proof.package-tree-durations-test\n"), std::string::npos);
  expect_duration_output_line(output, "├── PASS - duration-pass");
  expect_duration_output_line(output, "├── FAIL - duration-fail");
  expect_duration_output_line(output, "└── ERROR - duration-error");
}

TEST(ProofPackage, runner_merges_cli_args_over_package_config)
{
  const auto root = fresh_proof_fixture_root("cli-config");
  write_file(root / "spec/app/checkout-test.roo",
             R"((ns app.checkout-test
  (:require proof.core))

(deftest checkout-total
  (is true))

(deftest checkout-discount
  (is true))
)");
  write_file(root / "test/app/ignored-test.roo",
             R"((ns app.ignored-test
  (:require proof.core))

(deftest ignored-failure
  (is false))
)");

  Roo::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "spec").string()});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-cli-args-test
    (:require proof.runner)))");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(proof.runner/run {:package-root " + roo_string(root) +
                              " :config {:test-roots [\"test\"] "
                              ":filter \"ignored*\" "
                              ":durations? true} "
                              ":args [\"--test-root\" \"spec\" "
                              "\"--filter=checkout-discount\" "
                              "\"--reporter\" \"tree\" "
                              "\"--no-durations\"]})");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results), "[{:name checkout-discount :status :pass}]");
  EXPECT_NE(output.find("spec/app/checkout-test.roo\n"
                        "└── PASS - checkout-discount\n"),
            std::string::npos);
  EXPECT_EQ(output.find("checkout-discount ("), std::string::npos);
}

TEST(ProofPackage, runner_prints_help_without_loading_tests)
{
  const auto root = fresh_proof_fixture_root("runner-help");
  write_file(root / "test/app/failing-test.roo",
             R"((ns app.failing-test
  (:require proof.core))

(deftest should-not-run
  (is false))
)");

  Roo::DirRootFileSystem fs(
    {std::string(PROOF_PACKAGE_DIR) + "/src", (root / "test").string()});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"((ns proof.runner-help-test
    (:require proof.runner)))");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(proof.runner/run {:package-root " + roo_string(root) +
                              " :config {:test-roots [\"test\"]} "
                              ":args [\"--help\"]})");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results), "nil");
  EXPECT_NE(output.find("Usage: roo proof [options]\n"), std::string::npos);
  EXPECT_NE(output.find("--reporter simple|tree"), std::string::npos);
  EXPECT_NE(output.find("--durations"), std::string::npos);
  EXPECT_EQ(output.find("should-not-run"), std::string::npos);
  EXPECT_EQ(output.find("FAIL"), std::string::npos);
}

TEST(ProofPackage, package_tool_forwards_cli_args_to_proof)
{
  const auto root = std::filesystem::path(PROOF_PACKAGE_DIR) / "test/assets/dynamic-smoke";

  Roo::DirRootFileSystem manifest_fs("/");
  auto plan = Roo::Package::resolve_load_plan(manifest_fs, root.string());

  auto fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Package::LoadedNativePackages native_packages;
  Roo::Runtime runtime(fs.get());
  native_packages = Roo::Package::load_native_libraries(runtime, plan);
  Roo::Package::load_autoloads(runtime, plan);

  testing::internal::CaptureStdout();
  auto results = Roo::Package::Application::invoke_tool(
    runtime,
    plan,
    "proof",
    "run",
    {"--reporter=tree", "--durations", "--filter", "discovered-proof"});
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results), "[{:name discovered-proof :status :pass}]");
  EXPECT_NE(output.find("test/smoke/discovered.roo\n"), std::string::npos);
  expect_duration_output_line(output, "└── PASS - discovered-proof");
}

TEST(ProofPackage, runtime_error_marks_test_error_and_continues)
{
  Roo::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

  runtime.eval(R"(
    (ns proof.package-error-test
      (:require proof.core))
  )");
  runtime.eval("(clear!)");
  runtime.eval(R"(
    (deftest division-error
      (/ 1 0))
    (deftest after-error
      (is true))
  )");

  testing::internal::CaptureStdout();
  auto results = runtime.eval("(run)");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(without_elapsed_ms_string(results),
            "[{:name division-error :status :error :message \"Division by zero\"} "
            "{:name after-error :status :pass}]");
  EXPECT_NE(output.find("  ERROR division-error\n"
                        "    - Division by zero\n"),
            std::string::npos);
  EXPECT_NE(output.find("  PASS after-error\n"), std::string::npos);
  EXPECT_NE(output.find("proof: 1 passed, 0 failed, 1 errored, 2 total"), std::string::npos);
}

TEST(ProofPackage, ScenarioFormsRejectTooManyRequiredArguments)
{
  Roo::DirRootFileSystem fs({std::string(PROOF_PACKAGE_DIR) + "/src"});
  Roo::Runtime runtime(Roo::Proof::make_native_namespaces(), &fs);

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
