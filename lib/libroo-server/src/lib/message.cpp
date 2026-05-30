
#include "roo-server/message.h"

#include <optional>
#include <stack>
#include <string>
#include <variant>

namespace Roo::Server
{
  const std::string _MSG = "MSG";
  const std::string _CMD = "CMD";
  const std::string _RESP = "RESP";
  const std::string _ID = "ID";
  const std::string _CODE = "CODE";
  const std::string _BODY = "BODY";

  bool Block::empty() const
  {
    return properties.empty();
  }

  bool Block::has_property(const std::string& name) const
  {
    return properties.contains(name) &&
           std::holds_alternative<const std::string>(properties.at(name));
  }

  bool Block::has_block(const std::string& name) const
  {
    return properties.contains(name) && std::holds_alternative<Block>(properties.at(name));
  }

  Block* Block::add_block(const std::string& name)
  {
    properties.emplace(name, Block());
    return &std::get<Block>(properties[name]);
  }

  void Block::set_property(const std::string& name, const std::string& value)
  {
    properties.emplace(name, value);
  }

  const std::string& Block::get_property(const std::string& name) const
  {
    return std::get<const std::string>(properties.at(name));
  }

  const Block& Block::get_block(const std::string& name) const
  {
    return std::get<Block>(properties.at(name));
  }

  std::string Block::encode() const
  {
    return "!MSG\x1E" + encode_bl() + "/MSG\x1E";
  }

  std::string Block::encode_bl() const
  {
    std::string result;

    for (auto& [prop, value] : properties)
    {
      if (auto* v = std::get_if<const std::string>(&value))
      {
        result += "@" + prop + "=" + *v + "\x1E";
      }
      else
      {
        const Block& block = std::get<Block>(value);
        result += "!" + prop + '\x1E' + block.encode_bl() + "/" + prop + "\x1E";
      }
    }

    return result;
  }

  std::optional<Error> MessageBuilder::begin_block(const std::string& name)
  {
    if (ctx_stack.empty())
    {
      if (name != _MSG)
      {
        return Error{0x00, "Messages must open with an !MSG block"};
      }
      ctx_stack.push({name, &message});
    }
    else if (ctx_stack.top().block->has_property(name))
    {
      return Error{0x00, "Duplicate property " + name};
    }
    else
    {
      ctx_stack.push({name, message.add_block(name)});
    }

    return std::nullopt;
  }

  std::optional<Error> MessageBuilder::set_property(const std::string& name,
                                                    const std::string& value)
  {
    if (ctx_stack.empty())
    {
      return Error{0x00, "Messages must open with an !MSG block"};
    }

    if (ctx_stack.top().block->has_property(name))
    {
      return Error{0x00, "Duplicate property " + name};
    }
    ctx_stack.top().block->set_property(name, value);
    return std::nullopt;
  }

  std::optional<Error> MessageBuilder::end_block(const std::string& name)
  {
    if (ctx_stack.empty() || ctx_stack.top().name != name)
    {
      return Error{0x00, "Unbalanced block: " + name};
    }

    ctx_stack.pop();
    return std::nullopt;
  }

  std::variant<Message, Error> MessageBuilder::finalize_message()
  {
    if (!ctx_stack.empty())
    {
      return Error{0x00, "Incomplete message"};
    }
    if (message.empty())
    {
      return Error{0x00, "Empty message"};
    }
    return message;
  }

  Response MessageBuilder::build_simple_response(const std::string& id,
                                                 const std::string& body)
  {
    MessageBuilder builder;

    builder.begin_block(_MSG);
    builder.begin_block(_RESP);
    builder.set_property(_ID, id);
    builder.set_property(_BODY, body);
    builder.end_block(_RESP);
    builder.end_block(_MSG);

    return std::get<Message>(builder.finalize_message());
  }

  Response MessageBuilder::build_response_with_props(
    const std::string& id,
    const std::string& body,
    const std::map<std::string, std::string>& props)
  {
    MessageBuilder builder;

    builder.begin_block(_MSG);
    builder.begin_block(_RESP);
    builder.set_property(_ID, id);

    for (auto& [p, v] : props)
    {
      builder.set_property(p, v);
    }

    builder.set_property(_BODY, body);
    builder.end_block(_RESP);
    builder.end_block(_MSG);

    return std::get<Message>(builder.finalize_message());
  }

