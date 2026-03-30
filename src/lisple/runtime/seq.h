
#ifndef LISPLE__RUNTIME__SEQ_H
#define LISPLE__RUNTIME__SEQ_H

#include "value.h"

namespace Lisple
{
  sptr_rtval_v get_children(RTValue& v);
  sptr_rtval get_child(RTValue& seq, size_t index);
  sptr_rtval pop_child(RTValue& seq);
  size_t count(RTValue& v);
  void append(RTValue& seq, const sptr_rtval& element);
} // namespace Lisple

#endif /* LISPLE__RUNTIME__SEQ_H */
