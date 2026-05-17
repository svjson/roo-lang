#ifndef LISPLE_PACKAGE_NATIVE_LOADER_H
#define LISPLE_PACKAGE_NATIVE_LOADER_H

#include <memory>

#include <lisple-package/manifest.h>

namespace Lisple
{
  class Runtime;
}

namespace Lisple::Package
{
  class LoadedNativePackages
  {
   public:
    LoadedNativePackages();
    ~LoadedNativePackages();

    LoadedNativePackages(LoadedNativePackages&&) noexcept;
    LoadedNativePackages& operator=(LoadedNativePackages&&) noexcept;

    LoadedNativePackages(const LoadedNativePackages&) = delete;
    LoadedNativePackages& operator=(const LoadedNativePackages&) = delete;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    explicit LoadedNativePackages(std::unique_ptr<Impl> impl);

    friend LoadedNativePackages load_native_libraries(Runtime& runtime,
                                                      const LoadPlan& plan);
  };

  LoadedNativePackages load_native_libraries(Runtime& runtime, const LoadPlan& plan);
} // namespace Lisple::Package

#endif
