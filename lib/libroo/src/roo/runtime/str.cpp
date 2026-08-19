
#include <roo/runtime/str.h>

namespace Roo
{
  namespace
  {
    size_t utf8_scalar_size(const std::string& text, size_t offset)
    {
      const auto first = static_cast<unsigned char>(text[offset]);
      size_t size = 1;
      if (first >= 0xc2 && first <= 0xdf)
      {
        size = 2;
      }
      else if (first >= 0xe0 && first <= 0xef)
      {
        size = 3;
      }
      else if (first >= 0xf0 && first <= 0xf4)
      {
        size = 4;
      }
      else if (first >= 0x80)
      {
        return 1;
      }

      if (offset + size > text.size()) return 1;
      for (size_t index = 1; index < size; ++index)
      {
        if ((static_cast<unsigned char>(text[offset + index]) & 0xc0) != 0x80) return 1;
      }

      const auto second = size > 1 ? static_cast<unsigned char>(text[offset + 1]) : 0;
      if ((first == 0xe0 && second < 0xa0) || (first == 0xed && second >= 0xa0) ||
          (first == 0xf0 && second < 0x90) || (first == 0xf4 && second >= 0x90))
      {
        return 1;
      }
      return size;
    }
  } // namespace

  std::string escaped_string_literal(const std::string& value)
  {
    std::string result = "\"";
    for (char ch : value)
    {
      switch (ch)
      {
      case '\\':
        result += "\\\\";
        break;
      case '"':
        result += "\\\"";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        result += ch;
        break;
      }
    }
    result += "\"";
    return result;
  }

  std::vector<std::string> utf8_scalars(const std::string& value)
  {
    std::vector<std::string> result;
    result.reserve(value.size());
    for (size_t offset = 0; offset < value.size();)
    {
      const size_t size = utf8_scalar_size(value, offset);
      result.push_back(value.substr(offset, size));
      offset += size;
    }
    return result;
  }
} // namespace Roo
