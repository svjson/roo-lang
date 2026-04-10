
#include "product_native_adapters.h"

#include <lisple/host.h>
#include <lisple/host/accessor.h>
#include <lisple/host/object.h>

namespace LispleTest::Native
{
  const Lisple::sptr_rtval NAME = Lisple::RTValue::keyword("name");
  const Lisple::sptr_rtval PRICE = Lisple::RTValue::keyword("price");
  const Lisple::sptr_rtval SKU = Lisple::RTValue::keyword("sku");
  const Lisple::sptr_rtval AUTHOR = Lisple::RTValue::keyword("author");
  const Lisple::sptr_rtval ISBN = Lisple::RTValue::keyword("isbn");
  const Lisple::sptr_rtval SIZE = Lisple::RTValue::keyword("size");
  const Lisple::sptr_rtval MATERIAL = Lisple::RTValue::keyword("material");

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

} // namespace LispleTest::Native
