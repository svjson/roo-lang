#include <utility>

#include <terminal/event.h>

namespace Roo::Inpoots::Terminal
{
  namespace
  {
    std::string text_key(const std::string& text)
    {
      if (text == " ") return "space";
      if (text.size() != 1) return {};
      if (text[0] >= 'A' && text[0] <= 'Z')
      {
        return std::string(1, static_cast<char>(text[0] - 'A' + 'a'));
      }
      if ((text[0] >= 'a' && text[0] <= 'z') || (text[0] >= '0' && text[0] <= '9'))
      {
        return text;
      }
      return {};
    }

    sptr_val key_value(const std::string& key, const std::vector<std::string>& modifiers)
    {
      if (modifiers.empty()) return Value::keyword("key/" + key);

      sptr_val_v chord;
      chord.reserve(modifiers.size() + 1);
      for (const auto& modifier : modifiers)
      {
        chord.push_back(Value::keyword("key/" + modifier));
      }
      chord.push_back(Value::keyword("key/" + key));
      return Value::vector(chord);
    }
  } // namespace

  Event Event::keystroke_event(std::string key,
                               std::vector<std::string> modifiers,
                               std::string sequence)
  {
    Event event(EventType::KEYSTROKE);
    event.key = std::move(key);
    event.modifiers = std::move(modifiers);
    event.sequence = std::move(sequence);
    return event;
  }

  Event Event::text_keystroke(std::string text, std::vector<std::string> modifiers)
  {
    Event event = keystroke_event(text_key(text), std::move(modifiers));
    event.text = std::move(text);
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

  sptr_val Event::value() const
  {
    sptr_val_v fields{Value::keyword("type")};
    switch (type)
    {
    case EventType::KEYSTROKE:
      fields.push_back(Value::keyword("keystroke"));
      if (!key.empty())
      {
        fields.push_back(Value::keyword("key"));
        fields.push_back(key_value(key, modifiers));
      }
      if (!text.empty())
      {
        fields.push_back(Value::keyword("text"));
        fields.push_back(Value::string(text));
      }
      if (!control.empty())
      {
        fields.push_back(Value::keyword("control"));
        fields.push_back(Value::keyword(control));
      }
      if (!signal.empty())
      {
        fields.push_back(Value::keyword("signal"));
        fields.push_back(Value::keyword(signal));
      }
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
    }
    if (!sequence.empty())
    {
      fields.push_back(Value::keyword("sequence"));
      fields.push_back(Value::string(sequence));
    }
    return Value::map(fields);
  }
} // namespace Roo::Inpoots::Terminal
