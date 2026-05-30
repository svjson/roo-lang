#include <filesystem>
#include <fstream>
#include <map>
#include <string>

#include <roo/exception.h>
#include <roo/io/file_system.h>
#include <roo/runtime.h>

#include <gtest/gtest.h>
#include <roo-package/manifest.h>
#include <roo-package/native_loader.h>

namespace
{
  class MemoryFileSystem : public Roo::FileSystem
  {
    std::map<std::string, std::string> files;

   public:
    const std::string read(const std::string& file_name) override
    {
      return files.at(file_name);
    }

    void add(const std::string& path, const std::string& source) { files[path] = source; }
  };

  const char* proof_manifest = R"(
    {:name proof
     :version "0.1.0"
     :description "Roo test framework."
     :dependencies []
     :load-roots ["src"]
     :namespace-roots {proof "src/proof"}
     :native-libraries [{:name "proof-native"
                         :namespaces [proof.syntax]}]
     :autoloads [proof.core]
     :config {proof {:test-roots ["test"]}}
     :tools {run proof.runner/run}
     :entry-points [proof.core]
     :main proof.runner/main
     :run proof}
  )";

  void write_file(const std::filesystem::path& path, const std::string& source)
  {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream stream(path);
    stream << source;
  }
} // namespace

TEST(PackageManifest, parses_current_package_metadata_shape)
{
  auto manifest = Roo::Package::parse_manifest(proof_manifest, "proof/package.edn");

  EXPECT_EQ(manifest.name, "proof");
  EXPECT_EQ(manifest.version, "0.1.0");
  EXPECT_EQ(manifest.description, "Roo test framework.");
  EXPECT_TRUE(manifest.dependencies.empty());
  EXPECT_EQ(manifest.load_roots, std::vector<std::string>{"src"});
  ASSERT_EQ(manifest.namespace_roots.size(), 1u);
  EXPECT_EQ(manifest.namespace_roots[0].ns_prefix, "proof");
  EXPECT_EQ(manifest.namespace_roots[0].path, "src/proof");
  EXPECT_TRUE(manifest.native_namespaces.empty());
  ASSERT_EQ(manifest.native_libraries.size(), 1u);
  EXPECT_EQ(manifest.native_libraries[0].name, "proof-native");
  EXPECT_EQ(manifest.native_libraries[0].namespaces,
            std::vector<std::string>{"proof.syntax"});
  EXPECT_EQ(manifest.autoloads, std::vector<std::string>{"proof.core"});
  EXPECT_EQ(manifest.config.at("proof"), "{:test-roots [\"test\"]}");
  EXPECT_EQ(manifest.tools.at("run"), "proof.runner/run");
  EXPECT_EQ(manifest.entry_points, std::vector<std::string>{"proof.core"});
  EXPECT_EQ(manifest.main, "proof.runner/main");
  EXPECT_EQ(manifest.run, "proof");
}

TEST(PackageManifest, parses_namespace_roots_with_single_and_multiple_paths)
{
  auto manifest = Roo::Package::parse_manifest(
    R"({:name app
        :version "0.1.0"
        :dependencies []
        :load-roots ["src"]
        :namespace-roots {app.core "src"
                          app.feature ["src" "test"]}})",
    "app/package.edn");

  ASSERT_EQ(manifest.namespace_roots.size(), 3u);
  EXPECT_EQ(manifest.namespace_roots[0].ns_prefix, "app.core");
  EXPECT_EQ(manifest.namespace_roots[0].path, "src");
  EXPECT_EQ(manifest.namespace_roots[1].ns_prefix, "app.feature");
  EXPECT_EQ(manifest.namespace_roots[1].path, "src");
  EXPECT_EQ(manifest.namespace_roots[2].ns_prefix, "app.feature");
  EXPECT_EQ(manifest.namespace_roots[2].path, "test");
}

