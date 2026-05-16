
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/type.h>

#include "runtime_fixture.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <gtest/gtest_pred_impl.h>

namespace Lisple
{
  class Context;
}

using Function = LispleTest::RuntimeTestFixture;

Lisple::sptr_val dummy_exec1(Lisple::Context&, Lisple::sptr_val_v&)
{
  return Lisple::Value::string("retval1");
}

Lisple::sptr_val dummy_exec2(Lisple::Context&, Lisple::sptr_val_v&)
{
  return Lisple::Value::string("retval2");
}

TEST(FunctionSignature, matches__two_strings)
{
  // Given
  Lisple::sig signature(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING), Lisple::arg(&Lisple::Type::STRING)},
    std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2));

  Lisple::sptr_val_v args;
  args.push_back(Lisple::Value::string("string1"));
  args.push_back(Lisple::Value::string("string2"));

  // Then
  EXPECT_TRUE(signature.matches(args));
}

TEST_F(Function, execute__delegate)
{
  // Given
  Lisple::uptr_sig_v signatures;

  signatures.push_back(std::make_unique<Lisple::sig>(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING)},
    std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2)));

  signatures.push_back(std::make_unique<Lisple::sig>(
    Lisple::arg_v{Lisple::arg(&Lisple::Type::STRING), Lisple::arg(&Lisple::Type::STRING)},
    std::bind(&dummy_exec2, std::placeholders::_1, std::placeholders::_2)));

  Lisple::Function fn(std::move(signatures));

  // When
  Lisple::sptr_val_v args;
  args.push_back(Lisple::Value::string("string1"));

  Lisple::sptr_val retval = fn.execute(ctx, args);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*retval));
  EXPECT_EQ(retval->str(), "retval1");
}
