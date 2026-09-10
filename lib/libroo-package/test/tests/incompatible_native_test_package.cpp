#include <roo-package/native_abi.h>

namespace
{
  int load_native_package(const RooNativeHostV1*)
  {
    return 0;
  }

  const char* last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1) + sizeof(void*),
    "roo-package-test-incompatible-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_native_package,
    last_error,
  };
  return &package;
}