TEST(PackageManifest, parses_dependency_map_with_versions_and_paths)
{
  auto manifest = Roo::Package::parse_manifest(
    R"({:name app
        :dependencies {util {:path "../vendor/util"}
                       data "0.1.0"
                       local "file:../vendor/local"
                       ui {:version "2.0.0"
                           :path "/opt/roo/ui"}}
        :load-roots ["src"]})",
    "app/package.edn");

  ASSERT_EQ(manifest.dependencies.size(), 4);
  EXPECT_EQ(manifest.dependencies[0].name, "util");
  EXPECT_EQ(manifest.dependencies[0].path, "../vendor/util");
  EXPECT_EQ(manifest.dependencies[1].name, "data");
  EXPECT_EQ(manifest.dependencies[1].version, "0.1.0");
  EXPECT_EQ(manifest.dependencies[2].name, "local");
  EXPECT_EQ(manifest.dependencies[2].path, "../vendor/local");
  EXPECT_EQ(manifest.dependencies[3].name, "ui");
  EXPECT_EQ(manifest.dependencies[3].version, "2.0.0");
  EXPECT_EQ(manifest.dependencies[3].path, "/opt/roo/ui");
}

TEST(PackageManifest, builds_load_plan_from_manifest_and_package_root)
{
  auto manifest = Roo::Package::parse_manifest(proof_manifest, "proof/package.edn");

  auto plan = Roo::Package::build_load_plan(manifest, "/repo/pkg/proof");

  EXPECT_EQ(plan.package_root, "/repo/pkg/proof");
  EXPECT_EQ(plan.package_roots, std::vector<std::string>{"/repo/pkg/proof"});
  ASSERT_EQ(plan.packages.size(), 1u);
  EXPECT_EQ(plan.packages[0].name, "proof");
  EXPECT_EQ(plan.packages[0].package_root, "/repo/pkg/proof");
  EXPECT_EQ(plan.packages[0].load_roots, std::vector<std::string>{"/repo/pkg/proof/src"});
  EXPECT_EQ(plan.packages[0].config.at("proof"), "{:test-roots [\"test\"]}");
  EXPECT_EQ(plan.packages[0].tools.at("run"), "proof.runner/run");
  EXPECT_EQ(plan.load_paths, std::vector<std::string>{"/repo/pkg/proof/src"});
  ASSERT_EQ(plan.namespace_roots.size(), 1u);
  EXPECT_EQ(plan.namespace_roots[0].ns_prefix, "proof");
  EXPECT_EQ(plan.namespace_roots[0].path, "/repo/pkg/proof/src/proof");
  EXPECT_EQ(plan.native_namespaces, std::vector<std::string>{"proof.syntax"});
  ASSERT_EQ(plan.native_libraries.size(), 1u);
  EXPECT_EQ(plan.native_libraries[0].name, "proof-native");
  EXPECT_EQ(plan.native_libraries[0].package_root, "/repo/pkg/proof");
  EXPECT_EQ(plan.autoloads, std::vector<std::string>{"proof.core"});
  EXPECT_EQ(plan.entry_points, std::vector<std::string>{"proof.core"});
  EXPECT_EQ(plan.main, "proof.runner/main");
  EXPECT_EQ(plan.run, "proof");
}

TEST(PackageManifest, merges_extra_load_paths_before_resolved_package_paths)
{
  auto manifest = Roo::Package::parse_manifest(proof_manifest, "proof/package.edn");
  auto plan = Roo::Package::build_load_plan(manifest, "/repo/pkg/proof");

  EXPECT_EQ(Roo::Package::merge_load_paths(plan, {"/repo/app/src"}),
            (std::vector<std::string>{"/repo/app/src", "/repo/pkg/proof/src"}));
}

TEST(PackageManifest, rejects_non_map_manifest)
{
  EXPECT_THROW(Roo::Package::parse_manifest("[proof]", "bad/package.edn"),
               Roo::RooException);
}

TEST(PackageManifest, resolves_pure_roo_dependencies_from_search_roots)
{
  MemoryFileSystem fs;
  fs.add("/repo/pkg/app/package.edn",
         R"({:name app
             :dependencies [util]
             :load-roots ["src"]
             :entry-points [app.core]})");
  fs.add("/repo/pkg/util/package.edn",
         R"({:name util
             :dependencies []
             :load-roots ["src"]})");

  auto plan = Roo::Package::resolve_load_plan(fs,
                                              "/repo/pkg/app",
                                              Roo::Package::ResolveOptions{{"/repo/pkg"}});

  EXPECT_EQ(plan.package_root, "/repo/pkg/app");
  EXPECT_EQ(plan.package_roots,
            (std::vector<std::string>{"/repo/pkg/util", "/repo/pkg/app"}));
  ASSERT_EQ(plan.packages.size(), 2u);
  EXPECT_EQ(plan.packages[0].name, "util");
  EXPECT_EQ(plan.packages[0].package_root, "/repo/pkg/util");
  EXPECT_EQ(plan.packages[1].name, "app");
  EXPECT_EQ(plan.packages[1].package_root, "/repo/pkg/app");
  EXPECT_EQ(plan.load_paths,
            (std::vector<std::string>{"/repo/pkg/util/src", "/repo/pkg/app/src"}));
  EXPECT_EQ(plan.entry_points, std::vector<std::string>{"app.core"});
}

