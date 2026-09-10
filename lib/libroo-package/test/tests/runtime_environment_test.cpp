#include <barrier>
#include <exception>
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

TEST(ApplicationRuntime, replays_package_autoloads_in_application_worker)
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
  Roo::Package::ApplicationRuntimeSpec runtime_spec =
    Roo::Package::make_directory_application_runtime_spec(plan);
  Roo::Package::ApplicationRuntime parent(runtime_spec);
  Roo::WorkerEnvironmentFactory environment_factory =
    Roo::Package::make_application_runtime_factory(runtime_spec);

  Roo::Runtime& runtime = parent.runtime();
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

TEST(ApplicationRuntime, serializes_native_activation_per_generation)
{
  Roo::Package::LoadPlan plan;
  plan.native_libraries.push_back({"roo-package-test-native",
                                   "0.1.0",
                                   ROO_PACKAGE_TEST_NATIVE_LIBRARY,
                                   "",
                                   {"package.test.native"}});
  Roo::Package::ApplicationRuntimeSpec runtime_spec =
    Roo::Package::make_directory_application_runtime_spec(plan);
  std::barrier start(3);
  std::exception_ptr first_failure;
  std::exception_ptr second_failure;
  auto construct = [&](std::exception_ptr& failure)
  {
    start.arrive_and_wait();
    try
    {
      Roo::Package::ApplicationRuntime application(runtime_spec);
      application.runtime().eval("(package.test.native/answer nil)");
    }
    catch (...)
    {
      failure = std::current_exception();
    }
  };

  std::thread first(construct, std::ref(first_failure));
  std::thread second(construct, std::ref(second_failure));
  start.arrive_and_wait();
  first.join();
  second.join();

  EXPECT_EQ(first_failure, nullptr);
  EXPECT_EQ(second_failure, nullptr);
}

TEST(ApplicationRuntime, worker_startup_retains_autoload_error_after_native_load)
{
  Roo::Package::LoadPlan plan;
  plan.native_libraries.push_back({"roo-package-test-native",
                                   "0.1.0",
                                   ROO_PACKAGE_TEST_NATIVE_LIBRARY,
                                   "",
                                   {"package.test.native"}});
  plan.autoloads.push_back("missing.autoload");
  Roo::WorkerEnvironmentFactory environment_factory =
    Roo::Package::make_application_runtime_factory(
      Roo::Package::make_directory_application_runtime_spec(plan));

  try
  {
    environment_factory();
    FAIL() << "Expected application worker construction to fail.";
  }
  catch (const Roo::RooException& e)
  {
    EXPECT_NE(std::string(e.what()).find("missing.autoload"), std::string::npos);
  }
}
