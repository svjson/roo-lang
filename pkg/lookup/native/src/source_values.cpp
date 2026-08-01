#include "source_values.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Roo::Lookup
{
  std::string read_file(const std::filesystem::path& path)
  {
    errno = 0;
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
      std::string message = "Could not read file: " + path.string();
      if (errno != 0)
      {
        message += ": ";
        message += std::strerror(errno);
      }
      throw std::runtime_error(message);
    }

    std::ostringstream contents;
    contents << input.rdbuf();
    if (input.bad())
    {
      throw std::runtime_error("Could not read file: " + path.string());
    }
    return contents.str();
  }

  sptr_val source_position_value(const SourcePosition& position)
  {
    if (!position.valid())
    {
      return Constant::NIL;
    }

    return Value::map({
      Value::keyword("line"),
      Value::number(static_cast<int>(position.line)),
      Value::keyword("column"),
      Value::number(static_cast<int>(position.column)),
    });
  }

  sptr_val source_value(const SourceMap& source_map, const SourceRef& source)
  {
    return Value::map({
      Value::keyword("path"),
      Value::string(source.valid() ? source_map.file_name(source.file_id) : ""),
      Value::keyword("line"),
      source.valid() ? Value::number(static_cast<int>(source.span.start.line))
                     : Constant::NIL,
      Value::keyword("column"),
      source.valid() ? Value::number(static_cast<int>(source.span.start.column))
                     : Constant::NIL,
      Value::keyword("start"),
      source_position_value(source.span.start),
      Value::keyword("end"),
      source_position_value(source.span.end),
    });
  }

  sptr_val range_value(const SourceRef& source)
  {
    if (!source.valid())
    {
      return Constant::NIL;
    }

    return Value::map({
      Value::keyword("start"),
      source_position_value(source.span.start),
      Value::keyword("end"),
      source_position_value(source.span.end),
    });
  }

  sptr_val form_entry_value(const SourceMap& source_map, const sptr_ast_node& form)
  {
    return Value::map({
      Value::keyword("form"),
      to_rt_value(*form),
      Value::keyword("source"),
      source_value(source_map, form->get_source()),
    });
  }
} // namespace Roo::Lookup
