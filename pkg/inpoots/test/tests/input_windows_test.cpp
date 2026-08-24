#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

#include <roo/exception.h>

#include <gtest/gtest.h>
#include <terminal/input.h>

namespace
{
  class WindowsHandle
  {
   private:
    HANDLE value;

   public:
    explicit WindowsHandle(HANDLE value = INVALID_HANDLE_VALUE)
      : value(value)
    {
    }

    ~WindowsHandle()
    {
      if (value != nullptr && value != INVALID_HANDLE_VALUE) CloseHandle(value);
    }

    WindowsHandle(const WindowsHandle&) = delete;
    WindowsHandle& operator=(const WindowsHandle&) = delete;

    HANDLE get() const { return value; }

    void close()
    {
      if (value == nullptr || value == INVALID_HANDLE_VALUE) return;
      CloseHandle(value);
      value = INVALID_HANDLE_VALUE;
    }
  };

  INPUT_RECORD key_record(WCHAR character,
                          WORD virtual_key = 0,
                          DWORD control_state = 0,
                          WORD repeat_count = 1,
                          bool pressed = true)
  {
    INPUT_RECORD record{};
    record.EventType = KEY_EVENT;
    record.Event.KeyEvent.bKeyDown = pressed;
    record.Event.KeyEvent.wRepeatCount = repeat_count;
    record.Event.KeyEvent.wVirtualKeyCode = virtual_key;
    record.Event.KeyEvent.uChar.UnicodeChar = character;
    record.Event.KeyEvent.dwControlKeyState = control_state;
    return record;
  }

  INPUT_RECORD resize_record(SHORT columns, SHORT rows)
  {
    INPUT_RECORD record{};
    record.EventType = WINDOW_BUFFER_SIZE_EVENT;
    record.Event.WindowBufferSizeEvent.dwSize = {columns, rows};
    return record;
  }

  class InpootsWindowsConsoleInput : public ::testing::Test
  {
   protected:
    HANDLE input = INVALID_HANDLE_VALUE;
    DWORD original_mode = 0;
    bool allocated_console = false;

    void SetUp() override
    {
      input = CreateFileW(L"CONIN$",
                          GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          nullptr,
                          OPEN_EXISTING,
                          0,
                          nullptr);
      if (input == INVALID_HANDLE_VALUE)
      {
        FreeConsole();
        ASSERT_TRUE(AllocConsole());
        allocated_console = true;
        input = CreateFileW(L"CONIN$",
                            GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            nullptr,
                            OPEN_EXISTING,
                            0,
                            nullptr);
      }

      ASSERT_NE(input, INVALID_HANDLE_VALUE);
      ASSERT_TRUE(GetConsoleMode(input, &original_mode));
      ASSERT_TRUE(FlushConsoleInputBuffer(input));
    }

    void TearDown() override
    {
      if (input != INVALID_HANDLE_VALUE)
      {
        EXPECT_TRUE(SetConsoleMode(input, original_mode));
        EXPECT_TRUE(FlushConsoleInputBuffer(input));
        EXPECT_TRUE(CloseHandle(input));
      }
      if (allocated_console) EXPECT_TRUE(FreeConsole());
    }

    DWORD known_mode()
    {
      const DWORD mode = original_mode | ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT |
                         ENABLE_ECHO_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_QUICK_EDIT_MODE;
      EXPECT_TRUE(SetConsoleMode(input, mode));
      return mode;
    }

    void write_records(const std::vector<INPUT_RECORD>& records)
    {
      DWORD written = 0;
      ASSERT_TRUE(WriteConsoleInputW(input,
                                     records.data(),
                                     static_cast<DWORD>(records.size()),
                                     &written));
      ASSERT_EQ(written, records.size());
    }
  };
} // namespace

using Roo::Inpoots::Terminal::EventType;
using Roo::Inpoots::Terminal::Input;
using Roo::Inpoots::Terminal::Mode;

