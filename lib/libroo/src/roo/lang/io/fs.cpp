#include "roo/lang/io/fs.h"

#include <algorithm>
#include <string>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/exec.h>
#include <roo/host/schema.h>
#include <roo/io/file_system.h>
#include <roo/runtime.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/file_walk.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    struct ListDirectoryOptions
    {
      bool files = true;
      bool directories = true;
      bool hidden = false;
      std::vector<std::string> filters;
    };

    sptr_val type_keyword(FileSystemEntryType type, KeywordPool& keywords)
    {
      switch (type)
      {
      case FileSystemEntryType::FILE:
        return Value::keyword("file", keywords);
      case FileSystemEntryType::DIRECTORY:
        return Value::keyword("directory", keywords);
      case FileSystemEntryType::OTHER:
        return Value::keyword("other", keywords);
      }
      return Value::keyword("other", keywords);
    }

    sptr_val directory_entry_value(const DirectoryEntry& entry, KeywordPool& keywords)
    {
      return Value::map({Value::keyword("name", keywords),
                         Value::string(entry.name),
                         Value::keyword("path", keywords),
                         Value::string(entry.path),
                         Value::keyword("type", keywords),
                         type_keyword(entry.type, keywords)});
    }

    sptr_val stat_value(const FileSystemStat& stat, KeywordPool& keywords)
    {
      if (!stat.exists)
      {
        return Value::map({Value::keyword("exists?", keywords),
                           Value::boolean(false),
                           Value::keyword("type", keywords),
                           Constant::NIL,
                           Value::keyword("size", keywords),
                           Constant::NIL,
                           Value::keyword("modified-ms", keywords),
                           Constant::NIL});
      }

      return Value::map({Value::keyword("exists?", keywords),
                         Value::boolean(true),
                         Value::keyword("type", keywords),
                         type_keyword(stat.type, keywords),
                         Value::keyword("size", keywords),
                         stat.type == FileSystemEntryType::FILE
                           ? Value::number(static_cast<long>(stat.size))
                           : Constant::NIL,
                         Value::keyword("modified-ms", keywords),
                         Value::number(stat.modified_ms)});
    }

    bool wildcard_class_match(const std::string& pattern,
                              size_t open_i,
                              char value,
                              size_t& next_i)
    {
      const size_t close_i = pattern.find(']', open_i + 1);
      if (close_i == std::string::npos)
      {
        next_i = open_i + 1;
        return value == '[';
      }

      size_t item_i = open_i + 1;
      bool negate = false;
      if (item_i < close_i && (pattern[item_i] == '!' || pattern[item_i] == '^'))
      {
        negate = true;
        item_i++;
      }

      bool matched = false;
      while (item_i < close_i)
      {
        if (item_i + 2 < close_i && pattern[item_i + 1] == '-')
        {
          matched = matched || (pattern[item_i] <= value && value <= pattern[item_i + 2]);
          item_i += 3;
        }
        else
        {
          matched = matched || pattern[item_i] == value;
          item_i++;
        }
      }

      next_i = close_i + 1;
      return negate ? !matched : matched;
    }

    bool wildcard_match(const std::string& pattern,
                        const std::string& text,
                        size_t pattern_i = 0,
                        size_t text_i = 0)
    {
      while (pattern_i < pattern.size())
      {
        if (pattern[pattern_i] == '*')
        {
          while (pattern_i + 1 < pattern.size() && pattern[pattern_i + 1] == '*')
          {
            pattern_i++;
          }
          if (pattern_i + 1 == pattern.size())
          {
            return true;
          }
          for (size_t i = text_i; i <= text.size(); i++)
          {
            if (wildcard_match(pattern, text, pattern_i + 1, i))
            {
              return true;
            }
          }
          return false;
        }

        if (text_i >= text.size())
        {
          return false;
        }
        if (pattern[pattern_i] == '[')
        {
          size_t next_pattern_i = pattern_i;
          if (!wildcard_class_match(pattern, pattern_i, text[text_i], next_pattern_i))
          {
            return false;
          }
          pattern_i = next_pattern_i;
          text_i++;
          continue;
        }
        if (pattern[pattern_i] != '?' && pattern[pattern_i] != text[text_i])
        {
          return false;
        }
        pattern_i++;
        text_i++;
      }

      return text_i == text.size();
    }

    std::vector<std::string> option_filters(Value& options)
    {
      sptr_val value = Dict::get_property(options, "filter");
      if (value->type == Value::Type::NIL)
      {
        return {};
      }
      if (value->type == Value::Type::STRING)
      {
        return {value->str()};
      }
      if (value->type == Value::Type::VECTOR)
      {
        std::vector<std::string> filters;
        for (const sptr_val& filter : value->elements())
        {
          if (filter->type != Value::Type::STRING)
          {
            throw TypeError("roo.io/list-directory! option :filter vector must contain "
                            "only strings, got: " +
                            filter->to_string());
          }
          filters.push_back(filter->str());
        }
        return filters;
      }
      throw TypeError(
        "roo.io/list-directory! option :filter must be a string or vector, got: " +
        value->to_string());
    }

    bool matches_options(const DirectoryEntry& entry, const ListDirectoryOptions& options)
    {
      if (entry.type == FileSystemEntryType::FILE && !options.files)
      {
        return false;
      }
      if (entry.type == FileSystemEntryType::DIRECTORY && !options.directories)
      {
        return false;
      }
      if (entry.hidden && !options.hidden)
      {
        return false;
      }
      if (options.filters.empty())
      {
        return true;
      }
      return std::any_of(options.filters.begin(),
                         options.filters.end(),
                         [&](const std::string& filter)
                         { return wildcard_match(filter, entry.name); });
    }

    sptr_val list_directory(Context& ctx,
                            const std::string& path,
                            const ListDirectoryOptions& options)
    {
      sptr_val_v values;
      for (const DirectoryEntry& entry : ctx.file_system().list_directory(path))
      {
        if (matches_options(entry, options))
        {
          values.push_back(directory_entry_value(entry, ctx.get_runtime().keyword_pool()));
        }
      }
      return Value::vector(values);
    }
  } // namespace

  /** ListDirectoryBangFunction - roo.io/list-directory! */
  FUNC_IMPL(
    ListDirectoryBangFunction,
    MULTI_SIG((FN_ARGS((&Type::STRING)),
               EXEC_DISPATCH(&ListDirectoryBangFunction::exec_list_directory)),
              (FN_ARGS((&Type::STRING), (&Type::MAP)),
               EXEC_DISPATCH(&ListDirectoryBangFunction::exec_list_directory_with_options))))

  EXEC_BODY(ListDirectoryBangFunction, exec_list_directory)
  {
    return list_directory(ctx, args[0]->str(), ListDirectoryOptions{});
  }

  EXEC_BODY(ListDirectoryBangFunction, exec_list_directory_with_options)
  {
    static MapSchema schema({},
                            {{"files?", &Type::BOOL},
                             {"directories?", &Type::BOOL},
                             {"hidden?", &Type::BOOL},
                             {"dotfiles?", &Type::BOOL}});
    MapSchema::Inspector opts = schema.bind(ctx, *args[1]);

    ListDirectoryOptions options;
    options.files = opts.boolean("files?", true);
    options.directories = opts.boolean("directories?", true);
    options.hidden = opts.boolean("hidden?", opts.boolean("dotfiles?", false));
    options.filters = option_filters(*args[1]);

    return list_directory(ctx, args[0]->str(), options);
  }

  /** WalkBangFunction - roo.io/walk! */
  FUNC_IMPL(WalkBangFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING)),
                       EXEC_DISPATCH(&WalkBangFunction::exec_walk)),
                      (FN_ARGS((&Type::STRING), (&Type::MAP)),
                       EXEC_DISPATCH(&WalkBangFunction::exec_walk_with_options))))

  EXEC_BODY(WalkBangFunction, exec_walk)
  {
    return FileWalk::walk(ctx, args[0]->str());
  }

  EXEC_BODY(WalkBangFunction, exec_walk_with_options)
  {
    static MapSchema schema(
      {},
      {{"keep?", &Type::FUNCTION}, {"descend?", &Type::FUNCTION}, {"hidden?", &Type::BOOL}});
    MapSchema::Inspector options = schema.bind(ctx, *args[1]);

    FileWalk::Options walk_options;
    walk_options.keep = options.val("keep?");
    if (walk_options.keep->type == Value::Type::NIL) walk_options.keep.reset();
    walk_options.descend = options.val("descend?");
    if (walk_options.descend->type == Value::Type::NIL) walk_options.descend.reset();
    walk_options.hidden = options.boolean("hidden?", false);

    return FileWalk::walk(ctx, args[0]->str(), walk_options);
  }

  /** ExistsPFunction - roo.io/exists? */
  FUNC_IMPL(ExistsPFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&ExistsPFunction::exec_exists_p))))

  EXEC_BODY(ExistsPFunction, exec_exists_p)
  {
    return Value::boolean(ctx.file_system().exists(args[0]->str()));
  }

  /** FilePFunction - roo.io/file? */
  FUNC_IMPL(FilePFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&FilePFunction::exec_file_p))))

  EXEC_BODY(FilePFunction, exec_file_p)
  {
    return Value::boolean(ctx.file_system().is_file(args[0]->str()));
  }

  /** DirectoryPFunction - roo.io/directory? */
  FUNC_IMPL(DirectoryPFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&DirectoryPFunction::exec_directory_p))))

  EXEC_BODY(DirectoryPFunction, exec_directory_p)
  {
    return Value::boolean(ctx.file_system().is_directory(args[0]->str()));
  }

  /** StatBangFunction - roo.io/stat! */
  FUNC_IMPL(StatBangFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&StatBangFunction::exec_stat))))

  EXEC_BODY(StatBangFunction, exec_stat)
  {
    return stat_value(ctx.file_system().stat(args[0]->str()),
                      ctx.get_runtime().keyword_pool());
  }

  /** AbsolutePathBangFunction - roo.io/absolute-path! */
  FUNC_IMPL(AbsolutePathBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&AbsolutePathBangFunction::exec_absolute_path))))

  EXEC_BODY(AbsolutePathBangFunction, exec_absolute_path)
  {
    return Value::string(ctx.file_system().absolute_path(args[0]->str()));
  }

  /** CurrentDirectoryBangFunction - roo.io/current-directory! */
  FUNC_IMPL(CurrentDirectoryBangFunction,
            SIG((NO_ARGS,
                 EXEC_DISPATCH(&CurrentDirectoryBangFunction::exec_current_directory))))

  EXEC_BODY(CurrentDirectoryBangFunction, exec_current_directory)
  {
    return Value::string(ctx.file_system().current_directory());
  }

  /** HomeDirectoryBangFunction - roo.io/home-directory! */
  FUNC_IMPL(HomeDirectoryBangFunction,
            SIG((NO_ARGS, EXEC_DISPATCH(&HomeDirectoryBangFunction::exec_home_directory))))

  EXEC_BODY(HomeDirectoryBangFunction, exec_home_directory)
  {
    return Value::string(ctx.file_system().home_directory());
  }
} // namespace Roo