TEST(PackageManifest, resolves_versioned_dependencies_from_repository_layout)
{
  // Given
  MemoryFileSystem fs;
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {util "0.1.0"}
             :load-roots ["src"]})");
  fs.add("/repo/packages/util/0.1.0/package.edn",
         R"({:name util
             :version "0.1.0"
             :dependencies []
             :load-roots ["src"]})");

  // When
  auto plan =
    Roo::Package::resolve_load_plan(fs,
                                    "/repo/app",
                                    Roo::Package::ResolveOptions{{"/repo/packages"}});

  // Then
  EXPECT_EQ(plan.package_roots,
            (std::vector<std::string>{"/repo/packages/util/0.1.0", "/repo/app"}));
  EXPECT_EQ(plan.load_paths,
            (std::vector<std::string>{"/repo/packages/util/0.1.0/src", "/repo/app/src"}));
}

TEST(PackageManifest, resolves_versioned_dependencies_from_default_local_repository)
{
  // Given
  MemoryFileSystem fs;
  const std::string repository_root = Roo::Package::default_local_repository_root();
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {util "0.1.0"}
             :load-roots ["src"]})");
  fs.add(repository_root + "/util/0.1.0/package.edn",
         R"({:name util
             :version "0.1.0"
             :dependencies []
             :load-roots ["src"]})");

  // When
  auto plan = Roo::Package::resolve_load_plan(fs, "/repo/app");

  // Then
  EXPECT_EQ(plan.package_roots,
            (std::vector<std::string>{repository_root + "/util/0.1.0", "/repo/app"}));
  EXPECT_EQ(
    plan.load_paths,
    (std::vector<std::string>{repository_root + "/util/0.1.0/src", "/repo/app/src"}));
}

TEST(PackageManifest, deduplicates_same_package_resolved_from_repository_and_path)
{
  // Given
  MemoryFileSystem fs;
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {proof "0.1.0"
                            helper "file:../helper"}
             :load-roots ["src"]})");
  fs.add("/repo/helper/package.edn",
         R"({:name helper
             :dependencies {proof {:path "../proof-path"}}
             :load-roots ["src"]})");
  fs.add("/repo/proof-path/package.edn",
         R"({:name proof
             :version "0.1.0"
             :dependencies []
             :load-roots ["src"]})");
  fs.add("/repo/packages/proof/0.1.0/package.edn",
         R"({:name proof
             :version "0.1.0"
             :dependencies []
             :load-roots ["src"]})");

  // When
  auto plan =
    Roo::Package::resolve_load_plan(fs,
                                    "/repo/app",
                                    Roo::Package::ResolveOptions{{"/repo/packages"}});

  // Then
  EXPECT_EQ(
    plan.package_roots,
    (std::vector<std::string>{"/repo/packages/proof/0.1.0", "/repo/helper", "/repo/app"}));
  EXPECT_EQ(plan.load_paths,
            (std::vector<std::string>{"/repo/packages/proof/0.1.0/src",
                                      "/repo/helper/src",
                                      "/repo/app/src"}));
}

TEST(PackageManifest, rejects_same_package_name_with_different_versions)
{
  // Given
  MemoryFileSystem fs;
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {proof "0.1.0"
                            helper "file:../helper"}
             :load-roots ["src"]})");
  fs.add("/repo/helper/package.edn",
         R"({:name helper
             :dependencies {proof {:path "../proof-path"}}
             :load-roots ["src"]})");
  fs.add("/repo/proof-path/package.edn",
         R"({:name proof
             :version "0.2.0"
             :dependencies []
             :load-roots ["src"]})");
  fs.add("/repo/packages/proof/0.1.0/package.edn",
         R"({:name proof
             :version "0.1.0"
             :dependencies []
             :load-roots ["src"]})");

  // Then
  EXPECT_THROW(
    Roo::Package::resolve_load_plan(fs,
                                    "/repo/app",
                                    Roo::Package::ResolveOptions{{"/repo/packages"}}),
    Roo::RooException);
}

