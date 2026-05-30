#include "roo/runtime/pool.h"

#include <unordered_map>

#include <roo/runtime/value.h>

namespace Roo
{
  sptr_val IntegerPool::get(int value)
  {
    if (value >= MIN && value <= MAX) return pool()[static_cast<size_t>(value - MIN)];

    return std::make_shared<Value>(value);
  }

  sptr_val_v& IntegerPool::pool()
  {
    static sptr_val_v values = []
    {
      sptr_val_v v;
      v.reserve(static_cast<size_t>(MAX - MIN + 1));

      for (int i = MIN; i <= MAX; ++i)
        v.push_back(std::make_shared<Value>(i));

      return v;
    }();

    return values;
  }

  sptr_val KeywordPool::get(const std::string& kw)
  {
    static std::unordered_map<std::string, sptr_val> pool;

    auto it = pool.find(kw);
    if (it != pool.end()) return it->second;

    auto [inserted_it, _] =
      pool.emplace(kw, std::make_shared<Value>(kw, Value::Type::KEYWORD));
    return inserted_it->second;
  }

} // namespace Roo
