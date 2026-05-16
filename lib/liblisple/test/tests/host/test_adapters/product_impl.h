#ifndef LISPLETEST__HOST__PRODUCT_IMPL_H
#define LISPLETEST__HOST__PRODUCT_IMPL_H

#include <string>

namespace LispleTest
{
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

} // namespace LispleTest

#endif /* PRODUCT_IMPL_H */
