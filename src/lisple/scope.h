
#ifndef __SCOPE_H_
#define __SCOPE_H_

#include <map>
#include <memory>
#include <string>

#include "form.h"
#include "type.h"

namespace Lisple
{
  class Scope
  {
   protected:
    std::map<std::string, Lisple::sptr_sobject> objects;

  public:
    virtual ~Scope() = default;

    void store(const Lisple::Word& identifier, const Lisple::sptr_sobject& obj);
    virtual void mutate(const Lisple::Word& identifier, const Lisple::sptr_sobject& obj);
    void remove(const Lisple::Word& identifier);
    virtual bool has(const Lisple::Word& identifier) const;
    virtual std::shared_ptr<Lisple::Object> lookup(const Lisple::Word& identifier) const;
    std::shared_ptr<Lisple::Array> get_keys();
  };
}

#endif
