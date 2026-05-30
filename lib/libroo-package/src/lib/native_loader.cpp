#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <roo-package/native_abi.h>
#include <roo-package/native_loader.h>
#include <roo/exception.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace Roo::Package
{
  using Roo::Namespace;
  using Roo::RooException;
  using Roo::Runtime;

  namespace
  {
#if defined(_WIN32)
    using LibraryHandle = HMODULE;

    LibraryHandle open_library(const std::string& path)
    {
      return LoadLibraryA(path.c_str());
    }

    void* find_symbol(LibraryHandle handle, const char* symbol)
    {
      return reinterpret_cast<void*>(GetProcAddress(handle, symbol));
    }

    void close_library(LibraryHandle handle)
    {
      if (handle)
      {
        FreeLibrary(handle);
      }
    }

    std::string platform_error()
    {
      return "native library operation failed";
    }
#else
    using LibraryHandle = void*;

    LibraryHandle open_library(const std::string& path)
    {
      return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    }

    void* find_symbol(LibraryHandle handle, const char* symbol)
    {
      return dlsym(handle, symbol);
    }

    void close_library(LibraryHandle handle)
    {
      if (handle)
      {
        dlclose(handle);
      }
    }

    std::string platform_error()
    {
      const char* error = dlerror();
      return error ? error : "native library operation failed";
    }
#endif

    std::string platform_library_file_name(const std::string& name)
    {
#if defined(_WIN32)
      return name + ".dll";
#elif defined(__APPLE__)
      return "lib" + name + ".dylib";
#else
      return "lib" + name + ".so";
#endif
    }

    std::string join_path(const std::string& root, const std::string& child)
    {
      if (root.empty() || root == ".")
      {
        return child;
      }
      if (root.back() == '/')
      {
        return root + child;
      }
      return root + "/" + child;
    }

    std::string native_library_path(const NativeLibrary& library)
    {
      if (!library.path.empty())
      {
        std::error_code ec;
        if (std::filesystem::is_directory(library.path, ec))
        {
          return join_path(library.path, platform_library_file_name(library.name));
        }
        return library.path;
      }

      const std::string file_name = platform_library_file_name(library.name);
      if (library.package_root.empty() || library.package_root == ".")
      {
        return file_name;
      }
      return join_path(library.package_root, file_name);
    }

    struct NativeLoadContext
    {
      Runtime& runtime;
      std::string error;
    };

    int register_namespace(void* user, Namespace* raw_ns)
    {
      auto* context = static_cast<NativeLoadContext*>(user);
      try
      {
        std::unique_ptr<Namespace> ns(raw_ns);
        context->runtime.register_namespace(std::move(ns));
        return 0;
      }
      catch (const std::exception& e)
      {
        context->error = e.what();
        return 1;
      }
    }

    void note(void*, const char*) {}

    std::string package_error(const RooNativePackageV1* package)
    {
      if (package && package->last_error)
      {
        const char* error = package->last_error();
        if (error && error[0] != '\0')
        {
          return error;
        }
      }
      return "native package load failed";
    }

    struct LoadedLibrary
    {
      LibraryHandle handle = nullptr;
      const RooNativePackageV1* package = nullptr;

      LoadedLibrary() = default;

      LoadedLibrary(LibraryHandle handle, const RooNativePackageV1* package)
        : handle(handle)
        , package(package)
      {
      }

      LoadedLibrary(LoadedLibrary&& other) noexcept
        : handle(std::exchange(other.handle, nullptr))
        , package(std::exchange(other.package, nullptr))
      {
      }

      LoadedLibrary& operator=(LoadedLibrary&& other) noexcept
      {
        if (this != &other)
        {
          close();
          handle = std::exchange(other.handle, nullptr);
          package = std::exchange(other.package, nullptr);
        }
        return *this;
      }

      LoadedLibrary(const LoadedLibrary&) = delete;
      LoadedLibrary& operator=(const LoadedLibrary&) = delete;

      ~LoadedLibrary() { close(); }

      void close()
      {
        if (package && package->unload)
        {
          package->unload();
        }
        package = nullptr;
        close_library(handle);
        handle = nullptr;
      }
    };
  } // namespace

  struct LoadedNativePackages::Impl
  {
    std::vector<LoadedLibrary> libraries;
  };

  LoadedNativePackages::LoadedNativePackages()
    : impl(std::make_unique<Impl>())
  {
  }

  LoadedNativePackages::LoadedNativePackages(std::unique_ptr<Impl> impl)
    : impl(std::move(impl))
  {
  }

  LoadedNativePackages::~LoadedNativePackages() = default;
  LoadedNativePackages::LoadedNativePackages(LoadedNativePackages&&) noexcept = default;
  LoadedNativePackages& LoadedNativePackages::operator=(LoadedNativePackages&&) noexcept =
    default;

  LoadedNativePackages load_native_libraries(Runtime& runtime, const LoadPlan& plan)
  {
    auto impl = std::make_unique<LoadedNativePackages::Impl>();

    for (const auto& library : plan.native_libraries)
    {
      const std::string path = native_library_path(library);
      LibraryHandle handle = open_library(path);
      if (!handle)
      {
        throw RooException("Could not load native package library '" + path +
                           "': " + platform_error());
      }

      auto close_on_error = std::unique_ptr<void, void (*)(void*)>(
        handle,
        [](void* value) { close_library(static_cast<LibraryHandle>(value)); });

      auto* symbol = find_symbol(handle, ROO_NATIVE_ABI_SYMBOL);
      if (!symbol)
      {
        throw RooException("Native package library '" + path + "' does not export " +
                           ROO_NATIVE_ABI_SYMBOL + ": " + platform_error());
      }

      auto package_fn = reinterpret_cast<RooNativePackageV1Fn>(symbol);
      const RooNativePackageV1* package = package_fn();
      if (!package)
      {
        throw RooException("Native package library '" + path +
                           "' returned a null package descriptor.");
      }
      if (package->abi_version != ROO_NATIVE_ABI_VERSION ||
          package->struct_size < sizeof(RooNativePackageV1))
      {
        throw RooException("Native package library '" + path +
                           "' uses an incompatible native ABI.");
      }
      if (!package->roo_abi || std::string(package->roo_abi) != ROO_NATIVE_CXX_ABI)
      {
        throw RooException("Native package library '" + path +
                           "' targets an incompatible Roo ABI.");
      }
      if (package->package_name && library.name != package->package_name)
      {
        throw RooException("Native package library '" + path + "' identifies as '" +
                           package->package_name + "', expected '" + library.name + "'.");
      }
      if (!library.version.empty() && package->package_version &&
          library.version != package->package_version)
      {
        throw RooException("Native package library '" + path + "' has version '" +
                           package->package_version + "', expected '" + library.version +
                           "'.");
      }
      if (!package->load)
      {
        throw RooException("Native package library '" + path + "' has no load function.");
      }

      NativeLoadContext context{runtime, ""};
      RooNativeHostV1 host{
        ROO_NATIVE_ABI_VERSION,
        sizeof(RooNativeHostV1),
        &context,
        register_namespace,
        note,
      };

      if (package->load(&host) != 0)
      {
        const std::string error =
          context.error.empty() ? package_error(package) : context.error;
        throw RooException("Native package library '" + path + "' failed to load: " + error);
      }

      close_on_error.release();
      impl->libraries.emplace_back(handle, package);
    }

    return LoadedNativePackages(std::move(impl));
  }
} // namespace Roo::Package
