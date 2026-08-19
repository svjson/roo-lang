#ifndef _WIN32

#define _XOPEN_SOURCE 600

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <termios.h>
#include <unistd.h>

#include <roo/exception.h>
#include <roo/runtime.h>

#include <gtest/gtest.h>
#include <inpoots/native.h>
#include <terminal/input.h>

namespace
{
  class FileDescriptor
  {
   private:
    int value;

   public:
    explicit FileDescriptor(int value)
      : value(value)
    {
    }

    ~FileDescriptor()
    {
      if (value >= 0) close(value);
    }

    int get() const { return value; }
  };

  class ScopedStdin
  {
   private:
    int original;

   public:
    explicit ScopedStdin(int descriptor)
      : original(dup(STDIN_FILENO))
    {
      if (original < 0 || dup2(descriptor, STDIN_FILENO) < 0)
      {
        std::abort();
      }
    }

    ~ScopedStdin()
    {
      if (original < 0) return;
      if (dup2(original, STDIN_FILENO) < 0)
      {
        std::abort();
      }
      close(original);
    }
  };
} // namespace

using Roo::Inpoots::Terminal::EventType;
using Roo::Inpoots::Terminal::Input;
using Roo::Inpoots::Terminal::Mode;

TEST(InpootsInput, ReadsEventsAndEofFromCookedDescriptors)
{
  int descriptors[2];
  ASSERT_EQ(pipe(descriptors), 0);
  FileDescriptor input_descriptor(descriptors[0]);
  ASSERT_EQ(write(descriptors[1], "x\r\ny\x1b[D", 7), 7);
  ASSERT_EQ(close(descriptors[1]), 0);

  Input input(input_descriptor.get(), Mode::COOKED);
  auto text = input.read_event();
  auto enter = input.read_event();
  auto following_text = input.read_event();
  auto left = input.read_event();
  auto eof = input.read_event();

  EXPECT_EQ(text.type, EventType::TEXT);
  EXPECT_EQ(text.text, "x");
  EXPECT_EQ(enter.type, EventType::KEY);
  EXPECT_EQ(enter.key, "enter");
  EXPECT_EQ(following_text.type, EventType::TEXT);
  EXPECT_EQ(following_text.text, "y");
  EXPECT_EQ(left.type, EventType::KEY);
  EXPECT_EQ(left.key, "left");
  EXPECT_EQ(eof.type, EventType::EOF_EVENT);
}

TEST(InpootsInput, EmitsEscapeBeforeEof)
{
  int descriptors[2];
  ASSERT_EQ(pipe(descriptors), 0);
  FileDescriptor input_descriptor(descriptors[0]);
  ASSERT_EQ(write(descriptors[1], "\x1b", 1), 1);
  ASSERT_EQ(close(descriptors[1]), 0);

  Input input(input_descriptor.get(), Mode::COOKED);
  auto escape = input.read_event();
  auto eof = input.read_event();

  EXPECT_EQ(escape.type, EventType::KEY);
  EXPECT_EQ(escape.key, "escape");
  EXPECT_EQ(eof.type, EventType::EOF_EVENT);
}

TEST(InpootsInput, RawModeRejectsRedirectedInput)
{
  int descriptors[2];
  ASSERT_EQ(pipe(descriptors), 0);
  FileDescriptor input_descriptor(descriptors[0]);
  FileDescriptor output_descriptor(descriptors[1]);

  EXPECT_THROW(Input(input_descriptor.get(), Mode::RAW), Roo::RooException);
}

