
#include <gtest/gtest.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/type.h>

#include "lisp_reader_fixture.h"

namespace Lisple { class Context; }

std::shared_ptr<Lisple::Object> dummy_exec1(Lisple::Context&, Lisple::sptr_sobject_v&)
{
  return std::make_shared<Lisple::String>("retval1");
}

std::shared_ptr<Lisple::Object> dummy_exec2(Lisple::Context&, Lisple::sptr_sobject_v&)
{
  return std::make_shared<Lisple::String>("retval2");
}


TEST(FunctionSignature, matches__two_strings)
{
  // Given
  Lisple::sig signature(Lisple::arg_v { Lisple::arg(&Lisple::Type::STRING),
                                        Lisple::arg(&Lisple::Type::STRING) },
    std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2));

  Lisple::sptr_sobject_v args;
  args.push_back(std::make_shared<Lisple::String>("string1"));
  args.push_back(std::make_shared<Lisple::String>("string2"));

  // Then
  EXPECT_TRUE(signature.matches(args));
}

TEST(Function, execute__delegate)
{
  // Given
  LispleTest::LispReaderFixture fixture;
  Lisple::uptr_sig_v signatures;

  signatures.push_back(std::make_unique<Lisple::sig>
                       (Lisple::arg_v{ Lisple::arg(&Lisple::Type::STRING) },
                        std::bind(&dummy_exec1, std::placeholders::_1, std::placeholders::_2)));

  signatures.push_back(std::make_unique<Lisple::sig>
                       (Lisple::arg_v{
                         Lisple::arg(&Lisple::Type::STRING),
                         Lisple::arg(&Lisple::Type::STRING) },
                         std::bind(&dummy_exec2, std::placeholders::_1, std::placeholders::_2)));

  Lisple::Function fn(std::move(signatures));

  // When
  Lisple::sptr_sobject_v args;
  args.push_back(std::make_unique<Lisple::String>("string1"));

  std::shared_ptr<Lisple::Object> retval = fn.execute(fixture.ctx, args);

  // Then
  EXPECT_TRUE(Lisple::Type::STRING.is_type_of(*retval));
  EXPECT_EQ(Lisple::Value<std::string>::value_of(*retval), "retval1");
}
