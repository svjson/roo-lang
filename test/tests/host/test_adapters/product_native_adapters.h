#ifndef LISPLETEST__HOST__PRODUCT_NATIVE_ADAPTERS_H
#define LISPLETEST__HOST__PRODUCT_NATIVE_ADAPTERS_H

#include <lisple/exec.h>
#include <lisple/host/object.h>
#include <lisple/runtime/value.h>

#include "product_impl.h"

namespace LispleTest::Native
{
  inline Lisple::HostTypeRef PRODUCT("Product");
  inline Lisple::HostTypeRef BOOK("Book", &PRODUCT);
  inline Lisple::HostTypeRef CLOTHING("Clothing", &PRODUCT);

  NATIVE_ADAPTER(ProductAdapter, Product, (name, price, sku), (price, sku));
} // namespace LispleTest::Native

#endif
