
#include "lisple/runtime/value.h"

#include <map>
#include <string>

#include <lisple/exception.h>
#include <lisple/file_system.h>
#include <lisple/file_system_namespace_source.h>
#include <lisple/namespace_source.h>

#include "host/test_adapters/vehicle_native_adapters.h"
#include "runtime_fixture.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using FileSystemNamespaceSource = LispleTest::RuntimeTestFixture;
using NamespaceLoading = LispleTest::RuntimeTestFixture;
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

  class InMemoryFileSystem : public Lisple::FileSystem
  {
    std::map<std::string, std::string> files_;

   public:
    void add(const std::string& name, const std::string& content) { files_[name] = content; }

    const std::string read_file_to_string(const std::string& name) override
    {
      auto it = files_.find(normalize_path(name));
      if (it == files_.end())
      {
        throw Lisple::LispleException("File not found: " + name);
      }
      return it->second;
    }
  };

  class RootedInMemoryFileSystem : public Lisple::FileSystem
  {
    std::map<std::string, std::string> files_;
    std::string root_;

   public:
    explicit RootedInMemoryFileSystem(std::string root)
      : root_(std::move(root))
    {
    }

    void add(const std::string& name, const std::string& content) { files_[name] = content; }

    const std::string read_file_to_string(const std::string& name) override
    {
      const std::string resolved_name =
        normalize_path(root_.empty() ? name : root_ + "/" + name);

      auto it = files_.find(resolved_name);
      if (it == files_.end())
      {
        throw Lisple::LispleException("File not found: " + resolved_name);
      }
      return it->second;
    }
  };
} // namespace

TEST_F(FileSystemNamespaceSource, single_segment_maps_to_filename)
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

TEST_F(FileSystemNamespaceSource, dotted_namespace_maps_to_directory_path)
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

TEST_F(FileSystemNamespaceSource, dashes_are_preserved_in_path)
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

TEST_F(FileSystemNamespaceSource, falls_back_to_lspl_extension)
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

TEST_F(FileSystemNamespaceSource, lisple_extension_takes_priority_over_lspl)
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

TEST_F(FileSystemNamespaceSource, returns_nullopt_when_no_file_found)
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

TEST_F(FileSystemNamespaceSource,
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

TEST_F(FileSystemNamespaceSource, infer_path_strips_package_root_with_one_directory_level)
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

TEST_F(FileSystemNamespaceSource, infer_path_falls_back_to_full_path_when_short_path_missing)
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

TEST_F(FileSystemNamespaceSource, infer_path_infers_root_dir_from_file_path)
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

TEST_F(FileSystemNamespaceSource, infer_path_does_not_apply_across_different_root_packages)
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

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_from_package_root_when_entry_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("minesweeper/core.lisple", "(ns pixils.test.app.minesweeper.core)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "pixils.test.app";
  ctx.current_source_path = "run.lisple";

  // When
  auto result = source.fetch("pixils.test.app.minesweeper.core", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "minesweeper/core.lisple");
}

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_sibling_when_current_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("container.lisple", "(ns some.namespace.container)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "some.namespace.core";
  ctx.current_source_path = "game.lisple";

  // When
  auto result = source.fetch("some.namespace.container", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "container.lisple");
}

TEST_F(FileSystemNamespaceSource,
       infer_path_resolves_sibling_after_cross_branch_inferred_source_path)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("../shared/ui/components/button.lisple",
         "(ns pixils.test.app.shared.ui.components.button)");
  Lisple::FileSystemNamespaceSource source(&fs);

  Lisple::NamespaceResolutionContext ctx;
  ctx.current_ns_name = "pixils.test.app.shared.ui.components.window";
  ctx.current_source_path = "../shared/ui/components/window.lisple";

  // When
  auto result = source.fetch("pixils.test.app.shared.ui.components.button", ctx);

  // Then
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->resolved_path, "../shared/ui/components/button.lisple");
}

/** NamespaceLoading - integration */

TEST_F(NamespaceLoading, loads_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
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
  fs.add("broken.lisple", ")");
  auto& runtime = use_runtime_with(fs);

  // When
  std::string message;
  try
  {
    runtime.read_file("broken.lisple");
  }
  catch (const Lisple::ParseException& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("Error parsing 'broken.lisple'"));
  EXPECT_THAT(message, HasSubstr("Unmatched parens"));
}

