#include <algorithm>
#include <filesystem>
#include <fstream>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

#ifndef _WIN32
#include <unistd.h>

#include <sys/resource.h>
#endif

using WalkBang = RooTest::RuntimeTestFixture;

TEST_F(WalkBang, keeps_files_with_extensible_traversal_metadata)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/sprites/player.png", "png");

  EXPECT_EQ(
    runtime.eval(R"((roo.io/walk! "assets"))")->to_string(),
    R"([{:entry {:name "config.edn" :path "assets/config.edn" :type :file} :metadata {:root "assets" :relative-path "config.edn" :depth 1 :root? false :symlink? false}} {:entry {:name "player.png" :path "assets/sprites/player.png" :type :file} :metadata {:root "assets" :relative-path "sprites/player.png" :depth 2 :root? false :symlink? false}}])");
}

TEST_F(WalkBang, criteria_receive_entries_and_metadata)
{
  fs.add_file("assets/config.edn", "{}");
  fs.add_file("assets/generated/skip.edn", "{}");
  fs.add_file("assets/src/app.roo", "(ns app)");
  fs.add_file("assets/src/readme.txt", "text");

  EXPECT_EQ(
    runtime
      .eval(R"((roo.io/walk!
                 "assets"
                 {:descend? (fn [entry metadata]
                              (not (= "generated" (:relative-path metadata))))
                  :keep? (fn [entry metadata]
                           (and (= :file (:type entry))
                                (= ".roo" (roo.io/extension
                                            (:relative-path metadata)))))}))")
      ->to_string(),
    R"([{:entry {:name "app.roo" :path "assets/src/app.roo" :type :file} :metadata {:root "assets" :relative-path "src/app.roo" :depth 2 :root? false :symlink? false}}])");
}

TEST_F(WalkBang, can_keep_a_pruned_directory)
{
  fs.add_file("assets/package/package.edn", "{}");
  fs.add_file("assets/package/src/app.roo", "(ns app)");

  EXPECT_EQ(
    runtime
      .eval(R"((roo.io/walk!
                 "assets"
                 {:descend? (fn [entry metadata]
                              (not (= "package" (:relative-path metadata))))
                  :keep? (fn [entry metadata]
                           (= "package" (:relative-path metadata)))}))")
      ->to_string(),
    R"([{:entry {:name "package" :path "assets/package" :type :directory} :metadata {:root "assets" :relative-path "package" :depth 1 :root? false :symlink? false}}])");
}

TEST_F(WalkBang, excludes_hidden_entries_by_default)
{
  fs.add_file("assets/.hidden/secret.roo", "(ns secret)");
  fs.add_file("assets/visible.roo", "(ns visible)");

  EXPECT_EQ(runtime
              .eval(R"((map (roo.io/walk! "assets")
                                (fn [result]
                                  (:relative-path (:metadata result)))))")
              ->to_string(),
            R"(["visible.roo"])");

  EXPECT_EQ(runtime
              .eval(R"((map (roo.io/walk! "assets" {:hidden? true})
                                (fn [result]
                                  (:relative-path (:metadata result)))))")
              ->to_string(),
            R"([".hidden/secret.roo" "visible.roo"])");
}

TEST_F(WalkBang, includes_a_file_root)
{
  fs.add_file("assets/config.edn", "{}");

  EXPECT_EQ(
    runtime.eval(R"((roo.io/walk! "assets/config.edn"))")->to_string(),
    R"([{:entry {:name "config.edn" :path "assets/config.edn" :type :file} :metadata {:root "assets/config.edn" :relative-path "." :depth 0 :root? true :symlink? false}}])");
}

#ifndef _WIN32
TEST(WalkBangRealFileSystem, closes_each_directory_before_descending)
{
  const std::filesystem::path root =
    std::filesystem::temp_directory_path() /
    ("roo-walk-deep-test-" + std::to_string(static_cast<long>(getpid())));
  std::filesystem::remove_all(root);

  std::filesystem::path directory = root;
  for (int depth = 0; depth < 40; depth++)
  {
    directory /= "d";
  }
  std::filesystem::create_directories(directory);
  std::ofstream(directory / "leaf.roo") << "(ns leaf)";
  std::filesystem::create_directory_symlink(directory, root / "alias");

  Roo::DirRootFileSystem file_system(".");
  Roo::Runtime runtime(&file_system);

  struct rlimit original_limit;
  ASSERT_EQ(getrlimit(RLIMIT_NOFILE, &original_limit), 0);
  struct rlimit low_limit = original_limit;
  low_limit.rlim_cur = std::min<rlim_t>(original_limit.rlim_cur, 16);
  ASSERT_EQ(setrlimit(RLIMIT_NOFILE, &low_limit), 0);

  try
  {
    EXPECT_EQ(runtime.eval("(count (roo.io/walk! \"" + root.string() + "\"))")->to_string(),
              "1");
  }
  catch (...)
  {
    setrlimit(RLIMIT_NOFILE, &original_limit);
    std::filesystem::remove_all(root);
    throw;
  }

  EXPECT_EQ(setrlimit(RLIMIT_NOFILE, &original_limit), 0);
  std::filesystem::remove_all(root);
}
#endif
