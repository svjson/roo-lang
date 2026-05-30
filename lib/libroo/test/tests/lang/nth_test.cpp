
#include <vector>

#include <roo/adapter.h>
#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using NthFunction = RooTest::RuntimeTestFixture;

namespace
{
  class NoBulkNativeVectorAdapter : public Roo::NativeStdVectorAdapter<int>
  {
   public:
    explicit NoBulkNativeVectorAdapter(std::vector<int>& values)
      : Roo::NativeStdVectorAdapter<int>(values)
    {
    }

    Roo::sptr_val_v native_children() const override
    {
      throw Roo::RooException("native_children should not be used for indexed access");
    }
  };
} // namespace

TEST_F(NthFunction, nth_valid_numbers)
{
  // Given
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 0)")->i64(), 0);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 1)")->i64(), 1);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 2)")->i64(), 2);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 3)")->i64(), 3);
  EXPECT_EQ(runtime.eval("(nth [0 1 2 3 4] 4)")->i64(), 4);
}

TEST_F(NthFunction, nth_nil_target_returns_nil_when_index_is_number)
{
  EXPECT_EQ(*runtime.eval("(nth nil 0)"), *Roo::Constant::NIL);
}

TEST_F(NthFunction, nth_nil_index_throws_type_error)
{
  EXPECT_THROW(runtime.eval("(nth nil nil)"), Roo::TypeError);
  EXPECT_THROW(runtime.eval("(nth [1 2 3] nil)"), Roo::TypeError);
}

TEST_F(NthFunction, nth_non_number_index_is_rejected_by_signature)
{
  EXPECT_THROW(runtime.eval("(nth [1 2 3] :x)"), Roo::InvocationException);
  EXPECT_THROW(runtime.eval("(nth nil :x)"), Roo::InvocationException);
}

TEST_F(NthFunction, nth_negative_index_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(nth [1 2 3] -1)"), *Roo::Constant::NIL);
}

TEST_F(NthFunction, nth_out_of_bounds_index_returns_nil)
{
  EXPECT_EQ(*runtime.eval("(nth [1 2 3] 8)"), *Roo::Constant::NIL);
}

TEST_F(NthFunction, nth_string_as_char_sequence)
{
  EXPECT_EQ(runtime.eval(R"((nth "abc" 1))")->to_string(), "'b'");
}

TEST_F(NthFunction, nth_map_as_interleaved_sequence)
{
  EXPECT_EQ(runtime.eval("(nth {:a 1 :b 2} 0)")->to_string(), ":a");
  EXPECT_EQ(runtime.eval("(nth {:a 1 :b 2} 1)")->to_string(), "1");
  EXPECT_EQ(runtime.eval("(nth {:a 1 :b 2} 2)")->to_string(), ":b");
  EXPECT_EQ(runtime.eval("(nth {:a 1 :b 2} 3)")->to_string(), "2");
}

TEST_F(NthFunction, nth_native_vector_adapter_as_sequence)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store("values",
                                        Roo::NativeStdVectorAdapter<int>::make_ref(values));

  EXPECT_EQ(runtime.eval("(nth values 1)")->to_string(), "2");
}

TEST_F(NthFunction, nth_native_vector_adapter_uses_direct_index_access)
{
  std::vector<int> values = {1, 2, 3};
  runtime.get_current_namespace().store(
    "values",
    Roo::Value::native_object(std::make_shared<NoBulkNativeVectorAdapter>(values)));

  EXPECT_EQ(runtime.eval("(nth values 1)")->to_string(), "2");
  EXPECT_EQ(*runtime.eval("(nth values 8)"), *Roo::Constant::NIL);
}
