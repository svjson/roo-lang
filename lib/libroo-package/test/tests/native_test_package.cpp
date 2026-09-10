#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/namespace.h>
#include <roo/runtime/value.h>

#include <roo-package/native_abi.h>

namespace
{
  std::atomic<bool> loading = false;

  class AnswerFunction : public Roo::Function
  {
   public:
    AnswerFunction()
      : Function(
          SIG((FN_ARGS((&Roo::Type::ANY)), EXEC_DISPATCH(&AnswerFunction::exec_answer))))
    {
    }

    static Roo::sptr_val make()
    {
      return Roo::Value::executable(std::make_shared<AnswerFunction>());
    }

    Roo::sptr_val exec_answer(Roo::Context&, Roo::sptr_val_v&)
    {
      return Roo::Value::number(42);
    }
  };

  class FailFunction : public Roo::Function
  {
   public:
    FailFunction()
      : Function(SIG((FN_ARGS((&Roo::Type::ANY)), EXEC_DISPATCH(&FailFunction::exec_fail))))
    {
    }

    static Roo::sptr_val make()
    {
      return Roo::Value::executable(std::make_shared<FailFunction>());
    }

    Roo::sptr_val exec_fail(Roo::Context&, Roo::sptr_val_v&)
    {
      throw Roo::RooException("native test failure");
    }
  };

  int load_native_package(const RooNativeHostV1* host)
  {
    bool expected = false;
    if (!loading.compare_exchange_strong(expected, true))
    {
      return 1;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    auto ns = std::make_unique<Roo::Namespace>("package.test.native");
    ns->set_origin(Roo::Namespace::Origin::native());
    ns->store("answer", AnswerFunction::make());
    ns->store("fail", FailFunction::make());
    const int status = host->register_namespace(host->user, ns.release());
    loading.store(false);
    return status;
  }

  const char* last_error()
  {
    return "concurrent native package load";
  }
} // namespace

extern "C" ROO_NATIVE_EXPORT const RooNativePackageV1* roo_native_package_v1()
{
  static const RooNativePackageV1 package{
    ROO_NATIVE_ABI_VERSION,
    sizeof(RooNativePackageV1),
    "roo-package-test-native",
    "0.1.0",
    ROO_NATIVE_CXX_ABI,
    load_native_package,
    last_error,
  };
  return &package;
}
