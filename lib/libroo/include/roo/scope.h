
#ifndef __SCOPE_H_
#define __SCOPE_H_

#include <map>
#include <memory>
#include <string>

#include <roo/form.h>
#include <roo/type.h>

namespace Roo
{
  class Scope
  {
   protected:
    std::map<std::string, sptr_val> values;

   public:
    virtual ~Scope() = default;

    void store(const std::string& symbol, const sptr_val& value);

    virtual void mutate(const std::string& identifier, const sptr_val& val);
    void remove(const std::string& identifier);
    void clear();
    virtual bool has(const std::string& identifier) const;
    virtual const sptr_val* find(const std::string& identifier) const;
    virtual const sptr_val* find(const Value& identifier) const;
    virtual const sptr_val& lookup(const std::string& identifier) const;
    virtual const sptr_val& lookup(const Value& identifier) const;
    std::shared_ptr<AST::Vector> get_keys() const;
  };
} // namespace Roo

#endif
