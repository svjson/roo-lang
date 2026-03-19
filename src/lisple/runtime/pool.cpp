#include "pool.h"

#include <unordered_map>

#include "lisple/runtime/value.h"

namespace Lisple
{
  sptr_rtval IntegerPool::get(int value)
  {
    if (value >= MIN && value <= MAX) return pool()[static_cast<size_t>(value - MIN)];

    return std::make_shared<RTValue>(value);
  }

  sptr_rtval_v& IntegerPool::pool()
  {
    static sptr_rtval_v values = []
    {
      sptr_rtval_v v;
      v.reserve(static_cast<size_t>(MAX - MIN + 1));

      for (int i = MIN; i <= MAX; ++i)
        v.push_back(std::make_shared<RTValue>(i));

      return v;
    }();

    return values;
  }

  sptr_rtval KeywordPool::get(const std::string& kw)
  {
    auto it = pool.find(kw);
    if (it != pool.end()) return it->second;

    auto [inserted_it, _] =
      pool.emplace(kw, std::make_shared<RTValue>(kw, RTValue::Type::KEYWORD));
    return inserted_it->second;
  }

  std::unordered_map<std::string, sptr_rtval> KeywordPool::pool = {};
} // namespace Lisple
