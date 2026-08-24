#include <algorithm>
#include <cctype>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <roo/exception.h>

#include <terminal/decoder.h>

namespace Roo::Inpoots::Terminal
{
  namespace
  {
    constexpr std::string_view PASTE_START = "\x1b[200~";
    constexpr std::string_view PASTE_END = "\x1b[201~";

    std::size_t utf8_length(unsigned char first)
    {
      if (first < 0x80) return 1;
      if (first >= 0xc2 && first <= 0xdf) return 2;
      if (first >= 0xe0 && first <= 0xef) return 3;
      if (first >= 0xf0 && first <= 0xf4) return 4;
      throw RooException("Invalid UTF-8 terminal input.");
    }

    bool valid_utf8(const std::string& bytes, std::size_t length)
    {
      if (bytes.size() < length) return false;
      for (std::size_t i = 1; i < length; ++i)
      {
        const auto byte = static_cast<unsigned char>(bytes[i]);
        if ((byte & 0xc0) != 0x80)
        {
          throw RooException("Invalid UTF-8 terminal input.");
        }
      }

      const auto first = static_cast<unsigned char>(bytes[0]);
      const auto second = length > 1 ? static_cast<unsigned char>(bytes[1]) : 0;
      if ((first == 0xe0 && second < 0xa0) || (first == 0xed && second >= 0xa0) ||
          (first == 0xf0 && second < 0x90) || (first == 0xf4 && second >= 0x90))
      {
        throw RooException("Invalid UTF-8 terminal input.");
      }
      return true;
    }

    void validate_utf8(const std::string& text)
    {
      for (std::size_t offset = 0; offset < text.size();)
      {
        const std::size_t length = utf8_length(static_cast<unsigned char>(text[offset]));
        const std::string bytes = text.substr(offset, length);
        if (!valid_utf8(bytes, length))
        {
          throw RooException("Unexpected end of UTF-8 terminal input.");
        }
        offset += length;
      }
    }

    std::vector<std::string> xterm_modifiers(int value)
    {
      std::vector<std::string> modifiers;
      const int bits = value - 1;
      if ((bits & 1) != 0) modifiers.push_back("shift");
      if ((bits & 2) != 0) modifiers.push_back("alt");
      if ((bits & 4) != 0) modifiers.push_back("ctrl");
      return modifiers;
    }

    std::optional<std::vector<int>> csi_parameters(std::string_view body)
    {
      std::vector<int> parameters;
      int value = 0;
      bool has_value = false;
      for (char ch : body)
      {
        if (std::isdigit(static_cast<unsigned char>(ch)))
        {
          value = value * 10 + (ch - '0');
          has_value = true;
        }
        else if (ch == ';')
        {
          parameters.push_back(has_value ? value : 0);
          value = 0;
          has_value = false;
        }
        else
        {
          return std::nullopt;
        }
      }
      if (has_value || !body.empty()) parameters.push_back(has_value ? value : 0);
      return parameters;
    }

    Event csi_event(const std::string& sequence)
    {
      const char final = sequence.back();
      const auto parsed_parameters =
        csi_parameters(std::string_view(sequence).substr(2, sequence.size() - 3));
      if (!parsed_parameters) return Event::keystroke_event("unknown", {}, sequence);
      const auto& parameters = *parsed_parameters;
      std::vector<std::string> modifiers;
      if (parameters.size() >= 2 && parameters[1] >= 2 && parameters[1] <= 8)
      {
        modifiers = xterm_modifiers(parameters[1]);
      }

      switch (final)
      {
      case 'A':
        return Event::keystroke_event("up", modifiers);
      case 'B':
        return Event::keystroke_event("down", modifiers);
      case 'C':
        return Event::keystroke_event("right", modifiers);
      case 'D':
        return Event::keystroke_event("left", modifiers);
      case 'H':
        return Event::keystroke_event("home", modifiers);
      case 'F':
        return Event::keystroke_event("end", modifiers);
      case 'Z':
        return Event::keystroke_event("tab", {"shift"});
      case '~':
        if (!parameters.empty())
        {
          switch (parameters[0])
          {
          case 1:
          case 7:
            return Event::keystroke_event("home", modifiers);
          case 2:
            return Event::keystroke_event("insert", modifiers);
          case 3:
            return Event::keystroke_event("delete", modifiers);
          case 4:
          case 8:
            return Event::keystroke_event("end", modifiers);
          case 5:
            return Event::keystroke_event("page-up", modifiers);
          case 6:
            return Event::keystroke_event("page-down", modifiers);
          default:
            break;
          }
        }
        break;
      default:
        break;
      }
      return Event::keystroke_event("unknown", {}, sequence);
    }

