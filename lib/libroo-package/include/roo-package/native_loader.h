#ifndef ROO_PACKAGE_NATIVE_LOADER_H
#define ROO_PACKAGE_NATIVE_LOADER_H

#include <memory>

#include <roo-package/manifest.h>

namespace Lisple
{
  class Runtime;
}

namespace Roo::Package
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

    friend LoadedNativePackages load_native_libraries(Lisple::Runtime& runtime,
                                                      const LoadPlan& plan);
  };

  LoadedNativePackages load_native_libraries(Lisple::Runtime& runtime,
                                             const LoadPlan& plan);
} // namespace Roo::Package

#endif
