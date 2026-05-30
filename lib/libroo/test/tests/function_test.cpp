
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>
#include <roo/exec.h>
#include <roo/form.h>
#include <roo/type.h>

namespace Roo
{
  class Context;
}

using Function = RooTest::RuntimeTestFixture;

Roo::sptr_val dummy_exec1(Roo::Context&, Roo::sptr_val_v&)
{
  return Roo::Value::string("retval1");
}

Roo::sptr_val dummy_exec2(Roo::Context&, Roo::sptr_val_v&)
{
  return Roo::Value::string("retval2");
}

TEST(FunctionSignature, matches__two_strings)
{
  // Given
  Roo::sig signature(Roo::arg_v{Roo::arg(&Roo::Type::STRING), Roo::arg(&Roo::Type::STRING)},
                     std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2));

  Roo::sptr_val_v args;
  args.push_back(Roo::Value::string("string1"));
  args.push_back(Roo::Value::string("string2"));

  // Then
  EXPECT_TRUE(signature.matches(args));
}

TEST_F(Function, execute__delegate)
{
  // Given
  Roo::uptr_sig_v signatures;

  signatures.push_back(std::make_unique<Roo::sig>(
    Roo::arg_v{Roo::arg(&Roo::Type::STRING)},
    std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2)));

  signatures.push_back(std::make_unique<Roo::sig>(
    Roo::arg_v{Roo::arg(&Roo::Type::STRING), Roo::arg(&Roo::Type::STRING)},
    std::bind(&dummy_exec2, std::placeholders::_1, std::placeholders::_2)));

  Roo::Function fn(std::move(signatures));

  // When
  Roo::sptr_val_v args;
  args.push_back(Roo::Value::string("string1"));

  Roo::sptr_val retval = fn.execute(ctx, args);

  // Then
  EXPECT_TRUE(Roo::Type::STRING.is_type_of(*retval));
  EXPECT_EQ(retval->str(), "retval1");
}
