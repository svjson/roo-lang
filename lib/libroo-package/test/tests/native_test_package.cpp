#include <roo-package/native_abi.h>

#include <memory>

#include <lisple/context.h>
#include <lisple/exec.h>
#include <lisple/namespace.h>
#include <lisple/runtime/value.h>

namespace
{
  class AnswerFunction : public Lisple::Function
  {
   public:
    AnswerFunction()
      : Function(SIG((FN_ARGS((&Lisple::Type::ANY)),
                      EXEC_DISPATCH(&AnswerFunction::exec_answer))))
    {
    }

    static Lisple::sptr_val make()
    {
      return Lisple::Value::executable(std::make_shared<AnswerFunction>());
    }

    Lisple::sptr_val exec_answer(Lisple::Context&, Lisple::sptr_val_v&)
    {
      return Lisple::Value::number(42);
    }
  };

  int load_native_package(const RooNativeHostV1* host)
  {
    auto ns = std::make_unique<Lisple::Namespace>("package.test.native");
    ns->set_origin(Lisple::Namespace::Origin::native());
    ns->store("answer", AnswerFunction::make());
    return host->register_namespace(host->user, ns.release());
  }

  void unload_native_package()
  {
  }

  const char* last_error()
  {
    return "";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1*
roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "roo-package-test-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_native_package,
    unload_native_package,
    last_error,
  };
  return &package;
}
