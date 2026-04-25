
#include "lisple/runtime/value.h"

#include <map>
#include <string>

#include <lisple/exception.h>
#include <lisple/file_system.h>
#include <lisple/file_system_namespace_source.h>
#include <lisple/namespace_source.h>
#include <lisple/runtime.h>

#include "host/test_adapters/vehicle_native_adapters.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace
{
  class InMemoryFileSystem : public Lisple::FileSystem
  {
    std::map<std::string, std::string> files_;

   public:
    void add(const std::string& name, const std::string& content) { files_[name] = content; }

    const std::string read_file_to_string(const std::string& name) override
    {
      auto it = files_.find(name);
      if (it == files_.end())
      {
        throw Lisple::LispleException("File not found: " + name);
      }
      return it->second;
    }
  };
} // namespace

TEST(FileSystemNamespaceSource, single_segment_maps_to_filename)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.lisple", "(ns foo)");
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns foo)");
}

TEST(FileSystemNamespaceSource, dotted_namespace_maps_to_directory_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo/bar/baz.lisple", "(ns foo.bar.baz)");
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo.bar.baz", {});

  // Then
  ASSERT_TRUE(result.has_value());
}

TEST(FileSystemNamespaceSource, dashes_are_preserved_in_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my-module/some-util.lisple", "(ns my-module.some-util)");
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("my-module.some-util", {});

  // Then
  ASSERT_TRUE(result.has_value());
}

TEST(FileSystemNamespaceSource, falls_back_to_lspl_extension)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.lspl", "(ns foo)");
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns foo)");
}

TEST(FileSystemNamespaceSource, lisple_extension_takes_priority_over_lspl)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.lisple", "lisple-content");
  fs.add("foo.lspl", "lspl-content");
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "lisple-content");
}

TEST(FileSystemNamespaceSource, returns_nullopt_when_no_file_found)
{
  // Given
  InMemoryFileSystem fs;
  Lisple::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("does.not.exist", {});

  // Then
  EXPECT_FALSE(result.has_value());
}

/** FileSystemNamespaceSource - inferred path resolution */

TEST(FileSystemNamespaceSource,
     infer_path_strips_package_root_when_loading_from_flat_project_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("models.lisple", "(ns my.app.models)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.lisple";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models)");
  EXPECT_EQ(result->resolved_path, "models.lisple");
}

TEST(FileSystemNamespaceSource, infer_path_strips_package_root_with_one_directory_level)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("models/user.lisple", "(ns my.app.models.user)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.controllers.users";
  ctx.current_source_path = "controllers/users.lisple";

  // When
  auto result = source.fetch("my.app.models.user", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models.user)");
  EXPECT_EQ(result->resolved_path, "models/user.lisple");
}

TEST(FileSystemNamespaceSource, infer_path_falls_back_to_full_path_when_short_path_missing)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/app/models.lisple", "(ns my.app.models)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.lisple";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models)");
  EXPECT_EQ(result->resolved_path, "my/app/models.lisple");
}

TEST(FileSystemNamespaceSource, infer_path_infers_root_dir_from_file_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app/models.lisple", "(ns my.app.models)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app";
  ctx.current_source_path = "lisp/app.lisple";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "lisp/app/models.lisple");
}

TEST(FileSystemNamespaceSource, infer_path_does_not_apply_across_different_root_packages)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("other/lib.lisple", "(ns other.lib)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.lisple";

  // When
  auto result = source.fetch("other.lib", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "other/lib.lisple");
}

/** NamespaceLoading - integration */

TEST(NamespaceLoading, loads_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
  Lisple::Runtime runtime(&fs);

  // When
  runtime.eval("(ns my.app (:require my.utils))");

  // Then
  auto result = runtime.eval("(double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST(NamespaceLoading, loads_aliased_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
  Lisple::Runtime runtime(&fs);

  // When
  runtime.eval("(ns my.app (:require [my.utils :as utils]))");

  // Then
  auto result = runtime.eval("(utils/double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST(NamespaceLoading, loads_transitive_requirements)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("base.lisple", "(ns base) (defun add [a b] (+ a b))");
  fs.add("mid.lisple", "(ns mid (:require base)) (defun double [n] (add n n))");
  Lisple::Runtime runtime(&fs);

  // When
  runtime.eval("(ns top (:require mid))");

  // Then
  auto result = runtime.eval("(double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST(NamespaceLoading, throws_on_direct_cyclic_dependency)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("ns-a.lisple", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.lisple", "(ns ns-b (:require ns-a))");
  Lisple::Runtime runtime(&fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Lisple::CyclicNamespaceException);
}

TEST(NamespaceLoading, throws_on_indirect_cyclic_dependency)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("ns-a.lisple", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.lisple", "(ns ns-b (:require ns-c))");
  fs.add("ns-c.lisple", "(ns ns-c (:require ns-a))");
  Lisple::Runtime runtime(&fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Lisple::CyclicNamespaceException);
}

TEST(NamespaceLoading, does_not_load_already_loaded_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
  Lisple::Runtime runtime(&fs);
  runtime.eval("(ns my.utils) (defun double [n] (+ n n n))"); // triple, not double

  // When - require the same namespace that is already loaded
  runtime.eval("(ns my.app (:require my.utils))");

  // Then - the pre-loaded version (triple) is used, not the file version
  auto result = runtime.eval("(double 10)");
  EXPECT_EQ(result->to_string(), "30");
}

TEST(NamespaceLoading, throws_when_no_filesystem_and_namespace_missing)
{
  // Given
  Lisple::Runtime runtime; // no filesystem

  // Then
  std::string message;
  try
  {
    runtime.eval("(ns my.app (:require my.utils))");
  }
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST(NamespaceLoading, throws_when_file_not_found_on_filesystem)
{
  // Given
  InMemoryFileSystem fs; // empty - no files
  Lisple::Runtime runtime(&fs);

  // Then
  std::string message;
  try
  {
    runtime.eval("(ns my.app (:require my.utils))");
  }
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST(NamespaceLoading, resolves_require_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.lisple",
         "(ns my.app (:require my.app.models)) (defun run [] (make-thing 7))");
  fs.add("lisp/app/models.lisple", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  Lisple::Runtime runtime(&fs);

  // When
  runtime.read_file("lisp/app.lisple");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST(NamespaceLoading, resolves_require_alias_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.lisple",
         "(ns my.app (:require [my.app.models :as m])) (defun run [] (m/make-thing 7))");
  fs.add("lisp/app/models.lisple", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  Lisple::Runtime runtime(&fs);

  // When
  runtime.read_file("lisp/app.lisple");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST(NamespaceLoading, load_root_resolves_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple", "(ns app.core (:require vehicle))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("app.core/make-vehicle");

  // Then
  ASSERT_EQ(result->type, Lisple::RTValue::Type::FUNCTION);
}

TEST(NamespaceLoading, load_root_imports_aliased_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple",
         "(ns app.core (:require [vehicle :as v])) (defun mkv [] (v/make-vehicle-model "
         "{:model-name \"FF2\" :seats 8}))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}
