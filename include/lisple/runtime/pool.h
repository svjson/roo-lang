
#ifndef LISPLE__POOL_H
#define LISPLE__POOL_H

#include <unordered_map>

#include <lisple/runtime/value.h>

namespace Lisple
{
  class IntegerPool
  {
   public:
    static sptr_rtval get(int value);

   private:
    static constexpr int MIN = -256;
    static constexpr int MAX = 1024;

    static sptr_rtval_v& pool();
  };

  class KeywordPool
  {
   public:
    static sptr_rtval get(const std::string& keyword);

   private:
    static std::unordered_map<std::string, sptr_rtval> pool;
  };
} // namespace Lisple

#endif /* LISPLE__POOL_H */
