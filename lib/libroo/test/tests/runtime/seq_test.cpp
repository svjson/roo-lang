#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include <roo/host/std_adapter.h>
#include <roo/exception.h>
#include <roo/runtime/seq.h>
#include <roo/runtime/value.h>

#include <gtest/gtest.h>

namespace
{
  class IndexedOnlyNativeVectorAdapter : public Roo::NativeStdVectorAdapter<int>
  {
   public:
    explicit IndexedOnlyNativeVectorAdapter(std::vector<int>& values)
      : Roo::NativeStdVectorAdapter<int>(values)
    {
    }

    Roo::sptr_val_v native_children() const override
    {
      throw Roo::RooException("sequence traversal should use indexed access");
    }
  };
} // namespace

TEST(RuntimeSeq, checked_sequence_index_accepts_fixed_width_integers)
{
  EXPECT_EQ(Roo::checked_sequence_index(*Roo::Value::number(2), "index"), 2);
  EXPECT_EQ(Roo::checked_sequence_index(*Roo::Value::number(2147483648LL), "index"),
            INT64_C(2147483648));
  EXPECT_EQ(
    Roo::checked_sequence_index(*Roo::Value::number(Roo::Value::Number{
                                  .num_type = Roo::Value::NumberType::LONG,
                                  .long_value = std::numeric_limits<std::int64_t>::min()}),
                                "index"),
    std::numeric_limits<std::int64_t>::min());
}

TEST(RuntimeSeq, checked_sequence_index_rejects_fractional_values)
{
  EXPECT_THROW(Roo::checked_sequence_index(*Roo::Value::number(1.5), "index"),
               Roo::TypeError);
}

TEST(RuntimeSeq, normalized_sequence_index_resolves_and_clamps_offsets)
{
  EXPECT_EQ(Roo::normalized_sequence_index(2, 5), 2);
  EXPECT_EQ(Roo::normalized_sequence_index(20, 5), 5);
  EXPECT_EQ(Roo::normalized_sequence_index(-1, 5), 4);
  EXPECT_EQ(Roo::normalized_sequence_index(-5, 5), 0);
  EXPECT_EQ(Roo::normalized_sequence_index(-20, 5), 0);
  EXPECT_EQ(Roo::normalized_sequence_index(std::numeric_limits<std::int64_t>::min(), 5), 0);
}

TEST(RuntimeSeq, get_child_range_clamps_bounds)
{
  Roo::sptr_val sequence = Roo::Value::vector(
    {Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)});

  EXPECT_EQ(*Roo::Value::vector(Roo::get_child_range(*sequence, 1, 20)),
            *Roo::Value::vector({Roo::Value::number(2), Roo::Value::number(3)}));
  EXPECT_EQ(*Roo::Value::vector(Roo::get_child_range(*sequence, 2, 1)),
            *Roo::Value::vector({}));
}

TEST(RuntimeSeq, traversal_uses_indexed_native_sequence_access)
{
  std::vector<int> values = {1, 2, 3, 4};
  Roo::sptr_val sequence =
    Roo::Value::native_object(std::make_shared<IndexedOnlyNativeVectorAdapter>(values));

  EXPECT_EQ(*Roo::Value::vector(Roo::get_child_range(*sequence, 1, 3)),
            *Roo::Value::vector({Roo::Value::number(2), Roo::Value::number(3)}));

  Roo::sptr_val_v traversed;
  Roo::for_each_child(*sequence,
                      [&](const Roo::sptr_val& value) { traversed.push_back(value); });
  EXPECT_EQ(*Roo::Value::vector(traversed),
            *Roo::Value::vector({Roo::Value::number(1),
                                 Roo::Value::number(2),
                                 Roo::Value::number(3),
                                 Roo::Value::number(4)}));
}

TEST(RuntimeSeq, insert_values_mutates_vectors_lists_and_host_sequences)
{
  Roo::sptr_val vector = Roo::Value::vector({Roo::Value::number(1), Roo::Value::number(4)});
  Roo::insert_values(*vector, 1, {Roo::Value::number(2), Roo::Value::number(3)});
  EXPECT_EQ(vector->to_string(), "[1 2 3 4]");

  Roo::sptr_val list = Roo::Value::list({Roo::Value::number(1), Roo::Value::number(4)});
  Roo::insert_values(*list, 20, {Roo::Value::number(2), Roo::Value::number(3)});
  EXPECT_EQ(list->to_string(), "(1 4 2 3)");

  std::vector<int> native_values = {1, 4};
  Roo::sptr_val host_sequence = Roo::NativeStdVectorAdapter<int>::make_ref(native_values);
  Roo::insert_values(*host_sequence, 1, {Roo::Value::number(2), Roo::Value::number(3)});
  EXPECT_EQ(native_values, (std::vector<int>{1, 2, 3, 4}));
}

TEST(RuntimeSeq, insert_values_snapshots_an_aliased_values_vector)
{
  Roo::sptr_val sequence =
    Roo::Value::vector({Roo::Value::number(1), Roo::Value::number(2)});
  Roo::sptr_val_v& values = std::get<Roo::sptr_val_v>(sequence->value);

  Roo::insert_values(*sequence, 1, values);

  EXPECT_EQ(sequence->to_string(), "[1 1 2 2]");
}

TEST(RuntimeSeq, replace_values_mutates_vectors_lists_and_host_sequences)
{
  const Roo::sptr_val_v replacements = {Roo::Value::number(8), Roo::Value::number(9)};

  Roo::sptr_val vector = Roo::Value::vector(
    {Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)});
  Roo::replace_values(*vector, 1, 3, replacements);
  EXPECT_EQ(vector->to_string(), "[1 8 9]");

  Roo::sptr_val list =
    Roo::Value::list({Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)});
  Roo::replace_values(*list, 1, 2, replacements);
  EXPECT_EQ(list->to_string(), "(1 8 9 3)");

  std::vector<int> native_values = {1, 2, 3, 4};
  Roo::sptr_val host_sequence = Roo::NativeStdVectorAdapter<int>::make_ref(native_values);
  Roo::replace_values(*host_sequence, 1, 3, replacements);
  EXPECT_EQ(native_values, (std::vector<int>{1, 8, 9, 4}));
}

TEST(RuntimeSeq, replace_values_snapshots_an_aliased_values_vector)
{
  Roo::sptr_val sequence = Roo::Value::vector(
    {Roo::Value::number(1), Roo::Value::number(2), Roo::Value::number(3)});
  Roo::sptr_val_v& values = std::get<Roo::sptr_val_v>(sequence->value);

  Roo::replace_values(*sequence, 1, 2, values);

  EXPECT_EQ(sequence->to_string(), "[1 1 2 3 3]");
}
