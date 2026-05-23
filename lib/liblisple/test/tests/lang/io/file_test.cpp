#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IoNamespace = LispleTest::RuntimeTestFixture;

TEST_F(IoNamespace, slurp_bang_reads_file_contents)
{
  fs.add_file("notes.txt", "plain text\n");

  EXPECT_EQ(runtime.eval(R"((lisple.io/slurp! "notes.txt"))")->to_string(),
            R"("plain text
")");
}

TEST_F(IoNamespace, spit_bang_writes_file_contents)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/spit! "notes.txt" "plain text"))")->to_string(),
            "nil");

  ASSERT_TRUE(fs.has_file("notes.txt"));
  EXPECT_EQ(fs.get_file("notes.txt"), "plain text");
}

TEST_F(IoNamespace, copy_file_bang_copies_file_contents)
{
  // Given
  fs.add_file("src/native/libexample.so", "binary-ish contents");

  // When
  EXPECT_EQ(
    runtime
      .eval(
        R"((lisple.io/copy-file! "src/native/libexample.so" "repo/native/libexample.so"))")
      ->to_string(),
    "nil");

  // Then
  ASSERT_TRUE(fs.has_file("repo/native/libexample.so"));
  EXPECT_EQ(fs.get_file("repo/native/libexample.so"), "binary-ish contents");
}

TEST_F(IoNamespace, remove_tree_bang_removes_files_under_path)
{
  // Given
  fs.add_file("repo/pkg/proof/package.edn", "{}");
  fs.add_file("repo/pkg/proof/src/proof/core.lisple", "(ns proof.core)");
  fs.add_file("repo/pkg/other/package.edn", "{}");

  // When
  EXPECT_EQ(runtime.eval(R"((lisple.io/remove-tree! "repo/pkg/proof"))")->to_string(),
            "nil");

  // Then
  EXPECT_FALSE(fs.has_file("repo/pkg/proof/package.edn"));
  EXPECT_FALSE(fs.has_file("repo/pkg/proof/src/proof/core.lisple"));
  EXPECT_TRUE(fs.has_file("repo/pkg/other/package.edn"));
}

TEST_F(IoNamespace, slurp_edn_bang_reads_single_data_form)
{
  fs.add_file("data.edn", "{:name \"Lisple\" :numbers [1 2 3] :ok true}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/slurp-edn! "data.edn"))")->to_string(),
            R"({:name "Lisple" :numbers [1 2 3] :ok true})");
}

TEST_F(IoNamespace, spit_edn_bang_writes_value_representation)
{
  runtime.eval(R"((lisple.io/spit-edn! "data.edn" {:a 1 :b [true nil]}))");

  ASSERT_TRUE(fs.has_file("data.edn"));
  EXPECT_EQ(fs.get_file("data.edn"), "{:a 1 :b [true nil]}");
}

TEST_F(IoNamespace, io_namespace_can_be_required_with_alias)
{
  fs.add_file("data.edn", "[:ok 1]");

  EXPECT_EQ(runtime
              .eval(R"(
    (ns my-app.core (:require [lisple.io :as io]))
    (io/slurp-edn! "data.edn")
  )")
              ->to_string(),
            "[:ok 1]");
}

TEST_F(IoNamespace, io_namespace_cannot_be_modified_from_lisple)
{
  EXPECT_THROW(runtime.eval("(ns lisple.io) (def x 1)"), Lisple::NamespaceException);
}

TEST_F(IoNamespace, file_system_predicates_observe_files_and_directories)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/exists? "assets/config.edn"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((lisple.io/file? "assets/config.edn"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((lisple.io/directory? "assets"))")->to_string(), "true");
  EXPECT_EQ(runtime.eval(R"((lisple.io/exists? "assets/missing.edn"))")->to_string(),
            "false");
}

TEST_F(IoNamespace, stat_bang_returns_missing_and_file_records)
{
  fs.add_file("assets/config.edn", "12345");

  EXPECT_EQ(runtime.eval(R"((lisple.io/stat! "missing.edn"))")->to_string(),
            "{:exists? false :type nil :size nil :modified-ms nil}");
  EXPECT_EQ(runtime.eval(R"((lisple.io/stat! "assets/config.edn"))")->to_string(),
            "{:exists? true :type :file :size 5 :modified-ms 0}");
}

TEST_F(IoNamespace, list_directory_bang_lists_entry_records)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/sprites/player.png", "png");
  fs.add_file("assets/.secret", "hidden");

  EXPECT_EQ(
    runtime.eval(R"((lisple.io/list-directory! "assets"))")->to_string(),
    R"([{:name "config.edn" :path "assets/config.edn" :type :file} {:name "sprites" :path "assets/sprites" :type :directory}])");
}

TEST_F(IoNamespace, list_directory_bang_filters_by_options)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/level.edn", "{}");
  fs.add_file("assets/logo.png", "png");
  fs.add_file("assets/sprites/player.png", "png");
  fs.add_file("assets/.secret.edn", "hidden");

  EXPECT_EQ(
    runtime
      .eval(R"((lisple.io/list-directory! "assets" {:directories? false :filter "*.edn"}))")
      ->to_string(),
    R"([{:name "config.edn" :path "assets/config.edn" :type :file} {:name "level.edn" :path "assets/level.edn" :type :file}])");
  EXPECT_EQ(
    runtime
      .eval(
        R"((lisple.io/list-directory! "assets" {:directories? false :hidden? true :filter ["*.png" ".*.edn"]}))")
      ->to_string(),
    R"([{:name ".secret.edn" :path "assets/.secret.edn" :type :file} {:name "logo.png" :path "assets/logo.png" :type :file}])");
}

TEST_F(IoNamespace, directory_helpers_return_runtime_paths)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/current-directory!))")->to_string(), R"("/fake")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/home-directory!))")->to_string(), R"("/home/fake")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/absolute-path! "assets/config.edn"))")->to_string(),
            R"("/fake/assets/config.edn")");
}

TEST_F(IoNamespace, path_helpers_are_pure_string_operations)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/join-path "assets" "config.edn"))")->to_string(),
            R"("assets/config.edn")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/parent-path "assets/config.edn"))")->to_string(),
            R"("assets")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/basename "assets/config.edn"))")->to_string(),
            R"("config.edn")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/dirname "assets/config.edn"))")->to_string(),
            R"("assets")");
  EXPECT_EQ(runtime.eval(R"((lisple.io/extension "assets/config.edn"))")->to_string(),
            R"(".edn")");
}
