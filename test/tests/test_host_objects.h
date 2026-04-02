
#ifndef __TEST_HOST_OBJECTS_H_
#define __TEST_HOST_OBJECTS_H_

#include <string>

#include <lisple/exec.h>
#include <lisple/host.h>
#include <lisple/namespace.h>
#include <lisple/type.h>

namespace Tests
{

  // ===============================================================
  // Product example - With sub types
  // ===============================================================

  inline Lisple::HostTypeRef PRODUCT("Product");
  inline Lisple::HostTypeRef BOOK("Book", &PRODUCT);
  inline Lisple::HostTypeRef CLOTHING("Clothing", &PRODUCT);

  class Product
  {
   public:
    Product(const std::string& name, float price, int sku);
    virtual ~Product() = default;

    const std::string& get_name();
    float get_price();
    int get_sku();

    void set_price(float price);
    void set_sku(int sku);

   protected:
    const std::string name;
    float price;
    int sku;
  };

  class Book : public Product
  {
   public:
    Book(const std::string& name,
         float price,
         int sku,
         const std::string& author,
         const std::string& isbn);

    const std::string& get_author();
    const std::string& get_isbn();

   protected:
    const std::string author;
    const std::string isbn;
  };

  class Clothing : public Product
  {

   public:
    Clothing(const std::string& name,
             float price,
             int sku,
             const std::string& size,
             const std::string& material);

    const std::string size;
    const std::string material;
  };

  // ===============================================================
  // Product example - adapters
  // ===============================================================

  HOST_ADAPTER(ProductAdapter, Product, (name, price, sku), (price, sku));

  HOST_SUB_ADAPTER(BookAdapter, Book, ProductAdapter, Product, (author, isbn));

  HOST_SUB_ADAPTER(ClothingAdapter, Clothing, ProductAdapter, Product, (size, material));

} // namespace Tests

#endif
