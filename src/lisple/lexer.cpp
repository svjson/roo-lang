#include "lisple/lexer.h"

#include <regex>
#include <sstream>
#include <string>

#include <lisple/exception.h>

namespace Lisple
{

  TokenSymbol::TokenSymbol(Token token, std::string value)
    : token(token)
    , value(value)
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

  std::vector<TokenSymbol> Lexer::read_symbols(const std::string& input) const
  {
    std::vector<TokenSymbol> tokens;

    bool comment_context = false;

    std::string line = "";

    unsigned int offset = 0;
    std::string val = "";
    std::string cs = "";
    Token ct = Token::NONE;
    while (offset < input.size())
    {
      char c = input.at(offset++);
      cs = c;
      line += cs;

      if (ct != Token::STRING && c == ';')
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
              tokens.push_back(TokenSymbol(Token::NUMBER, hex_to_int_str(val)));
            }
            else if (std::regex_match(val, regex_num))
            {
              tokens.push_back(TokenSymbol(Token::NUMBER, val));
            }
            else
            {
              tokens.push_back(TokenSymbol(ct, val));
            }
            val = "";
            ct = Token::NONE;
            continue;
          }
        }
      }
      else if (ct == Token::STRING && c == '"')
      {
        tokens.push_back(TokenSymbol(ct, val));
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
        tokens.push_back(TokenSymbol(Token::CHAR, val));
        val = "";
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
          tokens.push_back(TokenSymbol(Token::SQUOT, "'"));
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
          continue;
        }
        else if (c == '\'')
        {
          ct = Token::SQUOT;
          continue;
        }
        if (c == ':')
        {
          ct = Token::KEYWORD;
          continue;
        }
        else if (std::regex_match(cs, regex_num) || std::regex_match(cs, regex_hexnum))
        {
          val = c;
          ct = Token::NUMBER;
          continue;
        }
        else if (c == '_')
        {
          tokens.push_back(TokenSymbol(Token::USCORE, "_"));
          continue;
        }
        else if (std::regex_match(cs, regex_alpha))
        {
          val = c;
          ct = Token::SYMBOL;
          continue;
        }
        else if (c == '#')
        {
          tokens.push_back(TokenSymbol(Token::HASH, "#"));
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
            tokens.push_back(TokenSymbol(Token::NUMBER, hex_to_int_str(val)));
          }
          else if (std::regex_match(val, regex_num))
          {
            tokens.push_back(TokenSymbol(Token::NUMBER, val));
          }
          else
          {
            tokens.push_back(TokenSymbol(ct, val));
          }
          val = "";
        }
        tokens.push_back(TokenSymbol(t, cs));
        ct = Token::NONE;
        continue;
      }

      throw LispleException("Unexpected token: '" + cs + "': " + line + " <-----");
    }

    if (val.size())
    {
      if (ct == Token::NUMBER && std::regex_match(val, regex_hexnum))
      {
        val = hex_to_int_str(val);
      }
      tokens.push_back(TokenSymbol(ct, val));
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
} // namespace Lisple
