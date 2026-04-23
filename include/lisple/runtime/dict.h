
#ifndef LISPLE__RUNTIME__DICT_H
#define LISPLE__RUNTIME__DICT_H

#include <unordered_set>

#include <lisple/form.h>
#include <lisple/runtime/value.h>

namespace Lisple::Dict
{
  bool contains_key(RTValue& source, const std::string& key);

  void set_property(sptr_rtval& target, const sptr_rtval& property, const sptr_rtval& value);

  sptr_rtval remove_property(sptr_rtval& target, const sptr_rtval& property);

  const std::vector<sptr_rtval> keys(const RTValue& dict);
  const std::vector<const RTValue*> map_keys(const std::vector<RTValue>& map_data);
  const std::vector<const RTValue*> map_keys(const sptr_rtval_v& map_data);
  const std::vector<const RTValue*> map_keys(const RTValue& map_data);
  std::vector<sptr_rtval> map_sptr_keys(const sptr_rtval& dict);
  std::unordered_set<std::string> map_string_keys(RTValue& map);

  sptr_rtval get_property_or_throw(RTValue& source, const std::string& keyword);
  sptr_rtval get_property(RTValue& object, const std::string& property);
  sptr_rtval get_property(const sptr_rtval& object, const sptr_rtval& property);
  sptr_rtval get_property(const sptr_rtval& object, const RTValue& property);

  sptr_rtval get_property_path(const sptr_rtval& object, const sptr_rtval_v& path);

  std::pair<const sptr_rtval, const sptr_rtval> map_entry(const sptr_rtval_v& map_data,
                                                          const RTValue& key);
  std::pair<sptr_rtval, sptr_rtval> map_entry(sptr_rtval_v& map_data, const RTValue& key);

  sptr_rtval shallow_copy(const sptr_rtval& source);
  sptr_rtval assoc_in(const sptr_rtval& source,
                      const sptr_rtval_v& path,
                      const sptr_rtval& value);

} // namespace Lisple::Dict

#endif /* LISPLE__RUNTIME__DICT_H */
