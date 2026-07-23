#include "roo/lang/io/path.h"

#include <filesystem>

#include <roo/context.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  /** JoinPathFunction - roo.io/join-path */
  FUNC_IMPL(JoinPathFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&JoinPathFunction::exec_join_path))))

  EXEC_BODY(JoinPathFunction, exec_join_path)
  {
    return Value::string((std::filesystem::path(args[0]->str()) / args[1]->str())
                           .lexically_normal()
                           .generic_string());
  }

  /** ParentPathFunction - roo.io/parent-path */
  FUNC_IMPL(ParentPathFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ParentPathFunction::exec_parent_path))))

  EXEC_BODY(ParentPathFunction, exec_parent_path)
  {
    return Value::string(std::filesystem::path(args[0]->str()).parent_path().string());
  }

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
} // namespace Roo
