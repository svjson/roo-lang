
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
    std::map<std::string, sptr_sobject> objects;
    std::map<std::string, sptr_rtval> values;

   public:
    virtual ~Scope() = default;

    void store(const Word& identifier, const sptr_sobject& obj);
    void store(const std::string& identifier, const sptr_sobject& obj);
    void store(const std::string& symbol, const sptr_rtval& value);

    virtual void mutate(const Word& identifier, const sptr_sobject& obj);
    void remove(const Word& identifier);
    void clear();
    virtual bool has(const Word& identifier) const;
    virtual std::shared_ptr<Object> lookup(const Word& identifier) const;
    virtual sptr_rtval lookup_value(const std::string& identifier) const;
    std::shared_ptr<Array> get_keys() const;
  };
} // namespace Lisple

#endif
