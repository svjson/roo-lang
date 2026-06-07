#include "roo/lexer.h"

#include <regex>
#include <sstream>
#include <string>

#include <roo/exception.h>

namespace Roo
{

  TokenSymbol::TokenSymbol(Token token, std::string value)
    : token(token)
    , value(value)
  {
  }

  TokenSymbol::TokenSymbol(Token token, std::string value, SourceSpan span)
    : token(token)
    , value(value)
    , span(span)
  {
  }

  bool TokenSymbol::operator==(const TokenSymbol& other) const
  {
    return other.token == token && other.value == value;
  }

  std::ostream& operator<<(std::ostream& stream, const TokenSymbol& symbol)
  {
    return stream << "TokenSymbol(" << symbol.token << ", \"" << symbol.value << "\")";
  }

  std::vector<std::string> token_names = {"NONE",
                                          "LPAREN",
                                          "RPAREN",
                                          "LBRACKET",
                                          "RBRACKET",
                                          "LCURLY",
                                          "RCURLY",
                                          "SQUOT",
                                          "SYMBOL",
                                          "NUMBER",
                                          "STRING",
                                          "CHAR",
                                          "KEYWORD",
                                          "HASH",
                                          "USCORE"};

  std::ostream& operator<<(std::ostream& stream, const Token& token)
  {
    return stream << token_names.at(static_cast<int>(token));
  }

