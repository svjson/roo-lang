
#ifndef ROO__POOL_H
#define ROO__POOL_H

#include <unordered_map>

#include <roo/runtime/value.h>

namespace Roo
{
  class IntegerPool
  {
   public:
    static sptr_val get(int value);

   private:
    static constexpr int MIN = -256;
    static constexpr int MAX = 1024;

    static sptr_val_v& pool();
  };

  class KeywordPool
  {
   public:
    static sptr_val get(const std::string& keyword);

   private:
    static std::unordered_map<std::string, sptr_val> pool;
  };
} // namespace Roo

#endif /* ROO__POOL_H */
