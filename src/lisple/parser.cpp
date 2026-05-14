
#include "lisple/parser.h"

#include <string>
#include <utility>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/lexer.h>
#include <lisple/type.h>

namespace Lisple
{
  void ParseContext::begin_list(Token start_type)
  {
    std::shared_ptr<AST::Object> list;
    switch (start_type)
    {
    case Token::LPAREN:
      list = std::make_shared<AST::List>();
      closing_tokens.push_back(Token::RPAREN);
      break;
    case Token::SQUOT:
      list = std::make_shared<AST::List>(true);
      closing_tokens.push_back(Token::RPAREN);
      break;
    case Token::LBRACKET:
      list = std::make_shared<AST::Vector>();
      closing_tokens.push_back(Token::RBRACKET);
      break;
    case Token::LCURLY:
      list = std::make_shared<AST::Map>();
      closing_tokens.push_back(Token::RCURLY);
      break;
    default:
      throw ParseException("Invalid list type");
    }

    stack.push_back(std::move(list));
  }

  void ParseContext::begin_hash_context()
  {
    stack.push_back(std::make_shared<AST::Discard>());
  }

  void ParseContext::close_context(Token end_token)
  {
    if (closing_tokens.empty() || end_token != closing_tokens.back())
    {
      throw ParseException("Unmatched parens");
    }

    // Do we really want to automatically add a NIL value for unmatched k/v pairs?
    if (end_token == Token::RCURLY && stack.back()->get_children().size() % 2 == 1)
    {
      append(AST::NIL);
      // We don't validate keys here, because nothing has actually been evaluated yet.
      // Something like {(inc! num) val (inc! num) val} would be identified as
      // duplicate keys, although evaluation would produce unique keys.
    }

    auto frame = stack.back();
    stack.pop_back();
    closing_tokens.pop_back();
    append(std::move(frame));
  }

  void ParseContext::append(std::shared_ptr<AST::Object> obj)
  {
    if (stack.empty())
    {
      forms.push_back(obj);
    }
    else
    {
      stack.back()->append(obj);
      if (stack.back()->get_type() == Form::DISCARD)
      {
        auto discard_form = stack.back();
        stack.pop_back();
        append(std::move(discard_form));
      }
    }
  }

  void eof_check(std::vector<TokenSymbol>& symbols, unsigned int offset)
  {
    if (offset == symbols.size())
    {
      throw ParseException("Unexpected end of input");
    }
  }

  sptr_sobject_v Parser::parse_sexps(std::vector<TokenSymbol> symbols) const
  {
    ParseContext ctx;

    unsigned int offset = 0;

    while (offset < symbols.size())
    {
      TokenSymbol& sym = symbols.at(offset++);

      switch (sym.token)
      {
      case Token::HASH:
        ctx.begin_hash_context();
        break;
      case Token::LPAREN:
      case Token::LBRACKET:
      case Token::LCURLY:
        ctx.begin_list(sym.token);
        break;
      case Token::RPAREN:
      case Token::RBRACKET:
      case Token::RCURLY:
        ctx.close_context(sym.token);
        break;
      case Token::STRING:
        ctx.append(std::make_unique<AST::String>(sym.value));
        break;
      case Token::CHAR:
        ctx.append(std::make_unique<AST::Char>(sym.value.at(0)));
        break;
      case Token::KEYWORD:
        ctx.append(AST::Keyword::make(sym.value));
        break;
      case Token::SYMBOL:
        ctx.append(std::make_unique<AST::Symbol>(sym.value));
        break;
      case Token::NUMBER:
        ctx.append(AST::Number::make(sym.value));
        break;
      case Token::SQUOT:
        eof_check(symbols, offset);
        sym = symbols.at(offset++);
        if (sym.token == Token::LPAREN)
        {
          ctx.begin_list(Token::SQUOT);
        }
        else if (sym.token == Token::SYMBOL)
        {
          ctx.append(std::make_unique<AST::QuotedSymbol>(sym.value));
        }
        else
        {
          throw ParseException("Unexpected token: " + sym.value);
        }
        break;
      default:
        break;
      }
    }

    if (!ctx.stack.empty())
    {
      throw ParseException("Unexpected end of input");
    }

    return std::move(ctx.forms);
  }
} // namespace Lisple