TEST_F(NamespaceLoading, read_file_parse_exception_from_required_namespace_includes_required_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app.lisple", "(ns app (:require app.utils))");
  fs.add("app/utils.lisple", ")");
  auto& runtime = use_runtime_with(fs);

  // When
  std::string message;
  try
  {
    runtime.read_file("app.lisple");
  }
  catch (const Lisple::ParseException& e)
  {
    message = e.what();
  }

  // Then
  EXPECT_THAT(message, HasSubstr("Error parsing 'app/utils.lisple'"));
  EXPECT_THAT(message, Not(HasSubstr("Error parsing 'app.lisple'")));
  EXPECT_THAT(message, HasSubstr("Unmatched parens"));
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_ns)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.lisple", "(ns app.utils)");
  auto& runtime = use_runtime_with(fs);

  std::cout << "NOW LETS LOAD IT" << std::endl;

  // When
  runtime.eval("(ns app (:require app.utils))");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Lisple::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_read_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.lisple", "(ns app.utils)");
  fs.add("app.lisple", "(ns app (:require app.utils))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("app.lisple");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Lisple::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_required_empty_namespace_and_marks_it_as_file_via_context)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("app/utils.lisple", "(ns app.utils)");
  fs.add("app.lisple", "(ns app (:require app.utils))");
  auto& runtime = use_runtime_with(fs);
  Lisple::Context ctx(runtime);

  // When
  ctx.read_file("app.lisple");

  // Then
  auto* required_namespace = runtime.ns("app.utils", false);
  ASSERT_NE(required_namespace, nullptr);
  EXPECT_EQ(required_namespace->get_origin().type, Lisple::Namespace::Origin::Type::FILE);
  EXPECT_TRUE(required_namespace->empty());
}

TEST_F(NamespaceLoading, loads_aliased_required_namespace_on_demand)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
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
  fs.add("base.lisple", "(ns base) (defun add [a b] (+ a b))");
  fs.add("mid.lisple", "(ns mid (:require base)) (defun double [n] (add n n))");
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
  fs.add("ns-a.lisple", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.lisple", "(ns ns-b (:require ns-a))");
  auto& runtime = use_runtime_with(fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Lisple::CyclicNamespaceException);
}

TEST_F(NamespaceLoading, throws_on_indirect_cyclic_dependency)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("ns-a.lisple", "(ns ns-a (:require ns-b))");
  fs.add("ns-b.lisple", "(ns ns-b (:require ns-c))");
  fs.add("ns-c.lisple", "(ns ns-c (:require ns-a))");
  auto& runtime = use_runtime_with(fs);

  // Then
  EXPECT_THROW(runtime.eval("(ns main (:require ns-a))"), Lisple::CyclicNamespaceException);
}

TEST_F(NamespaceLoading, does_not_load_already_loaded_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("my/utils.lisple", "(ns my.utils) (defun double [n] (+ n n))");
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
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
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
  catch (Lisple::LispleException& e)
  {
    message = e.what();
  }
  EXPECT_THAT(message, HasSubstr("does not exist"));
}

