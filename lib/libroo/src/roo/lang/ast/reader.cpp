#include "roo/lang/ast/reader.h"

#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/io/file_system.h>
#include <roo/reader.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>
#include <roo/source.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
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

    sptr_val form_type_value(Form form)
    {
      switch (form)
      {
      case Form::VECTOR:
        return Value::keyword("vector");
      case Form::BOOLEAN:
      case Form::B_TRUE:
      case Form::B_FALSE:
        return Value::keyword("boolean");
      case Form::CHAR:
        return Value::keyword("char");
      case Form::KEYWORD:
        return Value::keyword("keyword");
      case Form::LIST:
        return Value::keyword("list");
      case Form::MAP:
        return Value::keyword("map");
      case Form::NIL:
        return Value::keyword("nil");
      case Form::NUMBER:
        return Value::keyword("number");
      case Form::STRING:
        return Value::keyword("string");
      case Form::SYMBOL:
        return Value::keyword("symbol");
      case Form::QUOTED_SYMBOL:
        return Value::keyword("quoted-symbol");
      case Form::DISCARD:
        return Value::keyword("discard");
      case Form::FUNCTION:
        return Value::keyword("function");
      case Form::MACRO:
        return Value::keyword("macro");
      case Form::HOST_OBJECT:
        return Value::keyword("host-object");
      case Form::HOST_SEQ:
        return Value::keyword("host-seq");
      case Form::ANY:
        return Value::keyword("any");
      }
      return Value::keyword("unknown");
    }

    bool has_children(Form form)
    {
      return form == Form::LIST || form == Form::VECTOR || form == Form::MAP;
    }

    sptr_val ast_node_value(const SourceMap& source_map, const sptr_ast_node& form)
    {
      sptr_val_v children;
      if (has_children(form->get_type()))
      {
        for (const auto& child : form->get_children())
        {
          children.push_back(ast_node_value(source_map, child));
        }
      }

      return Value::map({
        Value::keyword("type"),
        form_type_value(form->get_type()),
        Value::keyword("form"),
        to_rt_value(*form),
        Value::keyword("source"),
        source_value(source_map, form->get_source()),
        Value::keyword("children"),
        Value::vector(std::move(children)),
      });
    }

    sptr_val read_source_forms(const std::string& source,
                               const std::string& source_name,
                               const std::string& operation)
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
        entries.push_back(ast_node_value(source_map, form));
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
    return read_source_forms(ctx.file_system().read(path), path, "roo.ast/slurp!");
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
                             "roo.ast/read-string");
  }
} // namespace Roo