TEST(PackageManifest, resolved_load_plan_uses_root_package_main)
{
  // Given
  MemoryFileSystem fs;
  fs.add("/repo/pkg/app/package.edn",
         R"({:name app
             :dependencies {util "file:../util"}
             :load-roots ["src"]
             :main app.core/main})");
  fs.add("/repo/pkg/util/package.edn",
         R"({:name util
             :dependencies []
             :load-roots ["src"]
             :main util.core/main})");

  // When
  auto plan = Roo::Package::resolve_load_plan(fs, "/repo/pkg/app");

  // Then
  EXPECT_EQ(plan.main, "app.core/main");
}

TEST(PackageManifest, resolved_load_plan_uses_root_package_run_tool)
{
  // Given
  MemoryFileSystem fs;
  fs.add("/repo/pkg/app/package.edn",
         R"({:name app
             :dependencies {runner "file:../runner"}
             :load-roots ["src"]
             :run runner})");
  fs.add("/repo/pkg/runner/package.edn",
         R"({:name runner
             :dependencies []
             :load-roots ["src"]
             :tools {run runner.tool/run}
             :run other})");

  // When
  auto plan = Roo::Package::resolve_load_plan(fs, "/repo/pkg/app");

  // Then
  EXPECT_EQ(plan.run, "runner");
}

TEST(PackageManifest, resolves_dependency_paths_from_manifest)
{
  MemoryFileSystem fs;
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {util "file:../vendor/util"
                            data {:path "../vendor/data"
                                  :version "1.2.3"}}
             :load-roots ["src"]})");
  fs.add("/repo/vendor/util/package.edn",
         R"({:name util
             :dependencies []
             :load-roots ["src"]})");
  fs.add("/repo/vendor/data/package.edn",
         R"({:name data
             :version "1.2.3"
             :dependencies []
             :load-roots ["src"]})");

  auto plan = Roo::Package::resolve_load_plan(fs, "/repo/app");

  EXPECT_EQ(
    plan.package_roots,
    (std::vector<std::string>{"/repo/vendor/util", "/repo/vendor/data", "/repo/app"}));
  EXPECT_EQ(plan.load_paths,
            (std::vector<std::string>{"/repo/vendor/util/src",
                                      "/repo/vendor/data/src",
                                      "/repo/app/src"}));
}

TEST(PackageManifest, rejects_path_dependency_with_mismatched_version)
{
  MemoryFileSystem fs;
  fs.add("/repo/app/package.edn",
         R"({:name app
             :dependencies {util {:path "../vendor/util"
                                  :version "2.0.0"}}
             :load-roots ["src"]})");
  fs.add("/repo/vendor/util/package.edn",
         R"({:name util
             :version "1.0.0"
             :dependencies []
             :load-roots ["src"]})");

  EXPECT_THROW(Roo::Package::resolve_load_plan(fs, "/repo/app"), Roo::RooException);
}

TEST(PackageManifest, resolves_transitive_dependencies_before_dependents)
{
  MemoryFileSystem fs;
  fs.add("pkg/app/package.edn", R"({:name app :dependencies [ui] :load-roots ["src"]})");
  fs.add("pkg/ui/package.edn", R"({:name ui :dependencies [core] :load-roots ["src"]})");
  fs.add("pkg/core/package.edn", R"({:name core :dependencies [] :load-roots ["src"]})");

  auto plan =
    Roo::Package::resolve_load_plan(fs, "pkg/app", Roo::Package::ResolveOptions{{"pkg"}});

  EXPECT_EQ(plan.package_roots, (std::vector<std::string>{"pkg/core", "pkg/ui", "pkg/app"}));
  EXPECT_EQ(plan.load_paths,
            (std::vector<std::string>{"pkg/core/src", "pkg/ui/src", "pkg/app/src"}));
}

TEST(PackageManifest, reports_missing_dependencies)
{
  MemoryFileSystem fs;
  fs.add("pkg/app/package.edn",
         R"({:name app :dependencies [missing] :load-roots ["src"]})");

  EXPECT_THROW(
    Roo::Package::resolve_load_plan(fs, "pkg/app", Roo::Package::ResolveOptions{{"pkg"}}),
    Roo::RooException);
}

