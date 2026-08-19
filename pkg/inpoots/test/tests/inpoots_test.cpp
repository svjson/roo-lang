#include <roo/io/dir_root_file_system.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#include <gtest/gtest.h>
#include <inpoots/native.h>
#include <roo-package/manifest.h>
#include <roo-package/native_loader.h>

TEST(InpootsNative, ConstructsInputNamespace)
{
  auto namespaces = Roo::Inpoots::make_native_namespaces();

  ASSERT_EQ(namespaces.size(), 1);
  EXPECT_EQ(namespaces[0]->get_name(), "inpoots.input");
  EXPECT_EQ(namespaces[0]->get_origin().type, Roo::Namespace::Origin::Type::NATIVE);
  EXPECT_TRUE(namespaces[0]->has("with-stdin!"));
  EXPECT_TRUE(namespaces[0]->has("read!"));
}

TEST(InpootsPackage, LoadsInputNamespaceFromManifest)
{
  Roo::DirRootFileSystem manifest_fs("/");
  const auto plan = Roo::Package::resolve_load_plan(manifest_fs, INPOOTS_PACKAGE_DIR);
  Roo::Package::LoadedNativePackages native_packages;
  Roo::Runtime runtime;

  native_packages = Roo::Package::load_native_libraries(runtime, plan);

  ASSERT_NE(runtime.ns("inpoots.input"), nullptr);
  EXPECT_EQ(runtime.ns("inpoots.input")->get_origin().type,
            Roo::Namespace::Origin::Type::NATIVE);
}
