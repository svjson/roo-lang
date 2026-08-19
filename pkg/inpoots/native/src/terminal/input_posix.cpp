#include <cerrno>
#include <cstdint>
#include <cstring>
#include <memory>
#include <poll.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <utility>

#include <roo/exception.h>

#include <terminal/input.h>

namespace Roo::Inpoots::Terminal
{
  namespace
  {
    constexpr int ESCAPE_TIMEOUT_MS = 30;

    std::string system_error(const std::string& operation)
    {
      return operation + ": " + std::strerror(errno);
    }

    termios terminal_mode(termios configured, Mode mode)
    {
      configured.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));
      if (mode == Mode::RAW)
      {
        configured.c_iflag &=
          static_cast<tcflag_t>(~(BRKINT | ICRNL | INPCK | ISTRIP | IXON));
        configured.c_cflag |= CS8;
        configured.c_lflag &= static_cast<tcflag_t>(~(IEXTEN | ISIG));
      }
      configured.c_cc[VMIN] = 1;
      configured.c_cc[VTIME] = 0;
      return configured;
    }
  } // namespace

  struct Input::State
  {
    int descriptor;
    Mode mode;
    bool changed = false;
    bool cooked_carriage_return = false;
    termios original{};

    State(int descriptor, Mode mode)
      : descriptor(descriptor)
      , mode(mode)
    {
      if (mode == Mode::COOKED) return;

      if (isatty(descriptor) == 0)
      {
        throw RooException("Raw and cbreak input require a terminal.");
      }

      if (tcgetattr(descriptor, &original) != 0)
      {
        throw RooException(system_error("Could not read terminal input mode"));
      }
      const termios configured = terminal_mode(original, mode);
      if (tcsetattr(descriptor, TCSANOW, &configured) != 0)
      {
        throw RooException(system_error("Could not set terminal input mode"));
      }
      changed = true;
    }

    void restore()
    {
      if (!changed) return;
      if (tcsetattr(descriptor, TCSANOW, &original) != 0)
      {
        throw RooException(system_error("Could not restore terminal input mode"));
      }
      changed = false;
    }
  };

  Input::Input(Mode mode)
    : Input(static_cast<std::intptr_t>(STDIN_FILENO), mode)
  {
  }

  Input::Input(std::intptr_t native_handle, Mode mode)
    : state(std::make_unique<State>(static_cast<int>(native_handle), mode))
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

    while (true)
    {
      if (auto event = decoder.next()) return std::move(*event);
      if (ended)
      {
        if (auto event = decoder.flush_escape()) return std::move(*event);
        if (!decoder.empty())
        {
          throw RooException("Unexpected end of terminal input sequence.");
        }
        return Event::eof_event();
      }

      pollfd descriptor{state->descriptor, POLLIN, 0};
      const int timeout = decoder.awaiting_escape() ? ESCAPE_TIMEOUT_MS : -1;
      const int ready = poll(&descriptor, 1, timeout);
      if (ready == 0)
      {
        if (auto event = decoder.flush_escape()) return std::move(*event);
        continue;
      }
      if (ready < 0)
      {
        if (errno == EINTR) continue;
        throw RooException(system_error("Could not wait for terminal input"));
      }

      char bytes[64];
      const ssize_t count = read(state->descriptor, bytes, sizeof(bytes));
      if (count == 0)
      {
        ended = true;
        continue;
      }
      if (count < 0)
      {
        if (errno == EINTR || errno == EAGAIN) continue;
        throw RooException(system_error("Could not read terminal input"));
      }
      const std::string_view read_bytes(bytes, static_cast<std::size_t>(count));
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