TEST(InpootsInput, RawModeIsRestoredWhenTheInputCloses)
{
  FileDescriptor master(posix_openpt(O_RDWR | O_NOCTTY));
  ASSERT_GE(master.get(), 0);
  ASSERT_EQ(grantpt(master.get()), 0);
  ASSERT_EQ(unlockpt(master.get()), 0);
  const char* slave_name = ptsname(master.get());
  ASSERT_NE(slave_name, nullptr);
  FileDescriptor slave(open(slave_name, O_RDWR | O_NOCTTY));
  ASSERT_GE(slave.get(), 0);

  termios original{};
  ASSERT_EQ(tcgetattr(slave.get(), &original), 0);

  Input input(slave.get(), Mode::RAW);
  termios raw{};
  ASSERT_EQ(tcgetattr(slave.get(), &raw), 0);
  EXPECT_EQ(raw.c_lflag & (ECHO | ICANON | IEXTEN | ISIG), 0u);
  EXPECT_EQ(raw.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON), 0u);
  EXPECT_EQ(raw.c_oflag, original.c_oflag);

  input.close();
  termios restored{};
  ASSERT_EQ(tcgetattr(slave.get(), &restored), 0);
  EXPECT_EQ(restored.c_iflag, original.c_iflag);
  EXPECT_EQ(restored.c_oflag, original.c_oflag);
  EXPECT_EQ(restored.c_cflag, original.c_cflag);
  EXPECT_EQ(restored.c_lflag, original.c_lflag);
  EXPECT_EQ(std::memcmp(restored.c_cc, original.c_cc, NCCS), 0);
}

TEST(InpootsInput, NativeApiReadsPortableEventsFromStdin)
{
  FileDescriptor master(posix_openpt(O_RDWR | O_NOCTTY));
  ASSERT_GE(master.get(), 0);
  ASSERT_EQ(grantpt(master.get()), 0);
  ASSERT_EQ(unlockpt(master.get()), 0);
  const char* slave_name = ptsname(master.get());
  ASSERT_NE(slave_name, nullptr);
  FileDescriptor slave(open(slave_name, O_RDWR | O_NOCTTY));
  ASSERT_GE(slave.get(), 0);
  ScopedStdin stdin_scope(slave.get());
  ASSERT_EQ(write(master.get(), "x", 1), 1);

  Roo::Runtime runtime(Roo::Inpoots::make_native_namespaces(), nullptr);
  auto event = runtime.eval(R"(
    (inpoots.input/with-stdin! {:mode :raw}
      (fn [input]
        (inpoots.input/read! input)))
  )");

  EXPECT_EQ(event->to_string(), "{:type :text :text \"x\"}");
}

TEST(InpootsInput, NativeApiRestoresRawModeWhenCallbackThrows)
{
  FileDescriptor master(posix_openpt(O_RDWR | O_NOCTTY));
  ASSERT_GE(master.get(), 0);
  ASSERT_EQ(grantpt(master.get()), 0);
  ASSERT_EQ(unlockpt(master.get()), 0);
  const char* slave_name = ptsname(master.get());
  ASSERT_NE(slave_name, nullptr);
  FileDescriptor slave(open(slave_name, O_RDWR | O_NOCTTY));
  ASSERT_GE(slave.get(), 0);
  ScopedStdin stdin_scope(slave.get());
  termios original{};
  ASSERT_EQ(tcgetattr(STDIN_FILENO, &original), 0);

  Roo::Runtime runtime(Roo::Inpoots::make_native_namespaces(), nullptr);
  EXPECT_THROW(runtime.eval(R"(
    (inpoots.input/with-stdin! {:mode :raw}
      (fn [input]
        unknown-input-operation))
  )"),
               Roo::IdentifierException);

  termios restored{};
  ASSERT_EQ(tcgetattr(STDIN_FILENO, &restored), 0);
  EXPECT_EQ(restored.c_iflag, original.c_iflag);
  EXPECT_EQ(restored.c_oflag, original.c_oflag);
  EXPECT_EQ(restored.c_cflag, original.c_cflag);
  EXPECT_EQ(restored.c_lflag, original.c_lflag);
  EXPECT_EQ(std::memcmp(restored.c_cc, original.c_cc, NCCS), 0);
}

#endif
