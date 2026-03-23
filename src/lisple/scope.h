
#ifndef __SCOPE_H_
#define __SCOPE_H_

#include "form.h"
#include "type.h"
#include <map>
#include <memory>
#include <string>

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
    virtual sptr_rtval lookup(const std::string& identifier) const;
    std::shared_ptr<Array> get_keys();
  };
} // namespace Lisple

#endif
