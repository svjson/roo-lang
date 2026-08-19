#include "runtime_fixture.h"
#include <gtest/gtest.h>

using ListDirectoryBang = RooTest::RuntimeTestFixture;

TEST_F(ListDirectoryBang, lists_entry_records)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/sprites/player.png", "png");
  fs.add_file("assets/.secret", "hidden");

  EXPECT_EQ(
    runtime.eval(R"((roo.io/list-directory! "assets"))")->to_string(),
    R"([{:name "config.edn" :path "assets/config.edn" :type :file} {:name "sprites" :path "assets/sprites" :type :directory}])");
}

TEST_F(ListDirectoryBang, filters_by_options)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/level.edn", "{}");
  fs.add_file("assets/logo.png", "png");
  fs.add_file("assets/sprites/player.png", "png");
  fs.add_file("assets/.secret.edn", "hidden");

  EXPECT_EQ(
    runtime
      .eval(R"((roo.io/list-directory! "assets" {:directories? false :filter "*.edn"}))")
      ->to_string(),
    R"([{:name "config.edn" :path "assets/config.edn" :type :file} {:name "level.edn" :path "assets/level.edn" :type :file}])");
  EXPECT_EQ(
    runtime
      .eval(
        R"((roo.io/list-directory! "assets" {:directories? false :hidden? true :filter ["*.png" ".*.edn"]}))")
      ->to_string(),
    R"([{:name ".secret.edn" :path "assets/.secret.edn" :type :file} {:name "logo.png" :path "assets/logo.png" :type :file}])");
}

TEST_F(ListDirectoryBang, filters_with_character_classes)
{
  fs.add_file("assets/a.roo", "");
  fs.add_file("assets/b.roo", "");
  fs.add_file("assets/c.roo", "");
  fs.add_file("assets/1.roo", "");

  EXPECT_EQ(
    runtime.eval(R"((map (roo.io/list-directory! "assets" {:filter "[a-b].roo"}) :name))")
      ->to_string(),
    R"(["a.roo" "b.roo"])");
  EXPECT_EQ(
    runtime.eval(R"((map (roo.io/list-directory! "assets" {:filter "[!a-z].roo"}) :name))")
      ->to_string(),
    R"(["1.roo"])");
}