TEST(InpootsWindowsInput, ReadsCookedEventsAndEofFromRedirectedInput)
{
  HANDLE read_handle = INVALID_HANDLE_VALUE;
  HANDLE write_handle = INVALID_HANDLE_VALUE;
  ASSERT_TRUE(CreatePipe(&read_handle, &write_handle, nullptr, 0));
  WindowsHandle input_handle(read_handle);
  WindowsHandle output_handle(write_handle);

  const std::string bytes = "x\r\ny\x1b[D";
  DWORD written = 0;
  ASSERT_TRUE(WriteFile(output_handle.get(),
                        bytes.data(),
                        static_cast<DWORD>(bytes.size()),
                        &written,
                        nullptr));
  ASSERT_EQ(written, bytes.size());
  output_handle.close();

  Input input(reinterpret_cast<std::intptr_t>(input_handle.get()), Mode::COOKED);
  const auto text = input.read_event();
  const auto enter = input.read_event();
  const auto following_text = input.read_event();
  const auto left = input.read_event();
  const auto eof = input.read_event();

  EXPECT_EQ(text.type, EventType::KEYSTROKE);
  EXPECT_EQ(text.key, "x");
  EXPECT_EQ(text.text, "x");
  EXPECT_EQ(enter.type, EventType::KEYSTROKE);
  EXPECT_EQ(enter.key, "enter");
  EXPECT_EQ(following_text.type, EventType::KEYSTROKE);
  EXPECT_EQ(following_text.key, "y");
  EXPECT_EQ(following_text.text, "y");
  EXPECT_EQ(left.type, EventType::KEYSTROKE);
  EXPECT_EQ(left.key, "left");
  EXPECT_EQ(eof.type, EventType::EOF_EVENT);
}

TEST(InpootsWindowsInput, RawModeRejectsRedirectedInput)
{
  HANDLE read_handle = INVALID_HANDLE_VALUE;
  HANDLE write_handle = INVALID_HANDLE_VALUE;
  ASSERT_TRUE(CreatePipe(&read_handle, &write_handle, nullptr, 0));
  WindowsHandle input_handle(read_handle);
  WindowsHandle output_handle(write_handle);

  EXPECT_THROW(Input(reinterpret_cast<std::intptr_t>(input_handle.get()), Mode::RAW),
               Roo::RooException);
}

TEST_F(InpootsWindowsConsoleInput, RawModeIsRestoredWhenTheInputCloses)
{
  const DWORD baseline = known_mode();
  Input terminal_input(reinterpret_cast<std::intptr_t>(input), Mode::RAW);

  DWORD raw = 0;
  ASSERT_TRUE(GetConsoleMode(input, &raw));
  EXPECT_EQ(raw & (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT |
                   ENABLE_QUICK_EDIT_MODE),
            0u);
  EXPECT_NE(raw & ENABLE_EXTENDED_FLAGS, 0u);
  EXPECT_NE(raw & ENABLE_WINDOW_INPUT, 0u);

  terminal_input.close();
  DWORD restored = 0;
  ASSERT_TRUE(GetConsoleMode(input, &restored));
  EXPECT_EQ(restored, baseline);
}

TEST_F(InpootsWindowsConsoleInput, CbreakModePreservesProcessedInput)
{
  const DWORD baseline = known_mode();
  {
    Input terminal_input(reinterpret_cast<std::intptr_t>(input), Mode::CBREAK);
    DWORD cbreak = 0;
    ASSERT_TRUE(GetConsoleMode(input, &cbreak));
    EXPECT_EQ(cbreak & (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_QUICK_EDIT_MODE), 0u);
    EXPECT_NE(cbreak & ENABLE_PROCESSED_INPUT, 0u);
    EXPECT_NE(cbreak & ENABLE_WINDOW_INPUT, 0u);
  }

  DWORD restored = 0;
  ASSERT_TRUE(GetConsoleMode(input, &restored));
  EXPECT_EQ(restored, baseline);
}

