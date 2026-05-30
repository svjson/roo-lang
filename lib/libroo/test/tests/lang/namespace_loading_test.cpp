
#include <map>
#include <string>

#include "host/test_adapters/vehicle_native_adapters.h"
#include "roo/runtime/value.h"
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <roo/exception.h>
#include <roo/io/file_system.h>
#include <roo/io/file_system_namespace_source.h>
#include <roo/namespace_source.h>

using FileSystemNamespaceSource = RooTest::RuntimeTestFixture;
using NamespaceLoading = RooTest::RuntimeTestFixture;
using namespace ::testing;

namespace
{
  std::vector<std::string> split_path(const std::string& path)
  {
    std::vector<std::string> parts;
    size_t start = 0;

    while (start <= path.size())
    {
      auto pos = path.find('/', start);
      if (pos == std::string::npos)
      {
        auto part = path.substr(start);
        if (!part.empty())
        {
          parts.push_back(part);
        }
        break;
      }

      auto part = path.substr(start, pos - start);
      if (!part.empty())
      {
        parts.push_back(part);
      }
      start = pos + 1;
    }

    return parts;
  }

  std::string normalize_path(const std::string& path)
  {
    std::vector<std::string> normalized;

    for (const auto& part : split_path(path))
    {
      if (part == ".")
      {
        continue;
      }
      if (part == "..")
      {
        if (!normalized.empty() && normalized.back() != "..")
        {
          normalized.pop_back();
        }
        else
        {
          normalized.push_back(part);
        }
        continue;
      }
      normalized.push_back(part);
    }

    std::string result;
    for (size_t i = 0; i < normalized.size(); ++i)
    {
      if (i > 0)
      {
        result += "/";
      }
      result += normalized[i];
    }
    return result;
  }

  class InMemoryFileSystem : public Roo::FileSystem
  {
    std::map<std::string, std::string> files_;

   public:
    void add(const std::string& name, const std::string& content) { files_[name] = content; }

    const std::string read(const std::string& name) override
    {
      auto it = files_.find(normalize_path(name));
      if (it == files_.end())
      {
        throw Roo::RooException("File not found: " + name);
      }
      return it->second;
    }
  };

  class RootedInMemoryFileSystem : public Roo::FileSystem
  {
    std::map<std::string, std::string> files_;
    std::string root_;

   public:
    explicit RootedInMemoryFileSystem(std::string root)
      : root_(std::move(root))
    {
    }

    void add(const std::string& name, const std::string& content) { files_[name] = content; }

    const std::string read(const std::string& name) override
    {
      const std::string resolved_name =
        normalize_path(root_.empty() ? name : root_ + "/" + name);

      auto it = files_.find(resolved_name);
      if (it == files_.end())
      {
        throw Roo::RooException("File not found: " + resolved_name);
      }
      return it->second;
    }
  };
} // namespace

TEST_F(FileSystemNamespaceSource, single_segment_maps_to_filename)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.roo", "(ns foo)");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns foo)");
}

TEST_F(FileSystemNamespaceSource, dotted_namespace_maps_to_directory_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo/bar/baz.roo", "(ns foo.bar.baz)");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo.bar.baz", {});

  // Then
  ASSERT_TRUE(result.has_value());
}

TEST_F(FileSystemNamespaceSource, dashes_are_preserved_in_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my-module/some-util.roo", "(ns my-module.some-util)");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("my-module.some-util", {});

  // Then
  ASSERT_TRUE(result.has_value());
}

TEST_F(FileSystemNamespaceSource, falls_back_to_lspl_extension)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.lspl", "(ns foo)");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns foo)");
}

TEST_F(FileSystemNamespaceSource, falls_back_to_lisple_extension)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.lisple", "(ns foo)");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns foo)");
}

TEST_F(FileSystemNamespaceSource, roo_extension_takes_priority_over_lspl)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("foo.roo", "roo-content");
  fs.add("foo.lisple", "lisple-content");
  fs.add("foo.lspl", "lspl-content");
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("foo", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "roo-content");
}

