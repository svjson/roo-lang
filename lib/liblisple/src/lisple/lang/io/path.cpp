#include "lisple/lang/io/path.h"

#include <filesystem>

#include <lisple/context.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  /** JoinPathFunction - lisple.io/join-path */
  FUNC_IMPL(JoinPathFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&JoinPathFunction::exec_join_path))))

  EXEC_BODY(JoinPathFunction, exec_join_path)
  {
    return Value::string(
      (std::filesystem::path(args[0]->str()) / args[1]->str()).lexically_normal().string());
  }

  /** ParentPathFunction - lisple.io/parent-path */
  FUNC_IMPL(ParentPathFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ParentPathFunction::exec_parent_path))))

  EXEC_BODY(ParentPathFunction, exec_parent_path)
  {
    return Value::string(std::filesystem::path(args[0]->str()).parent_path().string());
  }

  /** BasenameFunction - lisple.io/basename */
  FUNC_IMPL(BasenameFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&BasenameFunction::exec_basename))))

  EXEC_BODY(BasenameFunction, exec_basename)
  {
    return Value::string(std::filesystem::path(args[0]->str()).filename().string());
  }

  /** DirnameFunction - lisple.io/dirname */
  FUNC_IMPL(DirnameFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&DirnameFunction::exec_dirname))))

  EXEC_BODY(DirnameFunction, exec_dirname)
  {
    return Value::string(std::filesystem::path(args[0]->str()).parent_path().string());
  }

  /** ExtensionFunction - lisple.io/extension */
  FUNC_IMPL(ExtensionFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ExtensionFunction::exec_extension))))

  EXEC_BODY(ExtensionFunction, exec_extension)
  {
    return Value::string(std::filesystem::path(args[0]->str()).extension().string());
  }
} // namespace Lisple
