#ifndef ROO__RUNTIME__FILE_WALK_H
#define ROO__RUNTIME__FILE_WALK_H

#include <string>

#include <roo/runtime/value.h>

namespace Roo
{
  class Context;

  namespace FileWalk
  {
    struct Options
    {
      sptr_val keep;
      sptr_val descend;
      bool hidden = false;
    };

    /*!
     * @brief Walk a filesystem tree, invoking optional Roo criteria per entry.
     *
     * The keep and descend callables receive an entry map and an extensible
     * metadata map. Immediate children are materialized only after descend
     * accepts a directory, and the directory resource is released before any
     * child is visited.
     */
    sptr_val walk(Context& ctx, const std::string& root, const Options& options = {});
  } // namespace FileWalk
} // namespace Roo

#endif