TEST_F(FileSystemNamespaceSource, returns_nullopt_when_no_file_found)
{
  // Given
  InMemoryFileSystem fs;
  Roo::FileSystemNamespaceSource source(&fs);

  // When
  auto result = source.fetch("does.not.exist", {});

  // Then
  EXPECT_FALSE(result.has_value());
}

TEST_F(FileSystemNamespaceSource, namespace_root_resolves_prefix_before_full_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("mapped/core.roo", "mapped-content");
  fs.add("mylib/stuff/core.roo", "full-path-content");
  Roo::FileSystemNamespaceSource source(&fs,
                                        {".roo", ".lspl"},
                                        {Roo::NamespaceRoot{"mylib.stuff", "mapped"}});

  // When
  auto result = source.fetch("mylib.stuff.core", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "mapped-content");
  EXPECT_EQ(result->resolved_path, "mapped/core.roo");
}

TEST_F(FileSystemNamespaceSource, namespace_root_uses_longest_matching_prefix)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("general/tool.roo", "general-content");
  fs.add("specific/tool.roo", "specific-content");
  Roo::FileSystemNamespaceSource source(
    &fs,
    {".roo", ".lspl"},
    {Roo::NamespaceRoot{"mylib", "general"}, Roo::NamespaceRoot{"mylib.stuff", "specific"}});

  // When
  auto result = source.fetch("mylib.stuff.tool", {});

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "specific-content");
  EXPECT_EQ(result->resolved_path, "specific/tool.roo");
}

/** FileSystemNamespaceSource - inferred path resolution */

TEST_F(FileSystemNamespaceSource,
       infer_path_strips_package_root_when_loading_from_flat_project_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("models.roo", "(ns my.app.models)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.roo";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models)");
  EXPECT_EQ(result->resolved_path, "models.roo");
}

TEST_F(FileSystemNamespaceSource, infer_path_strips_package_root_with_one_directory_level)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("models/user.roo", "(ns my.app.models.user)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.controllers.users";
  ctx.current_source_path = "controllers/users.roo";

  // When
  auto result = source.fetch("my.app.models.user", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models.user)");
  EXPECT_EQ(result->resolved_path, "models/user.roo");
}

TEST_F(FileSystemNamespaceSource, infer_path_falls_back_to_full_path_when_short_path_missing)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/app/models.roo", "(ns my.app.models)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.roo";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->source, "(ns my.app.models)");
  EXPECT_EQ(result->resolved_path, "my/app/models.roo");
}

TEST_F(FileSystemNamespaceSource, infer_path_infers_root_dir_from_file_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app/models.roo", "(ns my.app.models)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app";
  ctx.current_source_path = "lisp/app.roo";

  // When
  auto result = source.fetch("my.app.models", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "lisp/app/models.roo");
}

TEST_F(FileSystemNamespaceSource, infer_path_does_not_apply_across_different_root_packages)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("other/lib.roo", "(ns other.lib)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "my.app.core";
  ctx.current_source_path = "core.roo";

  // When
  auto result = source.fetch("other.lib", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "other/lib.roo");
}

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_from_package_root_when_entry_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("minesweeper/core.roo", "(ns pixils.test.app.minesweeper.core)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "pixils.test.app";
  ctx.current_source_path = "run.roo";

  // When
  auto result = source.fetch("pixils.test.app.minesweeper.core", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "minesweeper/core.roo");
}

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_sibling_when_current_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("container.roo", "(ns some.namespace.container)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "some.namespace.core";
  ctx.current_source_path = "game.roo";

  // When
  auto result = source.fetch("some.namespace.container", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "container.roo");
}

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_sibling_after_cross_branch_inferred_source_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("../shared/ui/components/button.roo",
         "(ns pixils.test.app.shared.ui.components.button)");
  Roo::FileSystemNamespaceSource source(&fs);

  Roo::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "pixils.test.app.shared.ui.components.window";
  ctx.current_source_path = "../shared/ui/components/window.roo";

  // When
  auto result = source.fetch("pixils.test.app.shared.ui.components.button", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "../shared/ui/components/button.roo");
}

