
#ifndef __RUNTIME_FIXTURE_H_
#define __RUNTIME_FIXTURE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "fake_file_system.h"
#include <gtest/gtest.h>
#include <roo/context.h>
#include <roo/io/dir_root_file_system.h>
#include <roo/io/file_system.h>
#include <roo/lang.h>
#include <roo/runtime.h>

namespace RooTest
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

    RooTest::FakeFileSystem fs;
    Roo::DirRootFileSystem dir_fs;
    Roo::Reader parser;
    Roo::Runtime runtime{&fs};
    Roo::Context ctx{runtime};
    std::unique_ptr<Roo::Runtime> configured_runtime;
    std::unique_ptr<Roo::Context> configured_context;

    Roo::Runtime& use_bare_runtime()
    {
      configured_runtime = std::make_unique<Roo::Runtime>();
      configured_context = std::make_unique<Roo::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Roo::Runtime& use_runtime_with(Roo::FileSystem& file_system)
    {
      configured_runtime = std::make_unique<Roo::Runtime>(&file_system);
      configured_context = std::make_unique<Roo::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Roo::Runtime& use_runtime_with(Roo::Namespace& ns)
    {
      configured_runtime = std::make_unique<Roo::Runtime>(ns);
      configured_context = std::make_unique<Roo::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Roo::Runtime& use_runtime_with(const std::string& current_namespace,
                                   std::map<const std::string, Roo::Namespace> namespaces)
    {
      configured_runtime =
        std::make_unique<Roo::Runtime>(current_namespace, std::move(namespaces));
      configured_context = std::make_unique<Roo::Context>(*configured_runtime);
      return *configured_runtime;
    }

    Roo::Runtime& use_runtime_with(std::vector<std::unique_ptr<Roo::Namespace>> namespaces,
                                   Roo::FileSystem* file_system = nullptr)
    {
      configured_runtime =
        std::make_unique<Roo::Runtime>(std::move(namespaces), file_system);
      configured_context = std::make_unique<Roo::Context>(*configured_runtime);
      return *configured_runtime;
    }
  };

  class RuntimeTestFixture : public RuntimeFixture, public ::testing::Test
  {
  };
} // namespace RooTest

namespace Roo
{
  void PrintTo(const Roo::AST::ASTNode& obj, std::ostream* os);
}

#endif
