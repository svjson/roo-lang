
#include "lisple/adapter.h"

#include <lisple/host.h>

namespace Lisple
{
  StdVectorTraits::StdVectorTraits(const HostTypeRef* type_ref, const TypeRef* value_type)
    : AdapterTraits(type_ref, Lisple::NO_ACCESSORS)
    , value_type(value_type)
  {
  }

  StdMapTraits::StdMapTraits(const HostTypeRef* type_ref,
                             const TypeRef* key_type,
                             const TypeRef* value_type)
    : AdapterTraits(type_ref, Lisple::NO_ACCESSORS)
    , key_type(key_type)
    , value_type(value_type)
  {
  }
} // namespace Lisple
