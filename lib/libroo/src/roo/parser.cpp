
#include "roo/parser.h"

#include <string>
#include <utility>

#include <roo/exception.h>
#include <roo/form.h>
#include <roo/lexer.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    SourceRef source_ref(uint32_t file_id, const SourceSpan& span, bool enabled)
    {
      return enabled ? SourceRef{file_id, span} : SourceRef{};
    }

    sptr_ast_node with_source(sptr_ast_node node, const SourceRef& source)
    {
      node->set_source(source);
      return node;
    }
  } // namespace

  void ParseContext::begin_list(Token start_type, const SourceRef& source)
  {
    std::shared_ptr<AST::ASTNode> list;
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

    list->set_source(source);
    stack.push_back(std::move(list));
  }

  void ParseContext::begin_hash_context()
  {
    stack.push_back(std::make_shared<AST::Discard>());
  }

  void ParseContext::close_context(Token end_token, const SourceSpan& end_span)
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
    if (frame->get_source().valid() && end_span.valid())
    {
      SourceRef source = frame->get_source();
      source.span.end = end_span.end;
      frame->set_source(source);
    }
    stack.pop_back();
    closing_tokens.pop_back();
    append(std::move(frame));
  }

  void ParseContext::append(std::shared_ptr<AST::ASTNode> obj)
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

  sptr_ast_node_v Parser::parse_sexps(std::vector<TokenSymbol> symbols,
                                      uint32_t source_file_id,
                                      bool source_diagnostics) const
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
        ctx.begin_list(sym.token, source_ref(source_file_id, sym.span, source_diagnostics));
        break;
      case Token::RPAREN:
      case Token::RBRACKET:
      case Token::RCURLY:
        ctx.close_context(sym.token, sym.span);
        break;
      case Token::STRING:
        ctx.append(with_source(std::make_shared<AST::String>(sym.value),
                               source_ref(source_file_id, sym.span, source_diagnostics)));
        break;
      case Token::CHAR:
        ctx.append(with_source(std::make_shared<AST::Char>(sym.value.at(0)),
                               source_ref(source_file_id, sym.span, source_diagnostics)));
        break;
      case Token::KEYWORD:
        ctx.append(with_source(std::make_shared<AST::Keyword>(sym.value),
                               source_ref(source_file_id, sym.span, source_diagnostics)));
        break;
      case Token::SYMBOL:
        ctx.append(with_source(std::make_shared<AST::Symbol>(sym.value),
                               source_ref(source_file_id, sym.span, source_diagnostics)));
        break;
      case Token::NUMBER:
      {
        auto number = AST::Number::make(sym.value);
        if (!source_diagnostics)
        {
          ctx.append(number);
        }
        else
        {
          ctx.append(with_source(std::make_shared<AST::Number>(*number),
                                 source_ref(source_file_id, sym.span, source_diagnostics)));
        }
        break;
      }
      case Token::SQUOT:
      {
        SourceSpan quote_span = sym.span;
        eof_check(symbols, offset);
        sym = symbols.at(offset++);
        if (sym.token == Token::LPAREN)
        {
          SourceSpan quoted_list_span = quote_span;
          quoted_list_span.end = sym.span.end;
          ctx.begin_list(Token::SQUOT,
                         source_ref(source_file_id, quoted_list_span, source_diagnostics));
        }
        else if (sym.token == Token::SYMBOL)
        {
          SourceSpan quoted_symbol_span = quote_span;
          quoted_symbol_span.end = sym.span.end;
          ctx.append(
            with_source(std::make_shared<AST::QuotedSymbol>(sym.value),
                        source_ref(source_file_id, quoted_symbol_span, source_diagnostics)));
        }
        else
        {
          throw ParseException("Unexpected token: " + sym.value);
        }
        break;
      }
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
} // namespace Roo
