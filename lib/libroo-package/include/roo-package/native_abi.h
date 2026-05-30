#ifndef ROO_PACKAGE_NATIVE_ABI_H
#define ROO_PACKAGE_NATIVE_ABI_H

#include <cstdint>

#if defined(_WIN32)
#define ROO_NATIVE_EXPORT __declspec(dllexport)
#else
#define ROO_NATIVE_EXPORT __attribute__((visibility("default")))
#endif

#define ROO_NATIVE_ABI_VERSION 1u
#define ROO_NATIVE_ABI_SYMBOL "roo_native_package_v1"
#define ROO_NATIVE_CXX_ABI "roo-cxx"

namespace Roo
{
  class Namespace;
}

struct RooNativeHostV1
{
  uint32_t abi_version;
  uint32_t struct_size;

  void* user;

  int (*register_namespace)(void* user, Roo::Namespace* ns);
  void (*note)(void* user, const char* message);
};

struct RooNativePackageV1
{
  uint32_t abi_version;
  uint32_t struct_size;

  const char* package_name;
  const char* package_version;
  const char* roo_abi;

  int (*load)(const RooNativeHostV1* host);
  void (*unload)();
  const char* (*last_error)();
};

using RooNativePackageV1Fn = const RooNativePackageV1* (*)();

#endif