TEST(PackageManifest, detects_dependency_cycles)
{
  MemoryFileSystem fs;
  fs.add("pkg/app/package.edn", R"({:name app :dependencies [util] :load-roots ["src"]})");
  fs.add("pkg/util/package.edn", R"({:name util :dependencies [app] :load-roots ["src"]})");

  EXPECT_THROW(
    Roo::Package::resolve_load_plan(fs, "pkg/app", Roo::Package::ResolveOptions{{"pkg"}}),
    Roo::RooException);
}

TEST(PackageManifest, resolved_pure_roo_dependencies_are_available_to_runtime)
{
  const auto root =
    std::filesystem::temp_directory_path() / "roo-package-pure-dependency-test";
  std::filesystem::remove_all(root);

  write_file(root / "pkg/util/package.edn",
             R"({:name util :dependencies [] :load-roots ["src"]})");
  write_file(root / "pkg/util/src/util/core.roo",
             R"((ns util.core)
                (def dependency-value 41))");
  write_file(root / "pkg/app/package.edn",
             R"({:name app :dependencies [util] :load-roots ["src"]})");
  write_file(root / "pkg/app/src/app/core.roo",
             R"((ns app.core
                  (:require util.core))
                (defun run []
                  (+ dependency-value 1)))");

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan =
    Roo::Package::resolve_load_plan(*manifest_fs,
                                    (root / "pkg/app").string(),
                                    Roo::Package::ResolveOptions{{(root / "pkg").string()}});

  auto package_fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Runtime runtime(package_fs.get());
  runtime.read_file("app/core.roo");

  EXPECT_EQ(runtime.eval("(app.core/run)")->to_string(), "42");

  std::filesystem::remove_all(root);
}

TEST(PackageManifest, fixture_package_can_run_code_from_file_dependency)
{
  const auto packages_root =
    std::filesystem::path(ROO_PACKAGE_TEST_DIR) / "tests/assets/packages";
  const auto cafe_register_root = packages_root / "cafe-register";

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan = Roo::Package::resolve_load_plan(*manifest_fs, cafe_register_root.string());

  EXPECT_EQ(plan.package_roots,
            (std::vector<std::string>{(packages_root / "recipe-book").string(),
                                      cafe_register_root.string()}));

  auto package_fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Runtime runtime(package_fs.get());
  runtime.read_file("cafe/register.roo");

  EXPECT_EQ(runtime.eval("(cafe.register/morning-sale-total)")->to_string(), "50");
}

TEST(PackageManifest, namespace_roots_make_prefixed_namespaces_available_to_runtime)
{
  const auto root =
    std::filesystem::temp_directory_path() / "roo-package-namespace-root-test";
  std::filesystem::remove_all(root);

  write_file(root / "pkg/app/package.edn",
             R"({:name app
                 :dependencies []
                 :load-roots ["src"]
                 :namespace-roots {mylib.stuff "src/roo/main-stuff"}})");
  write_file(root / "pkg/app/src/roo/main-stuff/core.roo",
             R"((ns mylib.stuff.core)
                (def value 42))");

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan = Roo::Package::resolve_load_plan(*manifest_fs, (root / "pkg/app").string());

  auto package_fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Runtime runtime(package_fs.get());
  Roo::Package::configure_runtime_namespace_roots(runtime, plan);

  runtime.eval("(ns app (:require mylib.stuff.core))");

  EXPECT_EQ(runtime.eval("mylib.stuff.core/value")->to_string(), "42");

  std::filesystem::remove_all(root);
}

