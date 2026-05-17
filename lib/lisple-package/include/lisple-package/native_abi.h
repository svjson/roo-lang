#ifndef LISPLE_PACKAGE_NATIVE_ABI_H
#define LISPLE_PACKAGE_NATIVE_ABI_H

#include <cstdint>

#if defined(_WIN32)
#define LISPLE_NATIVE_EXPORT __declspec(dllexport)
#else
#define LISPLE_NATIVE_EXPORT __attribute__((visibility("default")))
#endif

#define LISPLE_NATIVE_ABI_VERSION 1u
#define LISPLE_NATIVE_ABI_SYMBOL "lisple_native_package_v1"
#define LISPLE_NATIVE_CXX_ABI "lisple-cxx"

namespace Lisple
{
  class Namespace;
}

struct LispleNativeHostV1
{
  uint32_t abi_version;
  uint32_t struct_size;

  void* user;

  int (*register_namespace)(void* user, Lisple::Namespace* ns);
  void (*note)(void* user, const char* message);
};

struct LispleNativePackageV1
{
  uint32_t abi_version;
  uint32_t struct_size;

  const char* package_name;
  const char* package_version;
  const char* lisple_abi;

  int (*load)(const LispleNativeHostV1* host);
  void (*unload)();
  const char* (*last_error)();
};

using LispleNativePackageV1Fn = const LispleNativePackageV1* (*)();

#endif
