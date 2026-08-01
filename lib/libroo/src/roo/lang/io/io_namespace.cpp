#include "roo/lang/io/io_namespace.h"

#include <map>
#include <string>

#include <roo/lang/io/file.h>
#include <roo/lang/io/fs.h>
#include <roo/lang/io/path.h>
#include <roo/runtime/value.h>

namespace Roo
{
  /*!
   * @brief Create the immutable native roo.io namespace.
   */
  Namespace make_io_namespace()
  {
    std::map<std::string, sptr_val> symbols;
    symbols.emplace("slurp!", SlurpBangFunction::make());
    symbols.emplace("spit!", SpitBangFunction::make());
    symbols.emplace("copy-file!", CopyFileBangFunction::make());
    symbols.emplace("remove-tree!", RemoveTreeBangFunction::make());
    symbols.emplace("symlink!", SymlinkBangFunction::make());
    symbols.emplace("symlink?", SymlinkPFunction::make());
    symbols.emplace("read-link!", ReadLinkBangFunction::make());
    symbols.emplace("slurp-edn!", SlurpEdnBangFunction::make());
    symbols.emplace("spit-edn!", SpitEdnBangFunction::make());
    symbols.emplace("list-directory!", ListDirectoryBangFunction::make());
    symbols.emplace("exists?", ExistsPFunction::make());
    symbols.emplace("file?", FilePFunction::make());
    symbols.emplace("directory?", DirectoryPFunction::make());
    symbols.emplace("stat!", StatBangFunction::make());
    symbols.emplace("absolute-path!", AbsolutePathBangFunction::make());
    symbols.emplace("current-directory!", CurrentDirectoryBangFunction::make());
    symbols.emplace("home-directory!", HomeDirectoryBangFunction::make());
    symbols.emplace("join-path", JoinPathFunction::make());
    symbols.emplace("parent-path", ParentPathFunction::make());
    symbols.emplace("basename", BasenameFunction::make());
    symbols.emplace("dirname", DirnameFunction::make());
    symbols.emplace("extension", ExtensionFunction::make());

    return Namespace::make_lang("roo.io", std::move(symbols));
  }
} // namespace Roo
