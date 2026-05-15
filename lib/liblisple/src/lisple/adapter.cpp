
#include "lisple/adapter.h"

namespace Lisple
{
  StdVectorTraits::StdVectorTraits(const HostTypeRef* type_ref, const TypeRef* value_type)
    : type_ref(type_ref)
    , value_type(value_type)
  {
  }

  StdMapTraits::StdMapTraits(const HostTypeRef* type_ref,
                             const TypeRef* key_type,
                             const TypeRef* value_type)
    : type_ref(type_ref)
    , key_type(key_type)
    , value_type(value_type)
  {
  }
} // namespace Lisple
