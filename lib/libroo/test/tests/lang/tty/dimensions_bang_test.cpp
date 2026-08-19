#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using DimensionsBang = RooTest::RuntimeTestFixture;

TEST_F(DimensionsBang, matches_documented_contract_for_current_tty_state)
{
  // dimensions! has no injectable abstraction to fake stdout's tty state (unlike
  // roo.io's FileSystem), so this asserts the documented contract against whatever
  // stdout actually is in the environment running this test, rather than a fixed
  // expected value.

  // Given
  const bool is_tty = runtime.eval(R"((roo.tty/tty?))")->to_string() == "true";

  // When/Then
  if (!is_tty)
  {
    EXPECT_EQ(runtime.eval(R"((roo.tty/dimensions!))")->to_string(), "nil");
    return;
  }

  EXPECT_EQ(runtime
              .eval(R"(
                (and (map? (roo.tty/dimensions!))
                     (> (:columns (roo.tty/dimensions!)) 0)
                     (> (:rows (roo.tty/dimensions!)) 0)))")
              ->to_string(),
            "true");
}

TEST_F(DimensionsBang, takes_no_arguments)
{
  // Then
  EXPECT_THROW(runtime.eval(R"((roo.tty/dimensions! "unexpected"))"), Roo::InvocationException);
}