  std::vector<TokenSymbol> Lexer::read_symbols(const std::string& input,
                                               bool source_diagnostics) const
  {
    std::vector<TokenSymbol> tokens;
    std::vector<SourcePosition> positions;
    if (source_diagnostics)
    {
      positions.resize(input.size() + 1);
      uint32_t line_number = 1;
      uint32_t column_number = 1;
      for (size_t i = 0; i < input.size(); ++i)
      {
        positions[i] = {line_number, column_number};
        if (input[i] == '\n')
        {
          line_number++;
          column_number = 1;
        }
        else
        {
          column_number++;
        }
      }
      positions[input.size()] = {line_number, column_number};
    }

    auto span_for = [&](size_t start, size_t end) -> SourceSpan
    {
      if (!source_diagnostics || input.empty())
      {
        return {};
      }
      if (start >= positions.size())
      {
        start = positions.size() - 1;
      }
      if (end >= positions.size())
      {
        end = positions.size() - 1;
      }
      return {positions[start], positions[end]};
    };

    bool comment_context = false;

    std::string line = "";

    unsigned int offset = 0;
    std::string val = "";
    std::string cs = "";
    Token ct = Token::NONE;
    size_t token_start = 0;
    auto emit_value = [&](Token token, const std::string& value, size_t end)
    {
      if (std::regex_match(value, regex_hexnum))
      {
        tokens.push_back(
          TokenSymbol(Token::NUMBER, hex_to_int_str(value), span_for(token_start, end)));
      }
      else if (std::regex_match(value, regex_num))
      {
        tokens.push_back(TokenSymbol(Token::NUMBER, value, span_for(token_start, end)));
      }
      else
      {
        tokens.push_back(TokenSymbol(token, value, span_for(token_start, end)));
      }
    };

    while (offset < input.size())
    {
      const size_t char_offset = offset;
      char c = input.at(offset++);
      cs = c;
      line += cs;

      if (ct != Token::STRING && ct != Token::SQUOT && c == ';')
      {
        comment_context = true;
        continue;
      }

      if (c == '\n')
      {
        line = "";
      }

      if (comment_context && c == '\n')
      {
        comment_context = false;
        continue;
      }

      if (comment_context)
      {
        continue;
      }

      if (ct != Token::STRING && ct != Token::SQUOT)
      {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f')
        {
          if (val.size() == 0)
          {
            continue;
          }
          else
          {
            if (std::regex_match(val, regex_hexnum))
            {
              emit_value(Token::NUMBER, val, char_offset - 1);
            }
            else if (std::regex_match(val, regex_num))
            {
              emit_value(Token::NUMBER, val, char_offset - 1);
            }
            else
            {
              emit_value(ct, val, char_offset - 1);
            }
            val = "";
            ct = Token::NONE;
            continue;
          }
        }
      }
      else if (ct == Token::STRING && c == '"')
      {
        tokens.push_back(TokenSymbol(ct, val, span_for(token_start, char_offset)));
        val = "";
        ct = Token::NONE;
        continue;
      }
      else if (ct == Token::SQUOT && c == '\'')
      {
        if (val.empty() || val.size() > 1)
        {
          throw ParseException("Invalid char token: '" + val +
                               "'. Chars must have a size of exactly 1 character.");
        }
        ct = Token::NONE;
        tokens.push_back(TokenSymbol(Token::CHAR, val, span_for(token_start, char_offset)));
        val = "";
        continue;
      }
      else if (ct == Token::SQUOT && c == '\\')
      {
        if (offset == input.size())
        {
          throw ParseException("Unexpected token: '\\': " + line + " <---");
        }

        char escaped = input.at(offset++);
        switch (escaped)
        {
        case 'n':
          val += '\n';
          break;
        case 'r':
          val += '\r';
          break;
        case 't':
          val += '\t';
          break;
        case '\'':
          val += '\'';
          break;
        case '\\':
          val += '\\';
          break;
        default:
          val += escaped;
          break;
        }
        continue;
      }
      // Check if the currently determined Token is still valid with the new character
      if ((ct == Token::SYMBOL && std::regex_match(val + cs, regex_alphanum)) ||
          (ct == Token::NUMBER && std::regex_match(val + cs, regex_num)) ||
          (ct == Token::NUMBER && std::regex_match(val + cs, regex_hexnum)) ||
          (ct == Token::KEYWORD && std::regex_match(val + cs, regex_keyword_alphanum)) ||
          (ct == Token::STRING && c != '"' && c != '\\'))
      {
        val += c;
        continue;
      }
      else if (ct == Token::STRING && c == '\\')
      {
        if (offset == input.size())
        {
          throw ParseException("Unexpected token: '\\': " + line + " <---");
        }

        char escaped = input.at(offset++);
        switch (escaped)
        {
        case 'n':
          val += '\n';
          break;
        case 'r':
          val += '\r';
          break;
        case 't':
          val += '\t';
          break;
        case '"':
          val += '"';
          break;
        case '\\':
          val += '\\';
          break;
        default:
          val += escaped;
          break;
        }
        continue;
      }
      else if (ct == Token::SYMBOL && std::regex_match(val + cs, regex_num))
      {
        val += c;
        ct = Token::NUMBER;
        continue;
      }

      if (ct == Token::SQUOT)
      {
        val += c;
        if (val.size() == 2)
        {
          tokens.push_back(
            TokenSymbol(Token::SQUOT, "'", span_for(token_start, token_start)));
          offset -= 2;
          val = "";
          ct = Token::NONE;
        }
        continue;
      }

      if (ct == Token::NONE)
      {
        if (c == '"')
        {
          ct = Token::STRING;
          token_start = char_offset;
          continue;
        }
        else if (c == '\'')
        {
          ct = Token::SQUOT;
          token_start = char_offset;
          continue;
        }
        if (c == ':')
        {
          ct = Token::KEYWORD;
          token_start = char_offset;
          continue;
        }
        else if (std::regex_match(cs, regex_num) || std::regex_match(cs, regex_hexnum))
        {
          val = c;
          ct = Token::NUMBER;
          token_start = char_offset;
          continue;
        }
        else if (c == '_')
        {
          tokens.push_back(
            TokenSymbol(Token::USCORE, "_", span_for(char_offset, char_offset)));
          continue;
        }
        else if (std::regex_match(cs, regex_alpha))
        {
          val = c;
          ct = Token::SYMBOL;
          token_start = char_offset;
          continue;
        }
        else if (c == '#')
        {
          tokens.push_back(
            TokenSymbol(Token::HASH, "#", span_for(char_offset, char_offset)));
          continue;
        }
      }

      Token t = Token::NONE;
      switch (c)
      {
      case '(':
        t = Token::LPAREN;
        break;
      case ')':
        t = Token::RPAREN;
        break;
      case '[':
        t = Token::LBRACKET;
        break;
      case ']':
        t = Token::RBRACKET;
        break;
      case '{':
        t = Token::LCURLY;
        break;
      case '}':
        t = Token::RCURLY;
        break;
      };

      if (t != Token::NONE)
      {
        if (ct != Token::NONE)
        {
          if (std::regex_match(val, regex_hexnum))
          {
            emit_value(Token::NUMBER, val, char_offset - 1);
          }
          else if (std::regex_match(val, regex_num))
          {
            emit_value(Token::NUMBER, val, char_offset - 1);
          }
          else
          {
            emit_value(ct, val, char_offset - 1);
          }
          val = "";
        }
        tokens.push_back(TokenSymbol(t, cs, span_for(char_offset, char_offset)));
        ct = Token::NONE;
        continue;
      }

      throw RooException("Unexpected token: '" + cs + "': " + line + " <-----");
    }

    if (val.size())
    {
      const size_t end = input.empty() ? 0 : input.size() - 1;
      if (ct == Token::NUMBER && std::regex_match(val, regex_hexnum))
      {
        tokens.push_back(
          TokenSymbol(Token::NUMBER, hex_to_int_str(val), span_for(token_start, end)));
      }
      else
      {
        tokens.push_back(TokenSymbol(ct, val, span_for(token_start, end)));
      }
    }

    return tokens;
  }

  std::string Lexer::hex_to_int_str(const std::string& val) const
  {
    std::stringstream ss(val);
    int value;
    ss >> std::hex >> value;
    return std::to_string(value);
  }
} // namespace Roo