TEST_F(NamespaceLoading, resolves_require_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.lisple",
         "(ns my.app (:require my.app.models)) (defun run [] (make-thing 7))");
  fs.add("lisp/app/models.lisple", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("lisp/app.lisple");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, resolves_require_alias_relative_to_entry_file)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/app.lisple",
         "(ns my.app (:require [my.app.models :as m])) (defun run [] (m/make-thing 7))");
  fs.add("lisp/app/models.lisple", "(ns my.app.models) (defun make-thing [n] (* n 6))");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("lisp/app.lisple");
  auto result = runtime.eval("(my.app/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_requires_from_arbitrary_package_root_entry_file)
{
  // Given
  RootedInMemoryFileSystem fs("app");
  fs.add("app/run.lisple",
         "(ns pixils.test.app (:require pixils.test.app.minesweeper.core))");
  fs.add("app/minesweeper/core.lisple",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.lisple",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.lisple",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("run.lisple");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, resolves_sibling_require_when_current_filename_is_arbitrary)
{
  // Given
  InMemoryFileSystem fs;
  fs.add(
    "game.lisple",
    "(ns some.namespace.core (:require some.namespace.container)) (defun run [] value)");
  fs.add("container.lisple", "(ns some.namespace.container) (def value 42)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("game.lisple");
  auto result = runtime.eval("(some.namespace.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_requires_when_loading_core_from_common_parent_root)
{
  // Given
  RootedInMemoryFileSystem fs("app");
  fs.add("app/minesweeper/core.lisple",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.lisple",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.lisple",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("minesweeper/core.lisple");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_sibling_shared_namespace_when_loading_core_from_branch_root)
{
  // Given
  RootedInMemoryFileSystem fs("app/minesweeper");
  fs.add("app/minesweeper/core.lisple",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require pixils.test.app.shared.ui.components.window))
           (defun run [] (window-value))
         )");
  fs.add("app/shared/ui/components/window.lisple",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require [pixils.test.app.minesweeper.menu-definition :as md]))
           (defun window-value [] (+ 40 (md/menu-value)))
         )");
  fs.add("app/minesweeper/menu-definition.lisple",
         "(ns pixils.test.app.minesweeper.menu-definition) (defun menu-value [] 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("core.lisple");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading,
       resolves_cross_branch_then_sibling_imports_when_loading_core_from_branch_root)
{
  // Given
  RootedInMemoryFileSystem fs("app/minesweeper");
  fs.add("app/minesweeper/core.lisple",
         R"(
           (ns pixils.test.app.minesweeper.core
             (:require [pixils.test.app.shared.ui.components.window :as w]))
           (defun run [] (w/window-value))
         )");
  fs.add("app/shared/ui/components/window.lisple",
         R"(
           (ns pixils.test.app.shared.ui.components.window
             (:require pixils.test.app.shared.ui.components.button
                       pixils.test.app.shared.ui.components.text-node))
           (defun window-value [] (+ button-value text-value))
         )");
  fs.add("app/shared/ui/components/button.lisple",
         "(ns pixils.test.app.shared.ui.components.button) (def button-value 40)");
  fs.add("app/shared/ui/components/text-node.lisple",
         "(ns pixils.test.app.shared.ui.components.text-node) (def text-value 2)");
  auto& runtime = use_runtime_with(fs);

  // When
  runtime.read_file("core.lisple");
  auto result = runtime.eval("(pixils.test.app.minesweeper.core/run)");

  // Then
  EXPECT_EQ(result->to_string(), "42");
}

TEST_F(NamespaceLoading, load_root_resolves_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple", "(ns app.core (:require vehicle))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("app.core/make-vehicle");

  // Then
  ASSERT_EQ(result->type, Lisple::Value::Type::FUNCTION);
}

TEST_F(NamespaceLoading, load_root_imports_aliased_native_defined_namespace)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple",
         "(ns app.core (:require [vehicle :as v])) (defun mkv [] (v/make-vehicle-model "
         "{:model-name \"FF2\" :seats 8}))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, load_root_using_both_full_and_aliased_imports)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple",
         R"(
           (ns app.core
             (:require app.construction
                       [vehicle :as v]))

           (defun mkv [] (construct v/make-vehicle-model {:model-name "FF2" :seats 8}))

           (def v1 (v/make-vehicle-model {:model-name "blargh" :seats 1}))
           (def v2 (construct v/make-vehicle-model {:model-name "oumph" :seats 2}))
          )");

  fs.add("lisp/construction.lisple",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, full_import_of_native_ns_survives_subsequent_file_ns_load)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple",
         R"(
           (ns app.core (:require vehicle app.construction))
           (defun mkv [] (construct make-vehicle-model {:model-name "FF2" :seats 8}))
         )");
  fs.add("lisp/construction.lisple",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}

TEST_F(NamespaceLoading, alias_of_native_ns_survives_subsequent_file_ns_load)
{
  // Given
  InMemoryFileSystem fs;
  fs.add("lisp/core.lisple",
         R"(
           (ns app.core (:require [vehicle :as v] app.construction))
           (defun mkv [] (construct v/make-vehicle-model {:model-name "FF2" :seats 8}))
         )");
  fs.add("lisp/construction.lisple",
         "(ns app.construction) (defun construct [cfn arg] (apply cfn [arg]))");
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<LispleTest::Native::VehicleNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), &fs);

  // When
  runtime.read_file("lisp/core.lisple");
  auto result = runtime.eval("(app.core/mkv)");

  // Then
  ASSERT_EQ(result->to_string(), "{:model-name \"FF2\" :seats 8}");
}
