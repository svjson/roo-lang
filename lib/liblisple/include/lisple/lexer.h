
#ifndef __LEXER_H_
#define __LEXER_H_

#include <cstdint>
#include <iosfwd>
#include <regex>
#include <string>
#include <vector>

namespace Lisple
{
  enum class Token : uint8_t
  {
    NONE,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LCURLY,
    RCURLY,
    SQUOT,
    SYMBOL,
    NUMBER,
    STRING,
    CHAR,
    KEYWORD,
    HASH,
    USCORE
  };

  class TokenSymbol
  {
  public:
    Token token;
    std::string value;

    TokenSymbol(Token token, std::string value);

    bool operator==(const TokenSymbol& other) const;
  };

  std::ostream& operator<<(std::ostream& stream, const TokenSymbol& symbol);
  std::ostream& operator<<(std::ostream& stream, const Token& token);

  class Lexer
  {
    std::regex regex_alpha{"[a-zA-Z\\/\\-_+?!|=\\>\\<\\*&]*"};
    std::regex regex_num{"-?[0-9]+\\.?[0-9]*?"};
    std::regex regex_hexnum{"0x[0-9a-fA-F]*"};
    std::regex regex_keyword_alphanum{"[a-zA-Z\\/\\-_+?!|:\\>\\<\\*0-9=]+"};
    std::regex regex_alphanum{"[a-zA-Z\\/\\-_+?!|\\>\\<\\*=&][a-zA-Z\\/\\-_+?!|:\\>\\<\\.\\*0-9=]*"};

  public:
    virtual ~Lexer() = default;

    virtual std::vector<TokenSymbol> read_symbols(const std::string& input) const;

  private:
    std::string hex_to_int_str(const std::string& val) const;
  };
} // namespace Lisple

#endif
