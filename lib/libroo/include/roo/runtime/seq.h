
#ifndef ROO__RUNTIME__SEQ_H
#define ROO__RUNTIME__SEQ_H

#include <cstdint>
#include <string>

#include <roo/runtime/value.h>

namespace Roo
{
  sptr_val_v get_children(Value& v);
  sptr_val get_child(const Value& seq, size_t index);
  size_t child_count(const Value& v);
  bool has_indexed_children(const Value& v);

  template <typename Fn> void for_each_child(Value& seq, Fn&& fn)
  {
    if (has_indexed_children(seq))
    {
      const size_t size = child_count(seq);
      for (size_t i = 0; i < size; i++)
      {
        fn(get_child(seq, i));
      }
      return;
    }

    sptr_val_v children = get_children(seq);
    for (auto& child : children)
    {
      fn(child);
    }
  }

  std::int64_t checked_sequence_index(const Value& value, const std::string& description);
  size_t normalized_sequence_index(std::int64_t index, size_t size);
  sptr_val_v get_child_range(Value& sequence, size_t start, size_t end);
  void set_child(Value& seq, size_t index, const sptr_val& value);
  void insert_values(Value& target, size_t position, sptr_val_v values);
  sptr_val pop_child(Value& seq);
  sptr_val peek_child(Value& seq);
  size_t count(const Value& v);
  void append(Value& seq, const sptr_val& element);
} // namespace Roo

#endif /* ROO__RUNTIME__SEQ_H */
