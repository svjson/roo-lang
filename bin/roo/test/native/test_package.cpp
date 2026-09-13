#include <memory>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/namespace.h>
#include <roo/runtime/value.h>

#include <roo-package/native_abi.h>

namespace
{
  class FailFunction : public Roo::Function
  {
   public:
    FailFunction()
      : Function(SIG((FN_ARGS((&Roo::Type::ANY)), EXEC_DISPATCH(&FailFunction::exec_fail))))
    {
    }

    Roo::sptr_val exec_fail(Roo::Context&, Roo::sptr_val_v&)
    {
      throw Roo::RooException("native test failure");
    }
  };

  int load_native_package(const RooNativeHostV1* host)
  {
    auto ns = std::make_unique<Roo::Namespace>("roo.cli.test.native");
    ns->set_origin(Roo::Namespace::Origin::native());
    ns->store("fail", Roo::Value::executable(std::make_shared<FailFunction>()));
    return host->register_namespace(host->user, ns.release());
  }

  const char* last_error()
  {
    return "Could not load Roo CLI native test package";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "roo-cli-test-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_native_package,
    last_error,
  };
  return &package;
}
