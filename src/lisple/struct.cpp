
#include "struct.h"

#include <map>
#include <memory>

#include "form.h"
#include "type.h"
#include "exception.h"

namespace Lisple
{
  MapEntryReq::MapEntryReq(const TypeRef* value_type, bool req)
    : value_type(value_type)
    , required(req)
  {
  }

  MapStruct::MapStruct(std::map<const std::string, MapEntryReq> valid_keys)
    : valid_keys(valid_keys)
  {
  }

  void MapStruct::validate(const std::string& ctx_str, Object& map_obj) const
  {
    Map& map = map_obj.as<Map>();

    for (Object* key : map.keys())
    {
      if (!valid_keys.count(key->to_string()))
      {
        throw LispleException(ctx_str + " - Unexpected key: '" + key->to_string() + "'.");
      }

      auto& val = map.get_property(*key);
      if (!valid_keys.at(key->to_string()).value_type->is_type_of(val))
      {
        throw LispleException("Invalid value '" + val.to_string() + "' for property " + key->to_string() + " in " + map.to_string());
      }
    }
  }
}

