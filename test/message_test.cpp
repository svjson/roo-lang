
#include <gtest/gtest.h>

#include "message.h"

TEST(Message, encode)
{
  ASSERT_EQ(Lisple::Server::MessageBuilder::build_error_response({ 0x00, "It's all gone weird!" }).encode(),
            "!MSG\x1E!RESP\x1E@BODY=It's all gone weird!\x1E@CODE=0\x1E@ID=err\x1E/RESP\x1E/MSG\x1E");
}
