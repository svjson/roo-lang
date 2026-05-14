
#ifndef __RUNTIME_FIXTURE_H_
#define __RUNTIME_FIXTURE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <lisple/context.h>
#include <lisple/dir_root_file_system.h>
#include <lisple/file_system.h>
#include <lisple/lang.h>
#include <lisple/runtime.h>

#include "fake_file_system.h"
#include <gtest/gtest.h>

namespace LispleTest
{
  const std::string COMMON_TESTDATA_DIR = "test_resources/script/common";

  class RuntimeFixture
  {
   public:
    RuntimeFixture()
      : dir_fs("test_resources/script")
      , runtime(&fs)
      , ctx(runtime)
    {
    }

    RuntimeFixture(const std::string path)
      : dir_fs(path)
      , runtime(&dir_fs)
      , ctx(runtime)
    {
    }

    LispleTest::FakeFileSystem fs;
    Lisple::DirRootFileSystem dir_fs;
    Lisple::Reader parser;
    Lisple::Runtime runtime{&fs};
    Lisple::Context ctx{runtime};
    std::unique_ptr<Lisple::Runtime> configured_runtime;
    std::unique_ptr<Lisple::Context> configured_context;

    Lisple::Runtime& use_bare_runtime()
    {
      configured_runtime = std::make_unique<Lisple::Runtime>();
      configured_context = std::make_unique<Lisple::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Lisple::Runtime& use_runtime_with(Lisple::FileSystem& file_system)
    {
      configured_runtime = std::make_unique<Lisple::Runtime>(&file_system);
      configured_context = std::make_unique<Lisple::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Lisple::Runtime& use_runtime_with(Lisple::Namespace& ns)
    {
      configured_runtime = std::make_unique<Lisple::Runtime>(ns);
      configured_context = std::make_unique<Lisple::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Lisple::Runtime& use_runtime_with(
      const std::string& current_namespace,
      std::map<const std::string, Lisple::Namespace> namespaces)
    {
      configured_runtime =
        std::make_unique<Lisple::Runtime>(current_namespace, std::move(namespaces));
      configured_context = std::make_unique<Lisple::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Lisple::Runtime& use_runtime_with(
      std::vector<std::unique_ptr<Lisple::Namespace>> namespaces,
      Lisple::FileSystem* file_system = nullptr)
    {
      configured_runtime =
        std::make_unique<Lisple::Runtime>(std::move(namespaces), file_system);
      configured_context = std::make_unique<Lisple::Context>(*configured_runtime);
      return *configured_runtime;
    }
  };

  class RuntimeTestFixture : public RuntimeFixture, public ::testing::Test
  {
  };
} // namespace LispleTest

namespace Lisple
{
  void PrintTo(const Lisple::AST::Object& obj, std::ostream* os);
}

#endif
