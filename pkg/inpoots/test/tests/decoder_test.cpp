#include <roo/exception.h>

#include <gtest/gtest.h>
#include <terminal/decoder.h>

using Roo::Inpoots::Terminal::Decoder;
using Roo::Inpoots::Terminal::EventType;

TEST(InpootsDecoder, DecodesTextAndControlKeys)
{
  Decoder decoder;
  decoder.append("a\r\x7f\x03\x04");

  auto text = decoder.next();
  auto enter = decoder.next();
  auto backspace = decoder.next();
  auto interrupt = decoder.next();
  auto eof = decoder.next();

  ASSERT_TRUE(text);
  EXPECT_EQ(text->type, EventType::TEXT);
  EXPECT_EQ(text->text, "a");
  ASSERT_TRUE(enter);
  EXPECT_EQ(enter->key, "enter");
  ASSERT_TRUE(backspace);
  EXPECT_EQ(backspace->key, "backspace");
  ASSERT_TRUE(interrupt);
  EXPECT_EQ(interrupt->type, EventType::INTERRUPT);
  ASSERT_TRUE(eof);
  EXPECT_EQ(eof->type, EventType::EOF_EVENT);
}

TEST(InpootsDecoder, WaitsForCompleteUtf8Text)
{
  Decoder decoder;
  decoder.append("\xc3");
  EXPECT_FALSE(decoder.next());

  decoder.append("\xa5");
  auto event = decoder.next();

  ASSERT_TRUE(event);
  EXPECT_EQ(event->type, EventType::TEXT);
  EXPECT_EQ(event->text, "å");
}

TEST(InpootsDecoder, RejectsInvalidUtf8Text)
{
  Decoder decoder;
  decoder.append("\xc3x");

  EXPECT_THROW(decoder.next(), Roo::RooException);
}

TEST(InpootsDecoder, DecodesNamedKeysAndXtermModifiers)
{
  Decoder decoder;
  decoder.append("\x1b[A\x1b[1;5C\x1b[3~");

  auto up = decoder.next();
  auto right = decoder.next();
  auto delete_key = decoder.next();

  ASSERT_TRUE(up);
  EXPECT_EQ(up->key, "up");
  ASSERT_TRUE(right);
  EXPECT_EQ(right->key, "right");
  EXPECT_EQ(right->modifiers, std::vector<std::string>({"control"}));
  ASSERT_TRUE(delete_key);
  EXPECT_EQ(delete_key->key, "delete");
}

TEST(InpootsDecoder, DistinguishesEscapeFromAnEscapeSequence)
{
  Decoder decoder;
  decoder.append("\x1b");

  EXPECT_FALSE(decoder.next());
  auto event = decoder.flush_escape();

  ASSERT_TRUE(event);
  EXPECT_EQ(event->type, EventType::KEY);
  EXPECT_EQ(event->key, "escape");
}

TEST(InpootsDecoder, CollectsFragmentedBracketedPaste)
{
  Decoder decoder;
  decoder.append("\x1b[200~(+ 1");
  EXPECT_FALSE(decoder.next());

  decoder.append(" 2)\x1b[20");
  EXPECT_FALSE(decoder.next());

  decoder.append("1~z");
  auto paste = decoder.next();
  auto text = decoder.next();

  ASSERT_TRUE(paste);
  EXPECT_EQ(paste->type, EventType::PASTE);
  EXPECT_EQ(paste->text, "(+ 1 2)");
  ASSERT_TRUE(text);
  EXPECT_EQ(text->text, "z");
}

TEST(InpootsDecoder, RejectsInvalidUtf8InBracketedPaste)
{
  Decoder decoder;
  decoder.append("\x1b[200~\xc3x\x1b[201~");

  EXPECT_THROW(decoder.next(), Roo::RooException);
}

TEST(InpootsDecoder, PreservesUnknownEscapeSequencesAsKeyEvents)
{
  Decoder decoder;
  decoder.append("\x1b[99~");
  auto event = decoder.next();

  ASSERT_TRUE(event);
  EXPECT_EQ(event->type, EventType::KEY);
  EXPECT_EQ(event->key, "unknown");
  EXPECT_EQ(event->sequence, "\x1b[99~");
}

TEST(InpootsDecoder, DoesNotInterpretPrivateCsiSequencesAsNamedKeys)
{
  Decoder decoder;
  decoder.append("\x1b[?A");
  auto event = decoder.next();

  ASSERT_TRUE(event);
  EXPECT_EQ(event->key, "unknown");
  EXPECT_EQ(event->sequence, "\x1b[?A");
}
