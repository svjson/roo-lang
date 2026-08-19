#include <utility>

#include <terminal/event.h>

namespace Roo::Inpoots::Terminal
{
  Event Event::text_event(std::string text, std::vector<std::string> modifiers)
  {
    Event event(EventType::TEXT);
    event.text = std::move(text);
    event.modifiers = std::move(modifiers);
    return event;
  }

  Event Event::key_event(std::string key,
                         std::vector<std::string> modifiers,
                         std::string sequence)
  {
    Event event(EventType::KEY);
    event.key = std::move(key);
    event.modifiers = std::move(modifiers);
    event.sequence = std::move(sequence);
    return event;
  }

  Event Event::paste_event(std::string text)
  {
    Event event(EventType::PASTE);
    event.text = std::move(text);
    return event;
  }

  Event Event::resize_event(int columns, int rows)
  {
    Event event(EventType::RESIZE);
    event.columns = columns;
    event.rows = rows;
    return event;
  }

  Event Event::eof_event()
  {
    return Event(EventType::EOF_EVENT);
  }

  Event Event::interrupt_event()
  {
    return Event(EventType::INTERRUPT);
  }

  sptr_val Event::value() const
  {
    sptr_val_v fields{Value::keyword("type")};
    switch (type)
    {
    case EventType::TEXT:
      fields.push_back(Value::keyword("text"));
      fields.push_back(Value::keyword("text"));
      fields.push_back(Value::string(text));
      break;
    case EventType::KEY:
      fields.push_back(Value::keyword("key"));
      fields.push_back(Value::keyword("key"));
      fields.push_back(Value::keyword(key));
      break;
    case EventType::PASTE:
      fields.push_back(Value::keyword("paste"));
      fields.push_back(Value::keyword("text"));
      fields.push_back(Value::string(text));
      break;
    case EventType::RESIZE:
      fields.push_back(Value::keyword("resize"));
      fields.push_back(Value::keyword("columns"));
      fields.push_back(Value::number(columns));
      fields.push_back(Value::keyword("rows"));
      fields.push_back(Value::number(rows));
      break;
    case EventType::EOF_EVENT:
      fields.push_back(Value::keyword("eof"));
      break;
    case EventType::INTERRUPT:
      fields.push_back(Value::keyword("interrupt"));
      break;
    }

    if (!modifiers.empty())
    {
      sptr_val_v modifier_values;
      modifier_values.reserve(modifiers.size());
      for (const auto& modifier : modifiers)
      {
        modifier_values.push_back(Value::keyword(modifier));
      }
      fields.push_back(Value::keyword("modifiers"));
      fields.push_back(Value::vector(modifier_values));
    }
    if (!sequence.empty())
    {
      fields.push_back(Value::keyword("sequence"));
      fields.push_back(Value::string(sequence));
    }
    return Value::map(fields);
  }
} // namespace Roo::Inpoots::Terminal
