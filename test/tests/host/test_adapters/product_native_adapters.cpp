
#include "product_native_adapters.h"

#include <lisple/host/accessor.h>
#include <lisple/host/object.h>

namespace LispleTest::Native
{
  const Lisple::sptr_val NAME = Lisple::Value::keyword("name");
  const Lisple::sptr_val PRICE = Lisple::Value::keyword("price");
  const Lisple::sptr_val SKU = Lisple::Value::keyword("sku");
  const Lisple::sptr_val AUTHOR = Lisple::Value::keyword("author");
  const Lisple::sptr_val ISBN = Lisple::Value::keyword("isbn");
  const Lisple::sptr_val SIZE = Lisple::Value::keyword("size");
  const Lisple::sptr_val MATERIAL = Lisple::Value::keyword("material");

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

} // namespace LispleTest::Native
