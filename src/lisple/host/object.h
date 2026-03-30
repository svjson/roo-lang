
#ifndef LISPLE__HOST__OBJECT_H
#define LISPLE__HOST__OBJECT_H

#include "../form.h"
#include "../runtime/value.h"

namespace Lisple
{
  template <typename T> T& obj(const RTValue& v)
  {
    return v.obj()->as<HostObject<T>>().get_object();
  }
} // namespace Lisple

#endif /* LISPLE__HOST__OBJECT_H */
