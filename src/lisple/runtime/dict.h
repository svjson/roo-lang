
#ifndef LISPLE__RUNTIME__DICT_H
#define LISPLE__RUNTIME__DICT_H

#include "../form.h"

namespace Lisple::Dict
{
  void set_property(sptr_rtval& target, const sptr_rtval& property, sptr_rtval& value);
  sptr_rtval remove_property(sptr_rtval& target, const sptr_rtval& property);

  const std::vector<const RTValue*> map_keys(const std::vector<RTValue>& map_data);

  sptr_rtval get_property(sptr_rtval& object, const sptr_rtval& property);
  std::pair<const sptr_rtval, const sptr_rtval> map_entry(const sptr_rtval_v& map_data,
                                                          const RTValue& key);
  std::pair<sptr_rtval, sptr_rtval> map_entry(sptr_rtval_v& map_data, const RTValue& key);

} // namespace Lisple::Dict

#endif /* LISPLE__RUNTIME__DICT_H */
