#include "product_impl.h"

namespace LispleTest
{
  Product::Product(const std::string& name, float price, int sku)
    : name(name)
    , price(price)
    , sku(sku)
  {
  }

  const std::string& Product::get_name()
  {
    return name;
  }

  float Product::get_price()
  {
    return price;
  }

  int Product::get_sku()
  {
    return sku;
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
    return author;
  }

  const std::string& Book::get_isbn()
  {
    return isbn;
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
