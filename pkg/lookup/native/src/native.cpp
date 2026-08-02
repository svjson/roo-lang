#include <memory>
#include <vector>

#include <lookup/native.h>
#include <roo-package/native_abi.h>

namespace Roo::Lookup
{
  std::vector<std::unique_ptr<Namespace>> make_native_namespaces()
  {
    std::vector<std::unique_ptr<Namespace>> namespaces;
    namespaces.push_back(make_analysis_namespace());
    namespaces.push_back(make_reader_namespace());
    namespaces.push_back(make_system_namespace());
    return namespaces;
  }
} // namespace Roo::Lookup

namespace
{
  int load_lookup_native(const RooNativeHostV1* host)
  {
    for (auto& ns : Roo::Lookup::make_native_namespaces())
    {
      int result = host->register_namespace(host->user, ns.release());
      if (result != 0)
      {
        return result;
      }
    }
    return 0;
  }

  void unload_lookup_native() {}

  const char* lookup_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "lookup-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_lookup_native,
    unload_lookup_native,
    lookup_native_last_error,
  };
  return &package;
}
