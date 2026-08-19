#ifndef INPOOTS__TERMINAL__INPUT_H
#define INPOOTS__TERMINAL__INPUT_H

#include <cstdint>
#include <memory>

#include <terminal/decoder.h>
#include <terminal/event.h>

namespace Roo::Inpoots::Terminal
{
  enum class Mode
  {
    COOKED,
    CBREAK,
    RAW
  };

  class Input
  {
   private:
    struct State;
    std::unique_ptr<State> state;
    Decoder decoder;
    bool ended = false;
    bool active = true;

   public:
    explicit Input(Mode mode);
    Input(std::intptr_t native_handle, Mode mode);
    ~Input();

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    Event read_event();
    void close();
    bool is_active() const;
  };
} // namespace Roo::Inpoots::Terminal

#endif /* INPOOTS__TERMINAL__INPUT_H */