    Event control_event(unsigned char byte)
    {
      switch (byte)
      {
      case 0x00:
        return Event::keystroke_event("at", {"ctrl"});
      case 0x03:
      {
        Event event = Event::keystroke_event("c", {"ctrl"});
        event.signal = "interrupt";
        return event;
      }
      case 0x04:
      {
        Event event = Event::keystroke_event("d", {"ctrl"});
        event.control = "eof";
        return event;
      }
      case 0x08:
      case 0x7f:
        return Event::keystroke_event("backspace");
      case 0x09:
        return Event::keystroke_event("tab");
      case 0x0a:
      case 0x0d:
        return Event::keystroke_event("enter");
      case 0x1c:
        return Event::keystroke_event("backslash", {"ctrl"});
      case 0x1d:
        return Event::keystroke_event("right-bracket", {"ctrl"});
      case 0x1e:
        return Event::keystroke_event("caret", {"ctrl"});
      case 0x1f:
        return Event::keystroke_event("underscore", {"ctrl"});
      default:
        return Event::keystroke_event(std::string(1, static_cast<char>('a' + byte - 1)),
                                      {"ctrl"});
      }
    }

    std::size_t paste_suffix_length(const std::string& text)
    {
      const std::size_t limit = std::min(text.size(), PASTE_END.size() - 1);
      for (std::size_t length = limit; length > 0; --length)
      {
        if (std::string_view(text).substr(text.size() - length) ==
            PASTE_END.substr(0, length))
        {
          return length;
        }
      }
      return 0;
    }
  } // namespace

  void Decoder::append(std::string_view bytes)
  {
    pending.append(bytes);
  }

  std::optional<Event> Decoder::next()
  {
    while (!pending.empty())
    {
      if (paste)
      {
        const auto end = pending.find(PASTE_END);
        if (end != std::string::npos)
        {
          pasted_text.append(pending, 0, end);
          pending.erase(0, end + PASTE_END.size());
          paste = false;
          validate_utf8(pasted_text);
          auto event = Event::paste_event(std::move(pasted_text));
          pasted_text.clear();
          return event;
        }

        const std::size_t retained = paste_suffix_length(pending);
        pasted_text.append(pending, 0, pending.size() - retained);
        pending.erase(0, pending.size() - retained);
        return std::nullopt;
      }

      const auto first = static_cast<unsigned char>(pending[0]);
      if (first == 0x1b)
      {
        if (pending.size() == 1) return std::nullopt;

        if (pending.size() < PASTE_START.size() &&
            PASTE_START.substr(0, pending.size()) == pending)
        {
          return std::nullopt;
        }
        if (pending.starts_with(PASTE_START))
        {
          pending.erase(0, PASTE_START.size());
          paste = true;
          continue;
        }

        if (pending[1] == '[')
        {
          const auto final =
            std::find_if(pending.begin() + 2,
                         pending.end(),
                         [](unsigned char byte) { return byte >= 0x40 && byte <= 0x7e; });
          if (final == pending.end()) return std::nullopt;
          const std::size_t length = static_cast<std::size_t>(final - pending.begin()) + 1;
          std::string sequence = pending.substr(0, length);
          pending.erase(0, length);
          return csi_event(sequence);
        }

        if (pending[1] == 'O')
        {
          if (pending.size() < 3) return std::nullopt;
          const char key = pending[2];
          std::string sequence = pending.substr(0, 3);
          pending.erase(0, 3);
          switch (key)
          {
          case 'A':
            return Event::keystroke_event("up");
          case 'B':
            return Event::keystroke_event("down");
          case 'C':
            return Event::keystroke_event("right");
          case 'D':
            return Event::keystroke_event("left");
          case 'H':
            return Event::keystroke_event("home");
          case 'F':
            return Event::keystroke_event("end");
          default:
            return Event::keystroke_event("unknown", {}, sequence);
          }
        }

        std::string alternate = pending.substr(1);
        const std::size_t length = utf8_length(static_cast<unsigned char>(alternate[0]));
        if (!valid_utf8(alternate, length)) return std::nullopt;
        pending.erase(0, length + 1);
        return Event::text_keystroke(alternate.substr(0, length), {"alt"});
      }

      if (first < 0x20 || first == 0x7f)
      {
        pending.erase(0, 1);
        return control_event(first);
      }

      const std::size_t length = utf8_length(first);
      if (!valid_utf8(pending, length)) return std::nullopt;
      std::string text = pending.substr(0, length);
      pending.erase(0, length);
      return Event::text_keystroke(std::move(text));
    }
    return std::nullopt;
  }

  std::optional<Event> Decoder::flush_escape()
  {
    if (pending.empty()) return std::nullopt;
    if (pending == "\x1b")
    {
      pending.clear();
      return Event::keystroke_event("escape");
    }
    if (pending[0] == 0x1b)
    {
      std::string sequence = std::move(pending);
      pending.clear();
      return Event::keystroke_event("unknown", {}, std::move(sequence));
    }
    return std::nullopt;
  }

  bool Decoder::awaiting_escape() const
  {
    return !pending.empty() && pending[0] == 0x1b;
  }

  bool Decoder::empty() const
  {
    return pending.empty() && !paste;
  }
} // namespace Roo::Inpoots::Terminal
