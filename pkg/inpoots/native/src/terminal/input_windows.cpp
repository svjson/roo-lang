#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <windows.h>

#include <roo/exception.h>

#include <terminal/input.h>

namespace Roo::Inpoots::Terminal
{
  namespace
  {
    constexpr DWORD ESCAPE_TIMEOUT_MS = 30;

    std::string windows_error(const std::string& operation)
    {
      return operation + ": Windows error " + std::to_string(GetLastError());
    }

    std::vector<std::string> key_modifiers(DWORD state)
    {
      std::vector<std::string> modifiers;
      if ((state & SHIFT_PRESSED) != 0) modifiers.push_back("shift");
      if ((state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0) modifiers.push_back("alt");
      if ((state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0)
        modifiers.push_back("control");
      return modifiers;
    }

    std::optional<std::string> named_key(WORD key)
    {
      switch (key)
      {
      case VK_BACK:
        return "backspace";
      case VK_TAB:
        return "tab";
      case VK_RETURN:
        return "enter";
      case VK_ESCAPE:
        return "escape";
      case VK_PRIOR:
        return "page-up";
      case VK_NEXT:
        return "page-down";
      case VK_END:
        return "end";
      case VK_HOME:
        return "home";
      case VK_LEFT:
        return "left";
      case VK_UP:
        return "up";
      case VK_RIGHT:
        return "right";
      case VK_DOWN:
        return "down";
      case VK_INSERT:
        return "insert";
      case VK_DELETE:
        return "delete";
      default:
        return std::nullopt;
      }
    }

    std::string utf8_text(WCHAR first, WCHAR second = 0)
    {
      WCHAR text[2]{first, second};
      const int length = second == 0 ? 1 : 2;
      const int size = WideCharToMultiByte(CP_UTF8,
                                           WC_ERR_INVALID_CHARS,
                                           text,
                                           length,
                                           nullptr,
                                           0,
                                           nullptr,
                                           nullptr);
      if (size <= 0)
      {
        throw RooException(windows_error("Could not decode terminal input"));
      }
      std::string result(static_cast<std::size_t>(size), '\0');
      if (WideCharToMultiByte(CP_UTF8,
                              WC_ERR_INVALID_CHARS,
                              text,
                              length,
                              result.data(),
                              size,
                              nullptr,
                              nullptr) != size)
      {
        throw RooException(windows_error("Could not decode terminal input"));
      }
      return result;
    }

    std::string utf8_text(std::wstring_view text)
    {
      const int size = WideCharToMultiByte(CP_UTF8,
                                           WC_ERR_INVALID_CHARS,
                                           text.data(),
                                           static_cast<int>(text.size()),
                                           nullptr,
                                           0,
                                           nullptr,
                                           nullptr);
      if (size <= 0)
      {
        throw RooException(windows_error("Could not decode terminal input"));
      }
      std::string result(static_cast<std::size_t>(size), '\0');
      if (WideCharToMultiByte(CP_UTF8,
                              WC_ERR_INVALID_CHARS,
                              text.data(),
                              static_cast<int>(text.size()),
                              result.data(),
                              size,
                              nullptr,
                              nullptr) != size)
      {
        throw RooException(windows_error("Could not decode terminal input"));
      }
      return result;
    }
  } // namespace

  struct Input::State
  {
    HANDLE handle;
    Mode mode;
    bool console = false;
    bool changed = false;
    bool cooked_carriage_return = false;
    DWORD original_mode = 0;
    WCHAR high_surrogate = 0;
    std::vector<std::string> high_surrogate_modifiers;
    std::optional<Event> repeated_event;
    WORD repeats_remaining = 0;

    State(HANDLE handle, Mode mode)
      : handle(handle)
      , mode(mode)
    {
      if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
      {
        if (mode == Mode::COOKED) return;
        throw RooException("Standard input is not available.");
      }

      console = GetConsoleMode(handle, &original_mode) != 0;
      if (mode == Mode::COOKED) return;
      if (!console)
      {
        throw RooException("Raw and cbreak input require a terminal.");
      }

      DWORD configured = original_mode;
      configured &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
      configured |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
      configured &= ~ENABLE_QUICK_EDIT_MODE;
      if (mode == Mode::RAW) configured &= ~ENABLE_PROCESSED_INPUT;
      if (!SetConsoleMode(handle, configured))
      {
        throw RooException(windows_error("Could not set terminal input mode"));
      }
      changed = true;
    }

    Event emit(Event event, WORD repeat_count)
    {
      if (repeat_count > 1)
      {
        repeated_event = event;
        repeats_remaining = repeat_count - 1;
      }
      return event;
    }

    void restore()
    {
      if (!changed) return;
      if (!SetConsoleMode(handle, original_mode))
      {
        throw RooException(windows_error("Could not restore terminal input mode"));
      }
      changed = false;
    }
  };

  Input::Input(Mode mode)
    : Input(reinterpret_cast<std::intptr_t>(GetStdHandle(STD_INPUT_HANDLE)), mode)
  {
  }

  Input::Input(std::intptr_t native_handle, Mode mode)
    : state(std::make_unique<State>(reinterpret_cast<HANDLE>(native_handle), mode))
  {
  }

  Input::~Input()
  {
    if (!state) return;
    try
    {
      state->restore();
    }
    catch (...)
    {
    }
  }

  Event Input::read_event()
  {
    if (!active)
    {
      throw RooException("Terminal input is no longer active.");
    }

    if (state->repeated_event)
    {
      Event event = *state->repeated_event;
      if (--state->repeats_remaining == 0) state->repeated_event.reset();
      return event;
    }

    while (true)
    {
      if (auto event = decoder.next()) return std::move(*event);
      if (ended)
      {
        if (auto event = decoder.flush_escape()) return std::move(*event);
        if (state->high_surrogate != 0)
        {
          throw RooException("Unexpected end of UTF-16 terminal input.");
        }
        if (!decoder.empty())
        {
          throw RooException("Unexpected end of terminal input sequence.");
        }
        return Event::eof_event();
      }

      if (state->console && state->mode == Mode::COOKED)
      {
        WCHAR characters[64];
        DWORD count = 0;
        if (!ReadConsoleW(state->handle, characters, 64, &count, nullptr))
        {
          throw RooException(windows_error("Could not read terminal input"));
        }
        if (count == 0)
        {
          ended = true;
          continue;
        }

        std::wstring text;
        if (state->high_surrogate != 0)
        {
          text.push_back(state->high_surrogate);
          state->high_surrogate = 0;
        }
        text.append(characters, characters + count);
        if (!text.empty() && text.back() >= 0xd800 && text.back() <= 0xdbff)
        {
          state->high_surrogate = text.back();
          text.pop_back();
        }
        std::wstring normalized;
        normalized.reserve(text.size());
        for (WCHAR character : text)
        {
          if (character == L'\n' && state->cooked_carriage_return)
          {
            state->cooked_carriage_return = false;
            continue;
          }
          state->cooked_carriage_return = character == L'\r';
          normalized.push_back(character);
        }
        if (!normalized.empty()) decoder.append(utf8_text(normalized));
        continue;
      }

      if (!state->console)
      {
        const DWORD timeout = decoder.awaiting_escape() ? ESCAPE_TIMEOUT_MS : INFINITE;
        const DWORD ready = WaitForSingleObject(state->handle, timeout);
        if (ready == WAIT_TIMEOUT)
        {
          if (auto event = decoder.flush_escape()) return std::move(*event);
          continue;
        }
        if (ready != WAIT_OBJECT_0)
        {
          throw RooException(windows_error("Could not wait for terminal input"));
        }

        char bytes[64];
        DWORD count = 0;
        if (!ReadFile(state->handle, bytes, sizeof(bytes), &count, nullptr))
        {
          if (GetLastError() == ERROR_BROKEN_PIPE)
          {
            ended = true;
            continue;
          }
          throw RooException(windows_error("Could not read terminal input"));
        }
        if (count == 0)
        {
          ended = true;
          continue;
        }
        const std::string_view read_bytes(bytes, count);
        if (state->mode != Mode::COOKED)
        {
          decoder.append(read_bytes);
          continue;
        }

        std::string normalized;
        normalized.reserve(read_bytes.size());
        for (char byte : read_bytes)
        {
          if (byte == '\n' && state->cooked_carriage_return)
          {
            state->cooked_carriage_return = false;
            continue;
          }
          state->cooked_carriage_return = byte == '\r';
          normalized.push_back(byte);
        }
        decoder.append(normalized);
        continue;
      }

      INPUT_RECORD record{};
      DWORD count = 0;
      if (!ReadConsoleInputW(state->handle, &record, 1, &count))
      {
        throw RooException(windows_error("Could not read terminal input"));
      }
      if (count == 0) continue;

      if (record.EventType == WINDOW_BUFFER_SIZE_EVENT)
      {
        const auto size = record.Event.WindowBufferSizeEvent.dwSize;
        return Event::resize_event(size.X, size.Y);
      }
      if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown) continue;

      const KEY_EVENT_RECORD& key = record.Event.KeyEvent;
      const WCHAR character = key.uChar.UnicodeChar;
      if (character == 0x03) return Event::interrupt_event();
      if (character == 0x04 || character == 0x1a) return Event::eof_event();

      if (auto name = named_key(key.wVirtualKeyCode))
      {
        return state->emit(Event::key_event(*name, key_modifiers(key.dwControlKeyState)),
                           key.wRepeatCount);
      }
      if (character == 0) continue;

      if (character >= 0xd800 && character <= 0xdbff)
      {
        state->high_surrogate = character;
        state->high_surrogate_modifiers = key_modifiers(key.dwControlKeyState);
        continue;
      }
      if (character >= 0xdc00 && character <= 0xdfff)
      {
        if (state->high_surrogate == 0)
        {
          throw RooException("Invalid UTF-16 terminal input.");
        }
        const WCHAR high = state->high_surrogate;
        state->high_surrogate = 0;
        auto modifiers = std::move(state->high_surrogate_modifiers);
        state->high_surrogate_modifiers.clear();
        return state->emit(
          Event::text_event(utf8_text(high, character), std::move(modifiers)),
          key.wRepeatCount);
      }
      if (state->high_surrogate != 0)
      {
        state->high_surrogate = 0;
        state->high_surrogate_modifiers.clear();
        throw RooException("Invalid UTF-16 terminal input.");
      }
      if (character < 0x20)
      {
        const char byte = static_cast<char>(character);
        const WORD repeat_count = key.wRepeatCount == 0 ? 1 : key.wRepeatCount;
        for (WORD i = 0; i < repeat_count; ++i)
        {
          decoder.append(std::string_view(&byte, 1));
        }
        continue;
      }
      return state->emit(
        Event::text_event(utf8_text(character), key_modifiers(key.dwControlKeyState)),
        key.wRepeatCount);
    }
  }

  void Input::close()
  {
    if (!active) return;
    state->restore();
    active = false;
  }

  bool Input::is_active() const
  {
    return active;
  }
} // namespace Roo::Inpoots::Terminal