  Response MessageBuilder::build_error_response(const Error& error)
  {
    MessageBuilder builder;

    builder.begin_block(_MSG);
    builder.begin_block(_RESP);
    builder.set_property(_ID, "err");
    builder.set_property(_CODE, std::to_string(static_cast<int>(error.code)));
    builder.set_property(_BODY, error.msg);
    builder.end_block(_RESP);
    builder.end_block(_MSG);

    return std::get<Message>(builder.finalize_message());
  }

  std::variant<Message, Error> MessageParser::parse_message(const std::string& data)
  {
    MessageBuilder builder;
    char c;
    for (size_t i = 0; i < data.size(); i++)
    {
      c = data[i];
      switch (c)
      {
      case '!':
      {
        i++;
        auto r = read_begin_block(data, &i, &builder);
        if (r) return *r;
        break;
      }
      case '@':
      {
        i++;
        auto r = read_property(data, &i, &builder);
        if (r) return *r;
        break;
      }
      case '/':
      {
        i++;
        auto r = read_end_block(data, &i, &builder);
        if (r) return *r;
        break;
      }
      }
    }

    return builder.finalize_message();
  }

  std::variant<std::string, Error> MessageParser::read_symbol(const std::string& data,
                                                              size_t* i)
  {
    std::string symbol;
    size_t begin = *i;
    size_t end = *i;

    char c;
    for (; *i < data.size() && begin == end; (*i)++)
    {
      c = data[*i];
      switch (c)
      {
      case '=':
      case '\x1E':
        end = *i;
        (*i)--;
        break;
      }
    }

    if (begin == end)
    {
      return Error(0x00, "Empty symbol");
    }

    return data.substr(begin, end - begin);
  }

  std::variant<std::string, Error> MessageParser::read_value(const std::string& data,
                                                             size_t* i)
  {
    std::string symbol;
    size_t begin = *i;
    size_t end = *i;
    bool term = false;

    char c;
    for (; *i < data.size() && !term; (*i)++)
    {
      c = data[*i];
      switch (c)
      {
      case '\x1E':
        end = *i;
        term = true;
        (*i)--;
        break;
      }
    }

    if (!term)
    {
      return Error(0x00, "Unterminated value");
    }
    return data.substr(begin, end - begin);
  }

  std::optional<Error> MessageParser::skip_until(char c, const std::string& data, size_t* i)
  {
    for (; *i < data.size(); (*i)++)
    {
      if (data[*i] == c) return std::nullopt;
    }
    return Error(0x00, "Unexpected end of data");
  }

  std::optional<Error> MessageParser::read_begin_block(const std::string& data,
                                                       size_t* i,
                                                       MessageBuilder* builder)
  {
    auto symbol = read_symbol(data, i);
    if (auto* err = std::get_if<Error>(&symbol)) return *err;

    builder->begin_block(std::get<std::string>(symbol));
    return skip_until('\x1E', data, i);
  }

  std::optional<Error> MessageParser::read_end_block(const std::string& data,
                                                     size_t* i,
                                                     MessageBuilder* builder)
  {
    auto symbol = read_symbol(data, i);
    if (auto* err = std::get_if<Error>(&symbol)) return *err;

    builder->end_block(std::get<std::string>(symbol));
    return skip_until('\x1E', data, i);
  }

  std::optional<Error> MessageParser::read_property(const std::string& data,
                                                    size_t* i,
                                                    MessageBuilder* builder)
  {
    auto symbol = read_symbol(data, i);
    if (auto* err = std::get_if<Error>(&symbol)) return *err;
    auto skip_err = skip_until('=', data, i);
    if (skip_err) return *skip_err;
    (*i)++;
    auto value = read_value(data, i);
    if (auto* err = std::get_if<Error>(&value)) return *err;
    return builder->set_property(std::get<std::string>(symbol),
                                 std::get<std::string>(value));
  }
} // namespace Roo::Server
