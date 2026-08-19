#include <roo/exception.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using TtyP = RooTest::RuntimeTestFixture;

TEST_F(TtyP, returns_a_boolean)
{
  // Given/When
  const std::string result = runtime.eval(R"((roo.tty/tty?))")->to_string();

  // Then
  EXPECT_TRUE(result == "true" || result == "false");
}

TEST_F(TtyP, takes_no_arguments)
{
  // Then
  EXPECT_THROW(runtime.eval(R"((roo.tty/tty? "unexpected"))"), Roo::InvocationException);
}