TEST_F(InpootsWindowsConsoleInput, ReadsCookedConsoleTextAndEnter)
{
  known_mode();
  Input terminal_input(reinterpret_cast<std::intptr_t>(input), Mode::COOKED);
  write_records({key_record(L'x', L'X'), key_record(L'\r', VK_RETURN)});

  const auto text = terminal_input.read_event();
  const auto enter = terminal_input.read_event();

  EXPECT_EQ(text.type, EventType::KEYSTROKE);
  EXPECT_EQ(text.key, "x");
  EXPECT_EQ(text.text, "x");
  EXPECT_EQ(enter.type, EventType::KEYSTROKE);
  EXPECT_EQ(enter.key, "enter");
}

TEST_F(InpootsWindowsConsoleInput, ReadsKeysUnicodeRepeatsAndResizeRecords)
{
  known_mode();
  Input terminal_input(reinterpret_cast<std::intptr_t>(input), Mode::RAW);
  write_records(
    {key_record(0, VK_LEFT, SHIFT_PRESSED | LEFT_ALT_PRESSED | LEFT_CTRL_PRESSED, 2, false),
     key_record(0, VK_LEFT, SHIFT_PRESSED | LEFT_ALT_PRESSED | LEFT_CTRL_PRESSED, 2),
     key_record(L'\u00e5', 0, 0, 2),
     key_record(L'\xd83d'),
     key_record(L'\xde42'),
     resize_record(120, 40)});

  const auto left = terminal_input.read_event();
  const auto repeated_left = terminal_input.read_event();
  const auto letter = terminal_input.read_event();
  const auto repeated_letter = terminal_input.read_event();
  const auto emoji = terminal_input.read_event();
  const auto resize = terminal_input.read_event();

  EXPECT_EQ(left.type, EventType::KEYSTROKE);
  EXPECT_EQ(left.key, "left");
  EXPECT_EQ(left.modifiers, (std::vector<std::string>{"shift", "alt", "ctrl"}));
  EXPECT_EQ(repeated_left.type, EventType::KEYSTROKE);
  EXPECT_EQ(repeated_left.key, "left");
  EXPECT_EQ(repeated_left.modifiers, left.modifiers);
  EXPECT_EQ(letter.type, EventType::KEYSTROKE);
  EXPECT_TRUE(letter.key.empty());
  EXPECT_EQ(letter.text, "\xc3\xa5");
  EXPECT_EQ(repeated_letter.type, EventType::KEYSTROKE);
  EXPECT_TRUE(repeated_letter.key.empty());
  EXPECT_EQ(repeated_letter.text, "\xc3\xa5");
  EXPECT_EQ(emoji.type, EventType::KEYSTROKE);
  EXPECT_TRUE(emoji.key.empty());
  EXPECT_EQ(emoji.text, "\xf0\x9f\x99\x82");
  EXPECT_EQ(resize.type, EventType::RESIZE);
  EXPECT_EQ(resize.columns, 120);
  EXPECT_EQ(resize.rows, 40);
}

TEST_F(InpootsWindowsConsoleInput, ReadsAnnotatedControlCAndControlDKeystrokes)
{
  known_mode();
  Input terminal_input(reinterpret_cast<std::intptr_t>(input), Mode::RAW);
  write_records({key_record(0x03), key_record(0x04)});

  const auto ctrl_c = terminal_input.read_event();
  const auto ctrl_d = terminal_input.read_event();

  EXPECT_EQ(ctrl_c.type, EventType::KEYSTROKE);
  EXPECT_EQ(ctrl_c.key, "c");
  EXPECT_EQ(ctrl_c.modifiers, std::vector<std::string>({"ctrl"}));
  EXPECT_EQ(ctrl_c.signal, "interrupt");
  EXPECT_EQ(ctrl_d.type, EventType::KEYSTROKE);
  EXPECT_EQ(ctrl_d.key, "d");
  EXPECT_EQ(ctrl_d.modifiers, std::vector<std::string>({"ctrl"}));
  EXPECT_EQ(ctrl_d.control, "eof");
}

#endif
