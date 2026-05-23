#include "lisple/lang/io/file.h"

#include <string>

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/io/file_system.h>
#include <lisple/form.h>
#include <lisple/reader.h>
#include <lisple/runtime/value.h>
#include <lisple/type.h>

namespace Lisple
{
  namespace
  {
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
        throw LispleException("No EDN form found in '" + source_name + "'");
      }
      if (forms.size() > 1)
      {
        throw LispleException("Expected one EDN form in '" + source_name + "', found " +
                              std::to_string(forms.size()));
      }
      return edn_to_rt_value(*forms.front());
    }
  } // namespace

  /** SlurpBangFunction - lisple.io/slurp! */
  FUNC_IMPL(SlurpBangFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&SlurpBangFunction::exec_slurp))))

  EXEC_BODY(SlurpBangFunction, exec_slurp)
  {
    return Value::string(ctx.file_system().read(args[0]->str()));
  }

  /** SpitBangFunction - lisple.io/spit! */
  FUNC_IMPL(SpitBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&SpitBangFunction::exec_spit))))

  EXEC_BODY(SpitBangFunction, exec_spit)
  {
    ctx.file_system().write(args[0]->str(), args[1]->str());
    return Constant::NIL;
  }

  /** CopyFileBangFunction - lisple.io/copy-file! */
  FUNC_IMPL(CopyFileBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::STRING)),
                 EXEC_DISPATCH(&CopyFileBangFunction::exec_copy_file))))

  EXEC_BODY(CopyFileBangFunction, exec_copy_file)
  {
    ctx.file_system().copy_file(args[0]->str(), args[1]->str());
    return Constant::NIL;
  }

  /** RemoveTreeBangFunction - lisple.io/remove-tree! */
  FUNC_IMPL(RemoveTreeBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&RemoveTreeBangFunction::exec_remove_tree))))

  EXEC_BODY(RemoveTreeBangFunction, exec_remove_tree)
  {
    ctx.file_system().remove_tree(args[0]->str());
    return Constant::NIL;
  }

  /** SlurpEdnBangFunction - lisple.io/slurp-edn! */
  FUNC_IMPL(SlurpEdnBangFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&SlurpEdnBangFunction::exec_slurp_edn))))

  EXEC_BODY(SlurpEdnBangFunction, exec_slurp_edn)
  {
    const std::string& file_name = args[0]->str();
    return read_edn_string(ctx.file_system().read(file_name), file_name);
  }

  /** SpitEdnBangFunction - lisple.io/spit-edn! */
  FUNC_IMPL(SpitEdnBangFunction,
            SIG((FN_ARGS((&Type::STRING), (&Type::ANY)),
                 EXEC_DISPATCH(&SpitEdnBangFunction::exec_spit_edn))))

  EXEC_BODY(SpitEdnBangFunction, exec_spit_edn)
  {
    ctx.file_system().write(args[0]->str(), args[1]->to_string());
    return Constant::NIL;
  }

} // namespace Lisple
