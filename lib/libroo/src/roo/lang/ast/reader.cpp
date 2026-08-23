#include "roo/lang/ast/reader.h"

#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/io/file_system.h>
#include <roo/reader.h>
#include <roo/runtime.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>
#include <roo/source.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    sptr_val source_position_value(const SourcePosition& position, KeywordPool& keywords)
    {
      if (!position.valid())
      {
        return Constant::NIL;
      }

      return Value::map({
        Value::keyword("line", keywords),
        Value::number(static_cast<int>(position.line)),
        Value::keyword("column", keywords),
        Value::number(static_cast<int>(position.column)),
      });
    }

    sptr_val source_value(const SourceMap& source_map,
                          const SourceRef& source,
                          KeywordPool& keywords)
    {
      return Value::map({
        Value::keyword("path", keywords),
        Value::string(source.valid() ? source_map.file_name(source.file_id) : ""),
        Value::keyword("line", keywords),
        source.valid() ? Value::number(static_cast<int>(source.span.start.line))
                       : Constant::NIL,
        Value::keyword("column", keywords),
        source.valid() ? Value::number(static_cast<int>(source.span.start.column))
                       : Constant::NIL,
        Value::keyword("start", keywords),
        source_position_value(source.span.start, keywords),
        Value::keyword("end", keywords),
        source_position_value(source.span.end, keywords),
      });
    }

    sptr_val form_type_value(Form form, KeywordPool& keywords)
    {
      switch (form)
      {
      case Form::VECTOR:
        return Value::keyword("vector", keywords);
      case Form::BOOLEAN:
      case Form::B_TRUE:
      case Form::B_FALSE:
        return Value::keyword("boolean", keywords);
      case Form::CHAR:
        return Value::keyword("char", keywords);
      case Form::KEYWORD:
        return Value::keyword("keyword", keywords);
      case Form::LIST:
        return Value::keyword("list", keywords);
      case Form::MAP:
        return Value::keyword("map", keywords);
      case Form::NIL:
        return Value::keyword("nil", keywords);
      case Form::NUMBER:
        return Value::keyword("number", keywords);
      case Form::STRING:
        return Value::keyword("string", keywords);
      case Form::SYMBOL:
        return Value::keyword("symbol", keywords);
      case Form::QUOTED_SYMBOL:
        return Value::keyword("quoted-symbol", keywords);
      case Form::DISCARD:
        return Value::keyword("discard", keywords);
      case Form::FUNCTION:
        return Value::keyword("function", keywords);
      case Form::MACRO:
        return Value::keyword("macro", keywords);
      case Form::ANY:
        return Value::keyword("any", keywords);
      }
      return Value::keyword("unknown", keywords);
    }

    bool has_children(Form form)
    {
      return form == Form::LIST || form == Form::VECTOR || form == Form::MAP;
    }

    sptr_val ast_node_value(const SourceMap& source_map,
                            const sptr_ast_node& form,
                            KeywordPool& keywords)
    {
      sptr_val_v children;
      if (has_children(form->get_type()))
      {
        for (const auto& child : form->get_children())
        {
          children.push_back(ast_node_value(source_map, child, keywords));
        }
      }

      return Value::map({
        Value::keyword("type", keywords),
        form_type_value(form->get_type(), keywords),
        Value::keyword("form", keywords),
        to_rt_value(*form),
        Value::keyword("source", keywords),
        source_value(source_map, form->get_source(), keywords),
        Value::keyword("children", keywords),
        Value::vector(std::move(children)),
      });
    }

    sptr_val read_source_forms(const std::string& source,
                               const std::string& source_name,
                               const std::string& operation,
                               KeywordPool& keywords)
    {
      SourceMap source_map;
      const uint32_t source_id = source_map.intern_file(source_name);
      Reader reader;
      sptr_ast_node_v forms;
      try
      {
        forms = reader.read_sexps(source, source_id, true);
      }
      catch (const std::exception& e)
      {
        throw RooException(operation + " failed for '" + source_name + "': " + e.what());
      }

      sptr_val_v entries;
      entries.reserve(forms.size());
      for (const auto& form : forms)
      {
        entries.push_back(ast_node_value(source_map, form, keywords));
      }
      return Value::vector(std::move(entries));
    }

    std::string read_string_source_name(sptr_val_v& args)
    {
      if (args.size() < 2)
      {
        return "<string>";
      }

      sptr_val path = Dict::get_property(*args[1], "path");
      if (path->type == Value::Type::NIL)
      {
        return "<string>";
      }
      if (path->type != Value::Type::STRING)
      {
        throw TypeError("roo.ast/read-string option :path must be a string, got: " +
                        path->to_string());
      }
      return path->str();
    }
  } // namespace

  /** AstSlurpBangFunction - roo.ast/slurp! */
  FUNC_IMPL(AstSlurpBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&AstSlurpBangFunction::exec_slurp))))

  EXEC_BODY(AstSlurpBangFunction, exec_slurp)
  {
    const std::string path = args[0]->str();
    try
    {
      return read_source_forms(ctx.file_system().read(path),
                               path,
                               "roo.ast/slurp!",
                               ctx.get_runtime().keyword_pool());
    }
    catch (const std::exception& e)
    {
      throw RooException("Could not read: '" + path + "': " + e.what());
    }
  }

  /** AstReadStringFunction - roo.ast/read-string */
  FUNC_IMPL(AstReadStringFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING)),
                       EXEC_DISPATCH(&AstReadStringFunction::exec_read_string)),
                      (FN_ARGS((&Type::STRING), (&Type::MAP)),
                       EXEC_DISPATCH(&AstReadStringFunction::exec_read_string))))

  EXEC_BODY(AstReadStringFunction, exec_read_string)
  {
    return read_source_forms(args[0]->str(),
                             read_string_source_name(args),
                             "roo.ast/read-string",
                             ctx.get_runtime().keyword_pool());
  }
} // namespace Roo
