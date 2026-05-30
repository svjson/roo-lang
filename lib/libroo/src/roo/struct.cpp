
#include "roo/struct.h"

#include <map>

#include "roo/runtime/dict.h"
#include <roo/exception.h>
#include <roo/form.h>
#include <roo/type.h>

namespace Roo
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
      for (auto key : Roo::Dict::map_sptr_keys(wrapper->val))
      {
        if (!valid_keys.count(key->to_string()))
        {
          throw RooException(ctx_str + " - Unexpected key: '" + key->to_string() + "'.");
        }

        auto val = Roo::Dict::get_property(wrapper->val, *key);
        if (!valid_keys.at(key->to_string()).value_type->is_type_of(*val))
        {
          throw RooException("Invalid value '" + val->to_string() + "' for property " +
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
          throw RooException(ctx_str + " - Unexpected key: '" + key->to_string() + "'.");
        }

        auto& val = map.get_property(*key);
        if (!valid_keys.at(key->to_string()).value_type->is_type_of(val))
        {
          throw RooException("Invalid value '" + val.to_string() + "' for property " +
                             key->to_string() + " in " + map.to_string());
        }
      }
    }
  }
} // namespace Roo
