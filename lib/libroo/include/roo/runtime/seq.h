
#ifndef ROO__RUNTIME__SEQ_H
#define ROO__RUNTIME__SEQ_H

#include <roo/runtime/value.h>

namespace Roo
{
  sptr_val_v get_children(Value& v);
  sptr_val get_child(const Value& seq, size_t index);
  size_t child_count(const Value& v);
  bool has_indexed_children(const Value& v);
  void set_child(Value& seq, size_t index, const sptr_val& value);
  sptr_val pop_child(Value& seq);
  sptr_val peek_child(Value& seq);
  size_t count(const Value& v);
  void append(Value& seq, const sptr_val& element);
} // namespace Roo

#endif /* ROO__RUNTIME__SEQ_H */
