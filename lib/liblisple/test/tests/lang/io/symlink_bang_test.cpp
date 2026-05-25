#include <filesystem>
#include <fstream>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using SymlinkBang = LispleTest::RuntimeTestFixture;

namespace
{
  std::filesystem::path fresh_symlink_test_root(const std::string& name)
  {
    const std::filesystem::path root =
      std::filesystem::temp_directory_path() / "lisple-io-symlink-tests" / name;
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    return root;
  }
} // namespace

TEST_F(SymlinkBang, creates_and_reads_symbolic_link)
{
  const std::filesystem::path root = fresh_symlink_test_root("creates-and-reads");
  const std::filesystem::path source = root / "source.txt";
  const std::filesystem::path link = root / "links" / "source.txt";
  std::filesystem::create_directories(source.parent_path());
  std::ofstream(source) << "linked contents";

  Lisple::DirRootFileSystem dir_fs(root.string());
  Lisple::Runtime& linked_runtime = use_runtime_with(dir_fs);

  EXPECT_EQ(linked_runtime.eval(R"((lisple.io/symlink! "source.txt" "links/source.txt"))")
              ->to_string(),
            "nil");

  EXPECT_EQ(linked_runtime.eval(R"((lisple.io/symlink? "links/source.txt"))")->to_string(),
            "true");
  EXPECT_EQ(linked_runtime.eval(R"((lisple.io/slurp! "links/source.txt"))")->to_string(),
            R"("linked contents")");
  EXPECT_EQ(linked_runtime.eval(R"((lisple.io/read-link! "links/source.txt"))")->to_string(),
            "\"" + source.lexically_normal().string() + "\"");

  std::filesystem::remove_all(root);
}

TEST_F(SymlinkBang, remove_tree_removes_link_without_removing_target)
{
  const std::filesystem::path root = fresh_symlink_test_root("remove-tree-safety");
  const std::filesystem::path source_dir = root / "source-package";
  const std::filesystem::path source_manifest = source_dir / "package.edn";
  const std::filesystem::path link = root / "repo" / "linked" / "0.1.0";
  std::filesystem::create_directories(source_dir);
  std::ofstream(source_manifest) << "{:name linked :version \"0.1.0\"}";

  Lisple::DirRootFileSystem dir_fs(root.string());
  Lisple::Runtime& linked_runtime = use_runtime_with(dir_fs);
  linked_runtime.eval(R"((lisple.io/symlink! "source-package" "repo/linked/0.1.0"))");

  EXPECT_TRUE(std::filesystem::is_symlink(link));

  EXPECT_EQ(
    linked_runtime.eval(R"((lisple.io/remove-tree! "repo/linked/0.1.0"))")->to_string(),
    "nil");

  EXPECT_FALSE(std::filesystem::exists(link));
  EXPECT_TRUE(std::filesystem::exists(source_manifest));

  std::filesystem::remove_all(root);
}
