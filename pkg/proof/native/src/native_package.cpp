#include <proof/native.h>
#include <roo-package/native_abi.h>

namespace
{
  int load_proof_native(const RooNativeHostV1* host)
  {
    for (auto& ns : Roo::Proof::make_native_namespaces())
    {
      const int result = host->register_namespace(host->user, ns.release());
      if (result != 0)
      {
        return result;
      }
    }
    return 0;
  }

  void unload_proof_native() {}

  const char* proof_native_last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "proof-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_proof_native,
    unload_proof_native,
    proof_native_last_error,
  };
  return &package;
}
