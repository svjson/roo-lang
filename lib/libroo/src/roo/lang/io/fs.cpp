#include "roo/lang/io/fs.h"

#include <algorithm>
#include <string>
#include <vector>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/io/file_system.h>
#include <roo/runtime/dict.h>
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

    sptr_val directory_entry_value(const DirectoryEntry& entry)
    {
      return Value::map({Value::keyword("name"),
                         Value::string(entry.name),
                         Value::keyword("path"),
                         Value::string(entry.path),
                         Value::keyword("type"),
                         type_keyword(entry.type)});
    }

    sptr_val stat_value(const FileSystemStat& stat)
    {
      if (!stat.exists)
      {
        return Value::map({Value::keyword("exists?"),
                           Value::boolean(false),
                           Value::keyword("type"),
                           Constant::NIL,
                           Value::keyword("size"),
                           Constant::NIL,
                           Value::keyword("modified-ms"),
                           Constant::NIL});
      }

      return Value::map({Value::keyword("exists?"),
                         Value::boolean(true),
                         Value::keyword("type"),
                         type_keyword(stat.type),
                         Value::keyword("size"),
                         stat.type == FileSystemEntryType::FILE
                           ? Value::number(static_cast<long>(stat.size))
                           : Constant::NIL,
                         Value::keyword("modified-ms"),
                         Value::number(stat.modified_ms)});
    }

    bool option_bool(Value& options, const std::string& name, bool default_value)
    {
      sptr_val value = Dict::get_property(options, name);
      if (value->type == Value::Type::NIL)
      {
        return default_value;
      }
      if (value->type != Value::Type::BOOL)
      {
        throw TypeError("roo.io/list-directory! option :" + name +
                        " must be a boolean, got: " + value->to_string());
      }
      return std::get<bool>(value->value);
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

    ListDirectoryOptions parse_options(Value& options)
    {
      ListDirectoryOptions parsed;
      parsed.files = option_bool(options, "files?", true);
      parsed.directories = option_bool(options, "directories?", true);
      parsed.hidden =
        option_bool(options, "hidden?", option_bool(options, "dotfiles?", false));
      parsed.filters = option_filters(options);
      return parsed;
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
          values.push_back(directory_entry_value(entry));
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
    return list_directory(ctx, args[0]->str(), parse_options(*args[1]));
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
    return stat_value(ctx.file_system().stat(args[0]->str()));
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
