#ifndef ROOTEST__HOST__PRODUCT_NATIVE_ADAPTERS_H
#define ROOTEST__HOST__PRODUCT_NATIVE_ADAPTERS_H

#include "product_impl.h"
#include <roo/exec.h>
#include <roo/host/object.h>
#include <roo/macro_support.h>
#include <roo/runtime/value.h>

namespace RooTest::Native
{
  inline Roo::HostTypeRef PRODUCT("Product");
  inline Roo::HostTypeRef BOOK("Book", &PRODUCT);
  inline Roo::HostTypeRef CLOTHING("Clothing", &PRODUCT);

  NATIVE_ADAPTER(ProductAdapter, Product, (name, price, sku), (price, sku));

  NATIVE_SUB_ADAPTER(ProductAdapter, (BookAdapter, Book), (author, isbn));
  NATIVE_SUB_ADAPTER(ProductAdapter, (ClothingAdapter, Clothing), (size, material));

} // namespace RooTest::Native

#endif
