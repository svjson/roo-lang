#include <filesystem>
#include <memory>
#include <thread>

#include <roo/io/dir_root_file_system.h>
#include <roo/runtime.h>
#include <roo/runtime/worker.h>

#include <gtest/gtest.h>
#include <roo-package/manifest.h>
#include <roo-package/runtime_environment.h>

namespace
{
  void wait_until_terminal(const Roo::WorkerRegistry& registry,
                           const Roo::sptr_val& execution)
  {
    while (true)
    {
      const Roo::WorkerExecutionStatus status = registry.poll(execution);
      if (status == Roo::WorkerExecutionStatus::SUCCEEDED ||
          status == Roo::WorkerExecutionStatus::FAILED)
      {
        return;
      }
      std::this_thread::yield();
    }
  }
} // namespace

TEST(ApplicationRuntimeEnvironment, replays_package_autoloads_in_application_worker)
{
  const std::filesystem::path package_root =
    std::filesystem::path(ROO_PACKAGE_TEST_DIR) / "tests/assets/packages/autoload-app";
  Roo::DirRootFileSystem manifest_fs("/");
  Roo::Package::LoadPlan plan =
    Roo::Package::resolve_load_plan(manifest_fs, package_root.string());
  plan.native_libraries.push_back({"roo-package-test-native",
                                   "0.1.0",
                                   ROO_PACKAGE_TEST_NATIVE_LIBRARY,
                                   package_root.string(),
                                   {"package.test.native"}});
  Roo::WorkerEnvironmentFactory environment_factory =
    Roo::Package::make_application_runtime_factory(
      Roo::Package::make_directory_application_runtime_spec(plan));

  std::unique_ptr<Roo::WorkerEnvironment> parent_environment = environment_factory();
  Roo::Runtime& runtime = parent_environment->runtime();
  runtime.worker_registry().register_environment("application", environment_factory);

  ASSERT_EQ(runtime.ns("autoload.worker-only"), nullptr);
  runtime.eval(R"(
    (roo.worker/create! :test/worker
      {:environment :application
       :autoloads ["autoload.worker-only"]})
  )");
  EXPECT_EQ(runtime.ns("autoload.worker-only"), nullptr);
  Roo::sptr_val execution = runtime.eval(R"(
    (roo.worker/execute-let! :test/worker []
      [autoload.bootstrap/value
       autoload.worker-only/value
       (roo.io/slurp! "autoload/bootstrap.roo")
       (package.test.native/answer nil)])
  )");
  wait_until_terminal(runtime.worker_registry(), execution);

  Roo::sptr_val result = runtime.worker_registry().collect(execution);
  ASSERT_EQ(result->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(result->elements().size(), 4u);
  EXPECT_EQ(*result->elements()[0], *Roo::Value::number(42));
  EXPECT_EQ(*result->elements()[1], *Roo::Value::number(84));
  EXPECT_NE(result->elements()[2]->str().find("(def value 42)"), std::string::npos);
  EXPECT_EQ(*result->elements()[3], *Roo::Value::number(42));
}
