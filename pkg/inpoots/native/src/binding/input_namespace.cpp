#include <string>

#include <roo/context.h>
#include <roo/exception.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>

#include <binding/input/input_adapter.h>
#include <binding/input_namespace.h>

namespace Roo::Inpoots
{
  namespace
  {
    Terminal::Mode input_mode(const sptr_val& options)
    {
      const auto& fields = options->elements();
      for (std::size_t i = 0; i < fields.size(); i += 2)
      {
        if (fields[i]->type != Value::Type::KEYWORD || fields[i]->str() != "mode")
        {
          throw InvocationException("with-stdin! received an unknown option: " +
                                    fields[i]->to_string());
        }
      }

      const sptr_val mode = Dict::get_property(options, *Value::keyword("mode"));
      if (mode->type == Value::Type::NIL) return Terminal::Mode::RAW;
      if (mode->type != Value::Type::KEYWORD)
      {
        throw TypeError("with-stdin! :mode must be a keyword.");
      }
      if (mode->str() == "cooked") return Terminal::Mode::COOKED;
      if (mode->str() == "cbreak") return Terminal::Mode::CBREAK;
      if (mode->str() == "raw") return Terminal::Mode::RAW;
      throw InvocationException("Unknown terminal input mode: " + mode->to_string());
    }
  } // namespace

  namespace Function
  {
    /** WithStdinFunction - inpoots.input/with-stdin! */
    FUNC_IMPL(WithStdinFunction,
              SIG((FN_ARGS((&Type::MAP), (&Type::EXEC)),
                   EXEC_DISPATCH(&WithStdinFunction::exec_with_stdin))))

    EXEC_BODY(WithStdinFunction, exec_with_stdin)
    {
      sptr_val input = InputAdapter::make_unique(input_mode(args[0]));
      auto& terminal_input = input->adapter<InputAdapter>().get_object();
      sptr_val_v callback_args{input};
      try
      {
        sptr_val result = args[1]->exec().execute(ctx, callback_args);
        terminal_input.close();
        return result;
      }
      catch (...)
      {
        terminal_input.close();
        throw;
      }
    }

    /** ReadFunction - inpoots.input/read! */
    FUNC_IMPL(ReadFunction,
              SIG((FN_ARGS((&INPOOTS_INPUT_TYPE)), EXEC_DISPATCH(&ReadFunction::exec_read))))

    EXEC_BODY(ReadFunction, exec_read)
    {
      return args[0]->adapter<InputAdapter>().get_object().read_event().value();
    }
  } // namespace Function

  InputNamespace::InputNamespace()
    : Roo::Namespace(std::string(NS__INPOOTS__INPUT))
  {
    set_origin(Namespace::Origin::native());
    store("with-stdin!", Function::WithStdinFunction::make());
    store("read!", Function::ReadFunction::make());
  }
} // namespace Roo::Inpoots
