
#include "product_native_adapters.h"

#include <roo/host/accessor.h>
#include <roo/host/object.h>

namespace RooTest::Native
{
  const Roo::sptr_val NAME = Roo::Value::keyword("name");
  const Roo::sptr_val PRICE = Roo::Value::keyword("price");
  const Roo::sptr_val SKU = Roo::Value::keyword("sku");
  const Roo::sptr_val AUTHOR = Roo::Value::keyword("author");
  const Roo::sptr_val ISBN = Roo::Value::keyword("isbn");
  const Roo::sptr_val SIZE = Roo::Value::keyword("size");
  const Roo::sptr_val MATERIAL = Roo::Value::keyword("material");

  NATIVE_ADAPTER_IMPL(ProductAdapter, Product, &PRODUCT, (name), (price), (sku));
  NOBJ_PROP_GET__METHOD(ProductAdapter, name);
  NOBJ_PROP_GET_SET__METHOD(ProductAdapter, price);
  NOBJ_PROP_GET_SET__METHOD(ProductAdapter, sku);

  NATIVE_SUB_ADAPTER_IMPL(ProductAdapter,
                          Product,
                          (BookAdapter, Book),
                          &BOOK,
                          (author),
                          (isbn))

  NOBJ_PROP_GET__METHOD(BookAdapter, author);
  NOBJ_PROP_GET__METHOD(BookAdapter, isbn);

  NATIVE_SUB_ADAPTER_IMPL(ProductAdapter,
                          Product,
                          (ClothingAdapter, Clothing),
                          &CLOTHING,
                          (size),
                          (material))

  NOBJ_PROP_GET__FIELD(ClothingAdapter, size);
  NOBJ_PROP_GET__FIELD(ClothingAdapter, material);

} // namespace RooTest::Native
