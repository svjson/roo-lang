#include "roo/lang/io/file.h"

#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/form.h>
#include <roo/io/file_system.h>
#include <roo/reader.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    struct EdnWriteOptions
    {
      bool pretty = false;
      int indent = 2;
    };

    bool option_bool(Value& options,
                     const std::string& option_name,
                     bool default_value,
                     const std::string& function_name)
    {
      sptr_val value = Dict::get_property(options, option_name);
      if (value->type == Value::Type::NIL)
      {
        return default_value;
      }
      if (value->type != Value::Type::BOOL)
      {
        throw TypeError(function_name + " option :" + option_name +
                        " must be a boolean, got: " + value->to_string());
      }
      return std::get<bool>(value->value);
    }

    int option_indent_width(Value& options, const std::string& function_name)
    {
      sptr_val value = Dict::get_property(options, "indent");
      if (value->type == Value::Type::NIL)
      {
        return 2;
      }
      if (value->type != Value::Type::NUMBER)
      {
        throw TypeError(function_name +
                        " option :indent must be a number, got: " + value->to_string());
      }

      const int indent = value->num().get_int();
      if (indent < 1)
      {
        throw TypeError(function_name + " option :indent must be greater than zero.");
      }
      return indent;
    }

    EdnWriteOptions parse_edn_write_options(Value& options)
    {
      return {option_bool(options, "pretty?", false, "roo.io/spit-edn!"),
              option_indent_width(options, "roo.io/spit-edn!")};
    }

    sptr_val edn_to_rt_value(const AST::ASTNode& obj)
    {
      switch (obj.get_type())
      {
      case Form::VECTOR:
      {
        sptr_val_v elements;
        for (const auto& child : obj.as<AST::Vector>().children)
        {
          elements.push_back(edn_to_rt_value(*child));
        }
        return Value::vector(std::move(elements));
      }
      case Form::LIST:
      {
        sptr_val_v elements;
        for (const auto& child : obj.as<AST::List>().children)
        {
          elements.push_back(edn_to_rt_value(*child));
        }
        return Value::list(std::move(elements));
      }
      case Form::MAP:
      {
        sptr_val_v elements;
        for (const auto& child : obj.as<AST::Map>().children)
        {
          elements.push_back(edn_to_rt_value(*child));
        }
        return Value::map(std::move(elements));
      }
      case Form::SYMBOL:
      {
        const auto& value = obj.as<AST::Symbol>().value;
        if (value == "true")
        {
          return Constant::BOOL_TRUE;
        }
        if (value == "false")
        {
          return Constant::BOOL_FALSE;
        }
        if (value == "nil")
        {
          return Constant::NIL;
        }
        return Value::symbol(value);
      }
      default:
        return to_rt_value(obj);
      }
    }

    sptr_val read_edn_string(const std::string& source, const std::string& source_name)
    {
      Reader reader;
      sptr_ast_node_v forms;
      try
      {
        forms = reader.read_sexps(source);
      }
      catch (const ParseException& e)
      {
        throw ParseException("Error parsing EDN '" + source_name + "': " + e.what());
      }
      if (forms.empty())
      {
        throw RooException("No EDN form found in '" + source_name + "'");
      }
      if (forms.size() > 1)
      {
        throw RooException("Expected one EDN form in '" + source_name + "', found " +
                           std::to_string(forms.size()));
      }
      return edn_to_rt_value(*forms.front());
    }
  } // namespace

  /** SlurpBangFunction - roo.io/slurp! */
  FUNC_IMPL(SlurpBangFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&SlurpBangFunction::exec_slurp))))

  EXEC_BODY(SlurpBangFunction, exec_slurp)
  {
    return Value::string(ctx.file_system().read(args[0]->str()));
  }

  /** SpitBangFunction - roo.io/spit! */
  FUNC_IMPL(SpitBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&SpitBangFunction::exec_spit))))

  EXEC_BODY(SpitBangFunction, exec_spit)
  {
    ctx.file_system().write(args[0]->str(), args[1]->str());
    return Constant::NIL;
  }

  /** CopyFileBangFunction - roo.io/copy-file! */
  FUNC_IMPL(CopyFileBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&CopyFileBangFunction::exec_copy_file))))

  EXEC_BODY(CopyFileBangFunction, exec_copy_file)
  {
    ctx.file_system().copy_file(args[0]->str(), args[1]->str());
    return Constant::NIL;
  }

  /** RemoveTreeBangFunction - roo.io/remove-tree! */
  FUNC_IMPL(RemoveTreeBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&RemoveTreeBangFunction::exec_remove_tree))))

  EXEC_BODY(RemoveTreeBangFunction, exec_remove_tree)
  {
    ctx.file_system().remove_tree(args[0]->str());
    return Constant::NIL;
  }

  /** SymlinkBangFunction - roo.io/symlink! */
  FUNC_IMPL(SymlinkBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&SymlinkBangFunction::exec_symlink))))

  EXEC_BODY(SymlinkBangFunction, exec_symlink)
  {
    ctx.file_system().create_symlink(args[0]->str(), args[1]->str());
    return Constant::NIL;
  }

  /** SymlinkPFunction - roo.io/symlink? */
  FUNC_IMPL(SymlinkPFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&SymlinkPFunction::exec_symlink_p))))

  EXEC_BODY(SymlinkPFunction, exec_symlink_p)
  {
    return ctx.file_system().is_symlink(args[0]->str()) ? Constant::BOOL_TRUE
                                                        : Constant::BOOL_FALSE;
  }

  /** ReadLinkBangFunction - roo.io/read-link! */
  FUNC_IMPL(ReadLinkBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&ReadLinkBangFunction::exec_read_link))))

  EXEC_BODY(ReadLinkBangFunction, exec_read_link)
  {
    return Value::string(ctx.file_system().read_symlink(args[0]->str()));
  }

  /** SlurpEdnBangFunction - roo.io/slurp-edn! */
  FUNC_IMPL(SlurpEdnBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&SlurpEdnBangFunction::exec_slurp_edn))))

  EXEC_BODY(SlurpEdnBangFunction, exec_slurp_edn)
  {
    const std::string& file_name = args[0]->str();
    return read_edn_string(ctx.file_system().read(file_name), file_name);
  }

  /** SpitEdnBangFunction - roo.io/spit-edn! */
  FUNC_IMPL(SpitEdnBangFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING), (&Type::ANY)),
                       EXEC_DISPATCH(&SpitEdnBangFunction::exec_spit_edn)),
                      (FN_ARGS((&Type::STRING), (&Type::ANY), (&Type::MAP)),
                       EXEC_DISPATCH(&SpitEdnBangFunction::exec_spit_edn))))

  EXEC_BODY(SpitEdnBangFunction, exec_spit_edn)
  {
    if (args.size() > 2)
    {
      const EdnWriteOptions options = parse_edn_write_options(*args[2]);
      ctx.file_system().write(
        args[0]->str(),
        options.pretty ? args[1]->to_pretty_string(options.indent) : args[1]->to_string());
    }
    else
    {
      ctx.file_system().write(args[0]->str(), args[1]->to_string());
    }
    return Constant::NIL;
  }

} // namespace Roo
