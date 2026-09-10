#ifndef ROO_PACKAGE_NATIVE_LOADER_H
#define ROO_PACKAGE_NATIVE_LOADER_H

#include <memory>

#include <roo-package/manifest.h>

namespace Roo
{
  class Runtime;
}

namespace Roo::Package
{
  /**
   * @brief Records native package generations activated for one Runtime.
   *
   * Destruction does not unload current-ABI libraries. Validated generations
   * remain mapped for process lifetime.
   */
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

    friend LoadedNativePackages load_native_libraries(Roo::Runtime& runtime,
                                                      const LoadPlan& plan);
  };

  /**
   * @brief Activate every native library in a load plan for a Runtime.
   *
   * Namespace registrations are published atomically after every native load
   * entry point succeeds.
   *
   * @param runtime Runtime that receives the native namespaces.
   * @param plan Resolved package load plan in dependency-first order.
   * @return Activation metadata that does not control library residency.
   */
  LoadedNativePackages load_native_libraries(Roo::Runtime& runtime, const LoadPlan& plan);
} // namespace Roo::Package

#endif