TEST(PackageManifest, namespace_roots_make_multiple_paths_available_to_runtime)
{
  const auto root =
    std::filesystem::temp_directory_path() / "roo-package-namespace-root-vector-test";
  std::filesystem::remove_all(root);

  write_file(root / "pkg/app/package.edn",
             R"({:name app
                 :dependencies []
                 :load-roots ["src" "test"]
                 :namespace-roots {mylib.stuff ["src/roo/main-stuff"
                                                "test/roo/main-stuff"]}})");
  write_file(root / "pkg/app/src/roo/main-stuff/core.roo",
             R"((ns mylib.stuff.core)
                (def value 42))");
  write_file(root / "pkg/app/test/roo/main-stuff/fixture.roo",
             R"((ns mylib.stuff.fixture)
                (def value 99))");

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan = Roo::Package::resolve_load_plan(*manifest_fs, (root / "pkg/app").string());

  ASSERT_EQ(plan.namespace_roots.size(), 2u);
  EXPECT_EQ(plan.namespace_roots[0].path,
            (root / "pkg/app/src/roo/main-stuff").lexically_normal().string());
  EXPECT_EQ(plan.namespace_roots[1].path,
            (root / "pkg/app/test/roo/main-stuff").lexically_normal().string());

  auto package_fs = Roo::Package::make_load_path_file_system(plan);
  Roo::Runtime runtime(package_fs.get());
  Roo::Package::configure_runtime_namespace_roots(runtime, plan);

  runtime.eval("(ns app (:require mylib.stuff.core mylib.stuff.fixture))");

  EXPECT_EQ(runtime.eval("mylib.stuff.core/value")->to_string(), "42");
  EXPECT_EQ(runtime.eval("mylib.stuff.fixture/value")->to_string(), "99");

  std::filesystem::remove_all(root);
}

TEST(PackageManifest, loads_native_library_namespaces_into_runtime)
{
  const auto root =
    std::filesystem::temp_directory_path() / "roo-package-native-library-test";
  std::filesystem::remove_all(root);

  write_file(root / "pkg/native-app/package.edn",
             std::string(R"({:name native-app
                :dependencies []
                :load-roots ["src"]
                :native-libraries [{:name "roo-package-test-native"
                                    :version "0.1.0"
                                    :path ")") +
               ROO_PACKAGE_TEST_NATIVE_LIBRARY +
               R"("
                                    :namespaces [package.test.native]}]})");
  write_file(root / "pkg/native-app/src/native/app.roo",
             R"((ns native.app
                  (:require [package.test.native :as native]))

                (defun run []
                  (native/answer nil)))");

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan =
    Roo::Package::resolve_load_plan(*manifest_fs, (root / "pkg/native-app").string());

  Roo::Package::LoadedNativePackages native_packages;
  {
    auto package_fs = Roo::Package::make_load_path_file_system(plan);
    Roo::Runtime runtime(package_fs.get());
    native_packages = Roo::Package::load_native_libraries(runtime, plan);
    runtime.read_file("native/app.roo");

    EXPECT_EQ(runtime.eval("(native.app/run)")->to_string(), "42");
  }

  std::filesystem::remove_all(root);
}

TEST(PackageManifest, autoloads_run_after_native_libraries_are_available)
{
  const auto root = std::filesystem::temp_directory_path() / "roo-package-autoload-test";
  std::filesystem::remove_all(root);

  write_file(root / "pkg/native-app/package.edn",
             std::string(R"({:name native-app
                :dependencies []
                :load-roots ["src"]
                :native-libraries [{:name "roo-package-test-native"
                                    :version "0.1.0"
                                    :path ")") +
               ROO_PACKAGE_TEST_NATIVE_LIBRARY +
               R"("
                                    :namespaces [package.test.native]}]
                :autoloads [native.bootstrap]})");
  write_file(root / "pkg/native-app/src/native/bootstrap.roo",
             R"((ns native.bootstrap
                  (:require [package.test.native :as native]))

                (def autoloaded-value (native/answer nil)))");

  Roo::Package::LoadPlan host_plan;
  host_plan.load_paths = {"/"};
  auto manifest_fs = Roo::Package::make_load_path_file_system(host_plan);
  auto plan =
    Roo::Package::resolve_load_plan(*manifest_fs, (root / "pkg/native-app").string());

  Roo::Package::LoadedNativePackages native_packages;
  {
    auto package_fs = Roo::Package::make_load_path_file_system(plan);
    Roo::Runtime runtime(package_fs.get());
    native_packages = Roo::Package::load_native_libraries(runtime, plan);
    Roo::Package::load_autoloads(runtime, plan);

    EXPECT_EQ(runtime.eval("native.bootstrap/autoloaded-value")->to_string(), "42");
  }

  std::filesystem::remove_all(root);
}

TEST(PackageManifest, rejects_non_vector_list_fields)
{
  EXPECT_THROW(
    Roo::Package::parse_manifest("{:name proof :load-roots \"src\"}", "bad/package.edn"),
    Roo::RooException);
}