/** NamespaceLoading - integration */

TEST_F(NamespaceLoading, loads_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.roo", "(ns my.utils) (defun double [n] (+ n n))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.eval("(ns my.app (:require my.utils))");

  // Then
  auto result = runtime.eval("(double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, read_file_parse_exception_includes_file_name)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("broken.roo", ")");
  auto& runtime = use_runtime_with(fs);

  // When
  std::string message;
  try
  {
    runtime.read_file("broken.roo");
  }
  catch (const Roo::ParseException& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("Error parsing 'broken.roo'"));
  EXPECT_THAT(message, HasSubstr("Unmatched parens"));
}

TEST_F(NamespaceLoading,
       read_file_parse_exception_from_required_namespace_includes_required_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app.roo", "(ns app (:require app.utils))");
  fs.add("app/utils.roo", ")");
  auto& runtime = use_runtime_with(fs);

  // When
  std::string message;
  try
  {
    runtime.read_file("app.roo");
  }
  catch (const Roo::ParseException& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("Error parsing 'app/utils.roo'"));
  EXPECT_THAT(message, Not(HasSubstr("Error parsing 'app.roo'")));
  EXPECT_THAT(message, HasSubstr("Unmatched parens"));
}

TEST_F(NamespaceLoading,
       read_file_runtime_exception_from_required_namespace_includes_required_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app.roo", "(ns app (:require app.utils)) (run)");
  fs.add("app/utils.roo", "(ns app.utils)\n(defun run []\n  (+ \"bad\" 4))");
  auto& runtime = use_runtime_with(fs);
  runtime.set_source_diagnostics(true);

  // When
  std::string message;
  try
  {
    runtime.read_file("app.roo");
  }
  catch (const std::exception& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("Error reading 'app.roo'"));
  EXPECT_THAT(message, HasSubstr("Error while calling + at app/utils.roo:3:3"));
  EXPECT_THAT(message, HasSubstr("Could not apply args"));
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_ns)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.roo", "(ns app.utils)");
  auto& runtime = use_runtime_with(fs);

  std::cout << "NOW LETS LOAD IT" << std::endl;

  // When
  runtime.eval("(ns app (:require app.utils))");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Roo::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_read_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.roo", "(ns app.utils)");
  fs.add("app.roo", "(ns app (:require app.utils))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("app.roo");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Roo::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_context)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.roo", "(ns app.utils)");
  fs.add("app.roo", "(ns app (:require app.utils))");
  auto& runtime = use_runtime_with(fs);
  Roo::Context ctx(runtime);

  // When
  ctx.read_file("app.roo");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Roo::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_aliased_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.roo", "(ns my.utils) (defun double [n] (+ n n))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.eval("(ns my.app (:require [my.utils :as utils]))");

  // Then
  auto result = runtime.eval("(utils/double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, loads_transitive_requirements)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("base.roo", "(ns base) (defun add [a b] (+ a b))");
  fs.add("mid.roo", "(ns mid (:require base)) (defun double [n] (add n n))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.eval("(ns top (:require mid))");

  // Then
  auto result = runtime.eval("(double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, throws_on_direct_cyclic_dependency)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("ns-a.roo", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.roo", "(ns ns-b (:require ns-a))");
  auto& runtime = use_runtime_with(fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Roo::CyclicNamespaceException);
}

TEST_F(NamespaceLoading, throws_on_indirect_cyclic_dependency)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("ns-a.roo", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.roo", "(ns ns-b (:require ns-c))");
  fs.add("ns-c.roo", "(ns ns-c (:require ns-a))");
  auto& runtime = use_runtime_with(fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Roo::CyclicNamespaceException);
}

TEST_F(NamespaceLoading, does_not_load_already_loaded_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.roo", "(ns my.utils) (defun double [n] (+ n n))");
  auto& runtime = use_runtime_with(fs);
  runtime.eval("(ns my.utils) (defun double [n] (+ n n n))"); // triple, not double

  // When - require the same namespace that is already loaded
  runtime.eval("(ns my.app (:require my.utils))");

  // Then - the pre-loaded version (triple) is used, not the file version
  auto result = runtime.eval("(double 10)");
  EXPECT_EQ(result->to_string(), "30");
}

TEST_F(NamespaceLoading, throws_when_no_filesystem_and_namespace_missing)
{
  // Given
  auto& runtime = use_bare_runtime();

  // Then
  std::string message;
  try
  {
    runtime.eval("(ns my.app (:require my.utils))");
  }
  catch (Roo::RooException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST_F(NamespaceLoading, loads_namespaces_without_application_file_system_access)
{
  // Given
  InMemoryFileSystem namespace_fs;
  namespace_fs.add("my/utils.roo", "(ns my.utils) (defun double [n] (+ n n))");
  auto namespace_source = std::make_unique<Roo::FileSystemNamespaceSource>(&namespace_fs);
  Roo::Runtime runtime(nullptr, std::move(namespace_source));

  // When
  runtime.eval("(ns my.app (:require my.utils))");

  // Then
  EXPECT_FALSE(runtime.has_file_system_access());
  EXPECT_TRUE(runtime.has_namespace_loading());
  auto result = runtime.eval("(double 21)");
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, application_file_io_remains_disabled_when_namespace_source_exists)
{
  // Given
  InMemoryFileSystem namespace_fs;
  namespace_fs.add("my/app.roo",
                   "(ns my.app) (defun read-data [] (roo.io/slurp! \"data.txt\"))");
  auto namespace_source = std::make_unique<Roo::FileSystemNamespaceSource>(&namespace_fs);
  Roo::Runtime runtime(nullptr, std::move(namespace_source));
  runtime.eval("(ns test (:require my.app))");

  // Then
  std::string message;
  try
  {
    runtime.eval("(my.app/read-data)");
  }
  catch (const Roo::RooException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not provide any file system access"));
}

TEST_F(NamespaceLoading, uses_distinct_file_systems_for_namespace_loading_and_application_io)
{
  // Given
  InMemoryFileSystem app_fs;
  app_fs.add("data.txt", "runtime-data");

  InMemoryFileSystem namespace_fs;
  namespace_fs.add("my/app.roo",
                   "(ns my.app) (defun read-data [] (roo.io/slurp! \"data.txt\"))");

  auto namespace_source = std::make_unique<Roo::FileSystemNamespaceSource>(&namespace_fs);
  Roo::Runtime runtime(&app_fs, std::move(namespace_source));

  // When
  runtime.eval("(ns test (:require my.app))");
  auto result = runtime.eval("(my.app/read-data)");

  // Then
  EXPECT_TRUE(runtime.has_file_system_access());
  EXPECT_TRUE(runtime.has_namespace_loading());
  EXPECT_EQ(result->to_string(), "\"runtime-data\"");
}

TEST_F(NamespaceLoading, configures_namespace_roots_without_application_file_system_access)
{
  // Given
  InMemoryFileSystem namespace_fs;
  namespace_fs.add("src/app/core.roo", "(ns app.core) (def value 42)");
  auto namespace_source = std::make_unique<Roo::FileSystemNamespaceSource>(&namespace_fs);
  Roo::Runtime runtime(nullptr, std::move(namespace_source));
  runtime.set_namespace_roots({Roo::NamespaceRoot{"app", "src/app"}});

  // When
  runtime.eval("(ns test (:require app.core))");

  // Then
  auto result = runtime.eval("app.core/value");
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, throws_when_file_not_found_on_filesystem)
{
  // Given
  InMemoryFileSystem fs; // empty - no files
  auto& runtime = use_runtime_with(fs);

  // Then
  std::string message;
  try
  {
    runtime.eval("(ns my.app (:require my.utils))");
  }
  catch (Roo::RooException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST_F(NamespaceLoading, resolves_require_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.roo",
         "(ns my.app (:require my.app.models)) (defun run [] (make-thing 7))");
  fs.add("lisp/app/models.roo", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("lisp/app.roo");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, resolves_require_alias_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.roo",
         "(ns my.app (:require [my.app.models :as m])) (defun run [] (m/make-thing 7))");
  fs.add("lisp/app/models.roo", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("lisp/app.roo");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_requires_from_arbitrary_package_root_entry_file)
{
  // Given
  RootedInMemoryFileSystem fs("app");
  fs.add("app/run.roo", "(ns pixils.test.app (:require pixils.test.app.minesweeper.core))");
  fs.add("app/minesweeper/core.roo",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.roo",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.roo",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("run.roo");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, resolves_sibling_require_when_current_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add(
    "game.roo",
    "(ns some.namespace.core (:require some.namespace.container)) (defun run [] value)");
  fs.add("container.roo", "(ns some.namespace.container) (def value 42)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("game.roo");
  auto result = runtime.eval("(some.namespace.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_requires_when_loading_core_from_common_parent_root)
{
  // Given
  RootedInMemoryFileSystem fs("app");
  fs.add("app/minesweeper/core.roo",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.roo",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.roo",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("minesweeper/core.roo");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_sibling_shared_namespace_when_loading_core_from_branch_root)
{
  // Given
  RootedInMemoryFileSystem fs("app/minesweeper");
  fs.add("app/minesweeper/core.roo",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.roo",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.roo",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("core.roo");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_then_sibling_imports_when_loading_core_from_branch_root)
{
  // Given
  RootedInMemoryFileSystem fs("app/minesweeper");
  fs.add("app/minesweeper/core.roo",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require [pixils.test.app.shared.ui.components.window :as w]))
           (defun run [] (w/window-value))
         )");
  fs.add("app/shared/ui/components/window.roo",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require pixils.test.app.shared.ui.components.button
                       pixils.test.app.shared.ui.components.text-node))
           (defun window-value [] (+ button-value text-value))
         )");
  fs.add("app/shared/ui/components/button.roo",
         "(ns pixils.test.app.shared.ui.components.button) (def button-value 40)");
  fs.add("app/shared/ui/components/text-node.roo",
         "(ns pixils.test.app.shared.ui.components.text-node) (def text-value 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("core.roo");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, load_root_resolves_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.roo", "(ns app.core (:require vehicle))");
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.roo");
  auto result = runtime.eval("app.core/make-vehicle");

  // Then
  ASSERT_EQ(result->type, Roo::Value::Type::FUNCTION);
}

TEST_F(NamespaceLoading, load_root_imports_aliased_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.roo",
         "(ns app.core (:require [vehicle :as v])) (defun mkv [] (v/make-vehicle-model "
         "{:model-name \"FF2\" :seats 8}))");
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.roo");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, load_root_using_both_full_and_aliased_imports)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.roo",
         R"(
           (ns app.core
             (:require app.construction
                       [vehicle :as v]))

           (defun mkv [] (construct v/make-vehicle-model {:model-name "FF2" :seats 8}))

           (def v1 (v/make-vehicle-model {:model-name "blargh" :seats 1}))
           (def v2 (construct v/make-vehicle-model {:model-name "oumph" :seats 2}))
          )");

  fs.add("lisp/construction.roo",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.roo");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, full_import_of_native_ns_survives_subsequent_file_ns_load)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.roo",
         R"(
           (ns app.core (:require vehicle app.construction))
           (defun mkv [] (construct make-vehicle-model {:model-name "FF2" :seats 8}))
         )");
  fs.add("lisp/construction.roo",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.roo");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, alias_of_native_ns_survives_subsequent_file_ns_load)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.roo",
         R"(
           (ns app.core (:require [vehicle :as v] app.construction))
           (defun mkv [] (construct v/make-vehicle-model {:model-name "FF2" :seats 8}))
         )");
  fs.add("lisp/construction.roo",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<RooTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.roo");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}
