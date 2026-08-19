#ifndef INPOOTS__TERMINAL__DECODER_H
#define INPOOTS__TERMINAL__DECODER_H

#include <optional>
#include <string>
#include <string_view>

#include <terminal/event.h>

namespace Roo::Inpoots::Terminal
{
  class Decoder
  {
   private:
    std::string pending;
    bool paste = false;
    std::string pasted_text;

   public:
    void append(std::string_view bytes);
    std::optional<Event> next();
    std::optional<Event> flush_escape();
    bool awaiting_escape() const;
    bool empty() const;
  };
} // namespace Roo::Inpoots::Terminal

#endif /* INPOOTS__TERMINAL__DECODER_H */
