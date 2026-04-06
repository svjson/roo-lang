
#ifndef __TEST_HOST_OBJECTS_H_
#define __TEST_HOST_OBJECTS_H_

#include <string>

#include <lisple/exec.h>
#include <lisple/host.h>
#include <lisple/namespace.h>
#include <lisple/type.h>

#include "host/test_adapters/product_impl.h"

using namespace LispleTest;

namespace Tests
{

  // ===============================================================
  // Product example - With sub types
  // ===============================================================

  inline Lisple::HostTypeRef PRODUCT("Product");
  inline Lisple::HostTypeRef BOOK("Book", &PRODUCT);
  inline Lisple::HostTypeRef CLOTHING("Clothing", &PRODUCT);

  // ===============================================================
  // Product example - adapters
  // ===============================================================

  HOST_ADAPTER(ProductAdapter, Product, (name, price, sku), (price, sku));

  HOST_SUB_ADAPTER(BookAdapter, Book, ProductAdapter, Product, (author, isbn));

  HOST_SUB_ADAPTER(ClothingAdapter, Clothing, ProductAdapter, Product, (size, material));

} // namespace Tests

#endif
