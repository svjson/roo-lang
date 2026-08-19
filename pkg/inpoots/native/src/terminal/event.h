#ifndef INPOOTS__TERMINAL__EVENT_H
#define INPOOTS__TERMINAL__EVENT_H

#include <string>
#include <vector>

#include <roo/runtime/value.h>

namespace Roo::Inpoots::Terminal
{
  enum class EventType
  {
    TEXT,
    KEY,
    PASTE,
    RESIZE,
    EOF_EVENT,
    INTERRUPT
  };

  struct Event
  {
    EventType type;
    std::string text;
    std::string key;
    std::vector<std::string> modifiers;
    std::string sequence;
    int columns = 0;
    int rows = 0;

    explicit Event(EventType type)
      : type(type)
    {
    }

    static Event text_event(std::string text, std::vector<std::string> modifiers = {});
    static Event key_event(std::string key,
                           std::vector<std::string> modifiers = {},
                           std::string sequence = {});
    static Event paste_event(std::string text);
    static Event resize_event(int columns, int rows);
    static Event eof_event();
    static Event interrupt_event();

    sptr_val value() const;
  };
} // namespace Roo::Inpoots::Terminal

#endif /* INPOOTS__TERMINAL__EVENT_H */
