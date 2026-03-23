
#ifndef LISPLE__RUNTIME__SEQ_H
#define LISPLE__RUNTIME__SEQ_H

#include "value.h"

namespace Lisple
{
  sptr_rtval_v get_children(RTValue& v);
  size_t count(RTValue& v);
} // namespace Lisple

#endif /* LISPLE__RUNTIME__SEQ_H */
