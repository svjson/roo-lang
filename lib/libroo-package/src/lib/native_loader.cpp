#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <roo/exception.h>
#include <roo/namespace.h>
#include <roo/runtime.h>

#include <roo-package/native_abi.h>
#include <roo-package/native_loader.h>

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

    std::string canonical_library_path(const std::string& path)
    {
      std::error_code ec;
      std::filesystem::path absolute = std::filesystem::absolute(path, ec);
      if (ec)
      {
        return std::filesystem::path(path).lexically_normal().string();
      }

      std::filesystem::path canonical = std::filesystem::weakly_canonical(absolute, ec);
      return ec ? absolute.lexically_normal().string() : canonical.string();
    }

    std::string loaded_library_path(LibraryHandle handle,
                                    void* symbol,
                                    const std::string& requested_path)
    {
#if defined(_WIN32)
      std::vector<char> buffer(32768);
      const DWORD size =
        GetModuleFileNameA(handle, buffer.data(), static_cast<DWORD>(buffer.size()));
      return size == 0 || size == buffer.size() ? requested_path
                                                : std::string(buffer.data(), size);
#else
      (void)handle;
      Dl_info info{};
      return dladdr(symbol, &info) != 0 && info.dli_fname ? info.dli_fname : requested_path;
#endif
    }

    struct NativeLoadContext
    {
      std::vector<std::unique_ptr<Namespace>> namespaces;
      std::string error;
    };

    int register_namespace(void* user, Namespace* raw_ns)
    {
      auto* context = static_cast<NativeLoadContext*>(user);
      try
      {
        std::unique_ptr<Namespace> ns(raw_ns);
        context->namespaces.push_back(std::move(ns));
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

    void validate_package(const std::string& path,
                          const NativeLibrary& library,
                          const RooNativePackageV1* package)
    {
      if (!package)
      {
        throw RooException("Native package library '" + path +
                           "' returned a null package descriptor.");
      }
      if (package->abi_version != ROO_NATIVE_ABI_VERSION ||
          package->struct_size != sizeof(RooNativePackageV1))
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
    }

    struct NativeLibraryGeneration
    {
      LibraryHandle handle;
      const RooNativePackageV1* package;
      std::string path;
      std::string package_name;
      std::string package_version;
      std::mutex load_mutex;

      NativeLibraryGeneration(LibraryHandle handle,
                              const RooNativePackageV1* package,
                              std::string path,
                              std::string package_name,
                              std::string package_version)
        : handle(handle)
        , package(package)
        , path(std::move(path))
        , package_name(std::move(package_name))
        , package_version(std::move(package_version))
      {
      }

      void validate(const std::string& requested_path, const NativeLibrary& library) const
      {
        validate_package(requested_path, library, package);
        if (library.name != package_name)
        {
          throw RooException("Native package library '" + requested_path +
                             "' is already pinned as '" + package_name + "'.");
        }
        if (!library.version.empty() && !package_version.empty() &&
            library.version != package_version)
        {
          throw RooException("Native package library '" + requested_path +
                             "' is already pinned at version '" + package_version + "'.");
        }
      }
    };

    struct NativeLibraryRegistry
    {
      std::mutex mutex;
      std::map<std::string, std::unique_ptr<NativeLibraryGeneration>> generations;
      std::map<std::string, NativeLibraryGeneration*> aliases;
    };

    NativeLibraryRegistry& native_library_registry()
    {
      static auto* registry = new NativeLibraryRegistry();
      return *registry;
    }

    NativeLibraryGeneration& native_library_generation(const NativeLibrary& library)
    {
      const std::string path = native_library_path(library);
      const std::string canonical_path = canonical_library_path(path);
      NativeLibraryRegistry& registry = native_library_registry();
      std::lock_guard<std::mutex> lock(registry.mutex);

      auto alias = registry.aliases.find(canonical_path);
      if (alias != registry.aliases.end())
      {
        alias->second->validate(path, library);
        return *alias->second;
      }
      if (auto pinned = registry.generations.find(canonical_path);
          pinned != registry.generations.end())
      {
        pinned->second->validate(path, library);
        return *pinned->second;
      }

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
      validate_package(path, library, package);

      const std::string resolved_path =
        canonical_library_path(loaded_library_path(handle, symbol, path));
      auto existing = registry.generations.find(resolved_path);
      if (existing != registry.generations.end())
      {
        existing->second->validate(path, library);
        registry.aliases.emplace(canonical_path, existing->second.get());
        return *existing->second;
      }

      auto generation = std::make_unique<NativeLibraryGeneration>(
        handle,
        package,
        resolved_path,
        package->package_name ? package->package_name : library.name,
        package->package_version ? package->package_version : library.version);
      NativeLibraryGeneration& result = *generation;
      registry.generations.emplace(resolved_path, std::move(generation));
      close_on_error.release();
      registry.aliases.emplace(canonical_path, &result);
      return result;
    }
  } // namespace

  struct LoadedNativePackages::Impl
  {
    std::vector<const NativeLibraryGeneration*> generations;
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
    NativeLoadContext context;

    for (const auto& library : plan.native_libraries)
    {
      const std::string path = native_library_path(library);
      NativeLibraryGeneration& generation = native_library_generation(library);
      RooNativeHostV1 host{
        ROO_NATIVE_ABI_VERSION,
        sizeof(RooNativeHostV1),
        &context,
        register_namespace,
        note,
      };

      std::lock_guard<std::mutex> lock(generation.load_mutex);
      context.error.clear();
      if (generation.package->load(&host) != 0)
      {
        const std::string error =
          context.error.empty() ? package_error(generation.package) : context.error;
        throw RooException("Native package library '" + path + "' failed to load: " + error);
      }

      impl->generations.push_back(&generation);
    }

    runtime.register_namespaces(std::move(context.namespaces));
    return LoadedNativePackages(std::move(impl));
  }
} // namespace Roo::Package
