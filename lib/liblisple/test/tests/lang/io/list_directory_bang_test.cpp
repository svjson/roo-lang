#include "runtime_fixture.h"

#include <gtest/gtest.h>

using ListDirectoryBang = LispleTest::RuntimeTestFixture;

TEST_F(ListDirectoryBang, lists_entry_records)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/sprites/player.png", "png");
  fs.add_file("assets/.secret", "hidden");

  EXPECT_EQ(
    runtime.eval(R"((lisple.io/list-directory! "assets"))")->to_string(),
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
