#include "roo/lang/tty/tty.h"

#include <cstdio>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>

#include <sys/ioctl.h>
#endif

#include <roo/exec.h>
#include <roo/runtime.h>
#include <roo/runtime/value.h>

namespace Roo
{
  namespace
  {
    bool stdout_is_tty()
    {
#ifdef _WIN32
      return _isatty(_fileno(stdout)) != 0;
#else
      return isatty(fileno(stdout)) != 0;
#endif
    }
  } // namespace

  /** TtyPFunction - roo.tty/tty? */
  FUNC_IMPL(TtyPFunction, SIG((NO_ARGS, EXEC_DISPATCH(&TtyPFunction::exec_tty_p))));

  EXEC_BODY(TtyPFunction, exec_tty_p)
  {
    return Value::boolean(stdout_is_tty());
  }

  /** DimensionsBangFunction - roo.tty/dimensions! */
  FUNC_IMPL(DimensionsBangFunction,
            SIG((NO_ARGS, EXEC_DISPATCH(&DimensionsBangFunction::exec_dimensions))));

  EXEC_BODY(DimensionsBangFunction, exec_dimensions)
  {
    if (!stdout_is_tty())
    {
      return Constant::NIL;
    }

#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (handle == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(handle, &info))
    {
      return Constant::NIL;
    }
    const long columns = static_cast<long>(info.srWindow.Right - info.srWindow.Left + 1);
    const long rows = static_cast<long>(info.srWindow.Bottom - info.srWindow.Top + 1);
#else
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0)
    {
      return Constant::NIL;
    }
    const long columns = static_cast<long>(ws.ws_col);
    const long rows = static_cast<long>(ws.ws_row);
#endif

    KeywordPool& keywords = ctx.get_runtime().keyword_pool();
    return Value::map({Value::keyword("columns", keywords),
                       Value::number(columns),
                       Value::keyword("rows", keywords),
                       Value::number(rows)});
  }
} // namespace Roo
