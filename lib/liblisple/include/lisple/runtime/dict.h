
#ifndef LISPLE__RUNTIME__DICT_H
#define LISPLE__RUNTIME__DICT_H

#include <unordered_set>

#include <lisple/form.h>
#include <lisple/runtime/value.h>

namespace Lisple::Dict
{
  bool contains_key(Value& source, const std::string& key);

  void set_property(sptr_val& target, const sptr_val& property, const sptr_val& value);

  sptr_val remove_property(sptr_val& target, const sptr_val& property);

  const std::vector<sptr_val> keys(const Value& dict);
  const std::vector<const Value*> map_keys(const std::vector<Value>& map_data);
  const std::vector<const Value*> map_keys(const sptr_val_v& map_data);
  const std::vector<const Value*> map_keys(const Value& map_data);
  std::vector<sptr_val> map_sptr_keys(const sptr_val& dict);
  std::unordered_set<std::string> map_string_keys(Value& map);

  sptr_val get_property_or_throw(Value& source, const std::string& keyword);
  sptr_val get_property(Value& object, const std::string& property);
  sptr_val get_property(const sptr_val& object, const sptr_val& property);
  sptr_val get_property(const sptr_val& object, const Value& property);

  sptr_val get_property_path(const sptr_val& object, const sptr_val_v& path);

  std::pair<const sptr_val, const sptr_val> map_entry(const sptr_val_v& map_data,
                                                      const Value& key);
  std::pair<sptr_val, sptr_val> map_entry(sptr_val_v& map_data, const Value& key);

  sptr_val shallow_copy(const sptr_val& source);
  sptr_val assoc_in(const sptr_val& source, const sptr_val_v& path, const sptr_val& value);

} // namespace Lisple::Dict

#endif /* LISPLE__RUNTIME__DICT_H */
