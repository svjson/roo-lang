#include "roo/lang/io/path.h"

#include <filesystem>

#include <roo/context.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  /** BasenameFunction - roo.io/basename */
  FUNC_IMPL(BasenameFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&BasenameFunction::exec_basename))))

  EXEC_BODY(BasenameFunction, exec_basename)
  {
    return Value::string(std::filesystem::path(args[0]->str()).filename().string());
  }

  /** DirnameFunction - roo.io/dirname */
  FUNC_IMPL(DirnameFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&DirnameFunction::exec_dirname))))

  EXEC_BODY(DirnameFunction, exec_dirname)
  {
    return Value::string(std::filesystem::path(args[0]->str()).parent_path().string());
  }

  /** ExtensionFunction - roo.io/extension */
  FUNC_IMPL(ExtensionFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ExtensionFunction::exec_extension))))

  EXEC_BODY(ExtensionFunction, exec_extension)
  {
    return Value::string(std::filesystem::path(args[0]->str()).extension().string());
  }

  /** JoinPathFunction - roo.io/join-path */
  FUNC_IMPL(JoinPathFunction,
            SIG((FN_ARGS((&Type::STRING), (VARARG, &Type::STRING)),
                 EXEC_DISPATCH(&JoinPathFunction::exec_join_path))))

  EXEC_BODY(JoinPathFunction, exec_join_path)
  {
    std::filesystem::path path;
    for (const auto& segment : args)
    {
      if (segment->type != Value::Type::NIL) path /= segment->str();
    }

    return Value::string(path.lexically_normal().generic_string());
  }

  /** NormalizePathFunction - roo.io/normalize-path */
  FUNC_IMPL(NormalizePathFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&NormalizePathFunction::exec_normalize_path))))

  EXEC_BODY(NormalizePathFunction, exec_normalize_path)
  {
    if (args[0]->type == Value::Type::NIL) return args[0];

    return Value::string(
      std::filesystem::path(args[0]->str()).lexically_normal().generic_string());
  }

  /** ParentPathFunction - roo.io/parent-path */
  FUNC_IMPL(ParentPathFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ParentPathFunction::exec_parent_path))))

  EXEC_BODY(ParentPathFunction, exec_parent_path)
  {
    return Value::string(std::filesystem::path(args[0]->str()).parent_path().string());
  }
} // namespace Roo
