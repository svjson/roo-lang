#include "test_host_objects.h"

#include "lisple/runtime/value.h"

#include <cmath>

#include <lisple/context.h>
#include <lisple/impl.h>
#include <lisple/runtime/dict.h>

namespace LispleTest
{

  // ===============================================================
  // Product example - With sub types
  // ===============================================================

  Product::Product(const std::string& name, float price, int sku)
    : name(name)
    , price(price)
    , sku(sku)
  {
  }

  const std::string& Product::get_name()
  {
    return this->name;
  }
  float Product::get_price()
  {
    return this->price;
  }
  int Product::get_sku()
  {
    return this->sku;
  }

  void Product::set_price(float price)
  {
    this->price = price;
  }
  void Product::set_sku(int sku)
  {
    this->sku = sku;
  }

  Book::Book(const std::string& name,
             float price,
             int sku,
             const std::string& author,
             const std::string& isbn)
    : Product(name, price, sku)
    , author(author)
    , isbn(isbn)
  {
  }

  const std::string& Book::get_author()
  {
    return this->author;
  }
  const std::string& Book::get_isbn()
  {
    return this->isbn;
  }

  Clothing::Clothing(const std::string& name,
                     float price,
                     int sku,
                     const std::string& size,
                     const std::string& material)
    : Product(name, price, sku)
    , size(size)
    , material(material)
  {
  }
} // namespace LispleTest
namespace Tests
{

  // ===============================================================
  // Product example - adapters
  // ===============================================================

  SHKEY(NAME, "name")
  SHKEY(PRICE, "price")
  SHKEY(SKU, "sku")
  SHKEY(AUTHOR, "author")
  SHKEY(ISBN, "isbn")
  SHKEY(SIZE, "size")
  SHKEY(MATERIAL, "material")

  HOST_ADAPTER_IMPL(ProductAdapter,
                    Product,
                    &PRODUCT,
                    ({K_GET(ProductAdapter, NAME, name),
                      K_GET_SET(ProductAdapter, PRICE, price),
                      K_GET_SET(ProductAdapter, SKU, sku)}));

  ADAPTER_PROP_GET__METHOD(ProductAdapter, name, Lisple::String, get_name);
  ADAPTER_PROP_GET__METHOD(ProductAdapter, price, Lisple::Number, get_price);
  ADAPTER_PROP_SET__METHOD(ProductAdapter, price, Lisple::Number, set_price);
  ADAPTER_PROP_GET__METHOD(ProductAdapter, sku, Lisple::Number, get_sku);
  ADAPTER_PROP_SET__METHOD(ProductAdapter, sku, Lisple::Number, set_sku);

  HOST_SUB_ADAPTER_IMPL(BookAdapter,
                        Book,
                        ProductAdapter,
                        Product,
                        &BOOK,
                        ({K_GET(BookAdapter, AUTHOR, author),
                          K_GET(BookAdapter, ISBN, isbn)}));

  ADAPTER_PROP_GET__METHOD(BookAdapter, author, Lisple::String, get_author);
  ADAPTER_PROP_GET__METHOD(BookAdapter, isbn, Lisple::String, get_isbn);

  HOST_SUB_ADAPTER_IMPL(ClothingAdapter,
                        Clothing,
                        ProductAdapter,
                        Product,
                        &CLOTHING,
                        ({K_GET(ClothingAdapter, SIZE, size),
                          K_GET(ClothingAdapter, MATERIAL, material)}));

  ADAPTER_PROP_GET__FIELD(ClothingAdapter, size, Lisple::String);
  ADAPTER_PROP_GET__FIELD(ClothingAdapter, material, Lisple::String);

  // ===============================================================
  // Vector graphics example
  // ===============================================================

} // namespace Tests
