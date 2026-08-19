#include "roo/runtime/file_walk.h"

#include <algorithm>
#include <filesystem>
#include <utility>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/io/file_system.h>
#include <roo/runtime/walk.h>

namespace Roo::FileWalk
{
  namespace
  {
    struct Metadata
    {
      std::string root;
      std::string relative_path;
      int depth = 0;
      bool root_entry = false;
      bool symlink = false;
    };

    struct Node
    {
      DirectoryEntry entry;
      Metadata metadata;
    };

    sptr_val type_keyword(FileSystemEntryType type)
    {
      switch (type)
      {
      case FileSystemEntryType::FILE:
        return Value::keyword("file");
      case FileSystemEntryType::DIRECTORY:
        return Value::keyword("directory");
      case FileSystemEntryType::OTHER:
        return Value::keyword("other");
      }
      return Value::keyword("other");
    }

    sptr_val entry_value(const DirectoryEntry& entry)
    {
      return Value::map({Value::keyword("name"),
                         Value::string(entry.name),
                         Value::keyword("path"),
                         Value::string(entry.path),
                         Value::keyword("type"),
                         type_keyword(entry.type)});
    }

    sptr_val metadata_value(const Metadata& metadata)
    {
      return Value::map({Value::keyword("root"),
                         Value::string(metadata.root),
                         Value::keyword("relative-path"),
                         Value::string(metadata.relative_path),
                         Value::keyword("depth"),
                         Value::number(metadata.depth),
                         Value::keyword("root?"),
                         Value::boolean(metadata.root_entry),
                         Value::keyword("symlink?"),
                         Value::boolean(metadata.symlink)});
    }

    sptr_val result_value(const sptr_val& entry, const sptr_val& metadata)
    {
      return Value::map(
        {Value::keyword("entry"), entry, Value::keyword("metadata"), metadata});
    }

    std::string relative_child_path(const std::string& parent, const std::string& name)
    {
      if (parent == ".")
      {
        return std::filesystem::path(name).generic_string();
      }
      return (std::filesystem::path(parent) / name).lexically_normal().generic_string();
    }

    bool hidden_path_name(const std::filesystem::path& path)
    {
      const std::string name = path.filename().string();
      return !name.empty() && name != "." && name != ".." && name[0] == '.';
    }

    class Children
    {
     public:
      Children(FileSystem& file_system, bool include_hidden)
        : file_system_(file_system)
        , include_hidden_(include_hidden)
      {
      }

      std::vector<Node> children(const Node& parent)
      {
        std::vector<DirectoryEntry> entries = file_system_.list_directory(parent.entry.path);
        std::sort(entries.begin(),
                  entries.end(),
                  [](const DirectoryEntry& lhs, const DirectoryEntry& rhs)
                  { return lhs.name < rhs.name; });

        std::vector<Node> nodes;
        nodes.reserve(entries.size());
        for (DirectoryEntry& entry : entries)
        {
          if (!include_hidden_ && entry.hidden)
          {
            continue;
          }

          Metadata metadata{
            parent.metadata.root,
            relative_child_path(parent.metadata.relative_path, entry.name),
            parent.metadata.depth + 1,
            false,
            entry.symlink,
          };
          nodes.push_back({std::move(entry), std::move(metadata)});
        }
        return nodes;
      }

     private:
      FileSystem& file_system_;
      bool include_hidden_;
    };

    class Visitor
    {
     public:
      Visitor(Context& ctx, const Options& options, sptr_val_v& results)
        : ctx_(ctx)
        , options_(options)
        , results_(results)
      {
      }

      Walk::Control enter(const Node& node)
      {
        const sptr_val entry = entry_value(node.entry);
        const sptr_val metadata = metadata_value(node.metadata);
        sptr_val_v predicate_args{entry, metadata};

        const bool keep =
          options_.keep ? is_truthy(*invoke_callable(ctx_, options_.keep, predicate_args))
                        : node.entry.type == FileSystemEntryType::FILE;
        if (keep)
        {
          results_.push_back(result_value(entry, metadata));
        }

        if (node.entry.type != FileSystemEntryType::DIRECTORY)
        {
          return Walk::Control::SKIP;
        }

        const bool descend =
          options_.descend
            ? is_truthy(*invoke_callable(ctx_, options_.descend, predicate_args))
            : true;
        if (!descend || node.metadata.symlink)
        {
          return Walk::Control::SKIP;
        }
        return Walk::Control::DESCEND;
      }

      void leave(const Node&) {}

     private:
      Context& ctx_;
      const Options& options_;
      sptr_val_v& results_;
    };

    Node root_node(FileSystem& file_system, const std::string& root)
    {
      const FileSystemStat stat = file_system.stat(root);
      if (!stat.exists)
      {
        throw RooException("Filesystem walk root does not exist: '" + root + "'");
      }

      const std::filesystem::path root_path(root);
      DirectoryEntry entry;
      entry.name = root_path.filename().string();
      if (entry.name.empty()) entry.name = root_path.root_path().string();
      entry.path = root_path.lexically_normal().string();
      entry.type = stat.type;
      entry.hidden = hidden_path_name(root_path);
      entry.symlink = file_system.is_symlink(root);
      const bool root_symlink = entry.symlink;

      return {std::move(entry),
              Metadata{root_path.lexically_normal().string(), ".", 0, true, root_symlink}};
    }
  } // namespace

  sptr_val walk(Context& ctx, const std::string& root, const Options& options)
  {
    sptr_val_v results;
    Children children(ctx.file_system(), options.hidden);
    Visitor visitor(ctx, options, results);
    Walk::walk(root_node(ctx.file_system(), root), children, visitor);
    return Value::vector(std::move(results));
  }
} // namespace Roo::FileWalk
