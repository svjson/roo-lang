
#ifndef __SCOPE_H_
#define __SCOPE_H_

#include <map>
#include <memory>
#include <string>

#include <lisple/form.h>
#include <lisple/type.h>

namespace Lisple
{
  class Scope
  {
   protected:
    std::map<std::string, sptr_rtval> values;

   public:
    virtual ~Scope() = default;

    void store(const std::string& symbol, const sptr_rtval& value);

    virtual void mutate(const std::string& identifier, const sptr_rtval& val);
    void remove(const std::string& identifier);
    void clear();
    virtual bool has(const std::string& identifier) const;
    virtual sptr_rtval lookup(const std::string& identifier) const;
    virtual sptr_rtval lookup(const RTValue& identifier) const;
    std::shared_ptr<Vector> get_keys() const;
  };
} // namespace Lisple

#endif
