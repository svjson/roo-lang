
#include "lisple/struct.h"

#include "lisple/runtime/dict.h"

#include <map>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/type.h>

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

  void MapStruct::validate(const std::string& ctx_str, AST::ASTNode& map_obj) const
  {
    if (auto* wrapper = dynamic_cast<AST::RuntimeValueWrapper*>(&map_obj))
    {
      for (auto key : Lisple::Dict::map_sptr_keys(wrapper->val))
      {
        if (!valid_keys.count(key->to_string()))
        {
          throw LispleException(ctx_str + " - Unexpected key: '" + key->to_string() + "'.");
        }

        auto val = Lisple::Dict::get_property(wrapper->val, *key);
        if (!valid_keys.at(key->to_string()).value_type->is_type_of(*val))
        {
          throw LispleException("Invalid value '" + val->to_string() + "' for property " +
                                key->to_string() + " in " + map_obj.to_string());
        }
      }
    }
    else
    {
      AST::Map& map = map_obj.as<AST::Map>();

      for (AST::ASTNode* key : map.keys())
      {
        if (!valid_keys.count(key->to_string()))
        {
          throw LispleException(ctx_str + " - Unexpected key: '" + key->to_string() + "'.");
        }

        auto& val = map.get_property(*key);
        if (!valid_keys.at(key->to_string()).value_type->is_type_of(val))
        {
          throw LispleException("Invalid value '" + val.to_string() + "' for property " +
                                key->to_string() + " in " + map.to_string());
        }
      }
    }
  }
} // namespace Lisple
