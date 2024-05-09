
#ifndef __STRUCT_H_
#define __STRUCT_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "form.h"
#include "lisple_exception.h"

namespace Lisple
{
  class TypeRef;

  class MapEntryReq
  {
   public:
    const TypeRef* value_type;
    bool required;

    MapEntryReq(const TypeRef* value_type, bool req);
  };

  class MapStruct
  {
    std::map<const std::string, MapEntryReq> valid_keys;

   public:
    MapStruct(std::map<const std::string, MapEntryReq> valid_keys);

    void validate(const std::string& ctx_str, Lisple::Object& map) const;

    template <class T>
    std::shared_ptr<T> get_value(Lisple::Object& map_obj, const Lisple::Object& key) const
    {
      Lisple::Map& map = map_obj.as<Lisple::Map>();
      for (auto keyptr : map.keys())
      {
        if (*keyptr == key)
        {
          return std::dynamic_pointer_cast<T>(map.get_sptr_property(key));
        }
      }

      if (valid_keys.at(key.to_string()).required)
      {
        throw Lisple::LispleException("Key " + key.to_string() + " is required, but not present in " + map.to_string());
      }

      return std::shared_ptr<T>();
    }
  };
}

#endif
