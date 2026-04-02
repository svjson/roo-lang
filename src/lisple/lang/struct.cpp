
#include <lisple/host.h>
#include <lisple/lang/struct.h>
#include <lisple/runtime/dict.h>
#include <lisple/runtime/seq.h>
#include <lisple/runtime/value.h>

namespace Lisple
{
  /* AssocFunction - assoc */
  FUNC_IMPL(AssocFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&AssocFunction::exec_assoc))))

  EXEC_BODY(AssocFunction, exec_assoc)
  {
    sptr_rtval_v new_content = Lisple::get_children(*args[0]);
    sptr_rtval& assoc_key = args[1];
    sptr_rtval& value = args.back();

    bool found = false;
    for (size_t i = 0; i < new_content.size(); i += 2)
    {
      if (*new_content[i] == *assoc_key)
      {
        new_content[i + 1] = value;
        found = true;
        break;
      }
    }

    if (!found)
    {
      new_content.push_back(assoc_key);
      new_content.push_back(value);
    }

    return RTValue::map(std::move(new_content));
  }

  /* AssocBangFunction - assoc! */
  FUNC_IMPL(AssocBangFunction,
            MULTI_SIG((FN_ARGS((&Type::COMPLEX), (&Type::ANY), (VARARG, &Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_bang)),
                      (FN_ARGS((&Type::SEQ), (&Type::NUMBER), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocBangFunction::exec_assoc_seq_bang))))

  EXEC_BODY(AssocBangFunction, exec_assoc_bang)
  {
    if (args.size() % 2 == 0)
    {
      throw Lisple::InvocationException("No value given for key '" +
                                        args.back()->to_string() + " '");
    }

    for (size_t i = 1; i < args.size() - 1; i += 2)
    {
      Dict::set_property(args[0], args[i], args[i + 1]);
    }

    return args[0];
  }

  EXEC_BODY(AssocBangFunction, exec_assoc_seq_bang)
  {
    if (auto* elements = std::get_if<sptr_rtval_v>(&args[0]->value))
    {
      size_t index = args[1]->i32();
      (*elements)[index] = args.back();
    }
    else
    {
      throw TypeError("assoc! on seq by index not implemented for value type: " +
                      std::to_string((int)args[0]->type));
    }

    return args[0];
  }

  /* AssocInBangFunction - assoc-in! */
  FUNC_IMPL(AssocInBangFunction,
            SIG((FN_ARGS((&Type::COMPLEX), (&Type::ARRAY), (&Type::ANY)),
                 EXEC_DISPATCH(&AssocInBangFunction::inv_assoc_in_bang))))

  FUNC_BODY(AssocInBangFunction, inv_assoc_in_bang)
  {
    sptr_sobject assoc_path = args[1];
    if (assoc_path->get_children().empty())
    {
      throw InvocationException("Path for assoc-in! cannot be empty.");
    }
    sptr_sobject value = args.back();
    sptr_sobject assoc_key = assoc_path->get_children().back();

    sptr_sobject& target = args[0];
    for (size_t i = 0; i < assoc_path->get_children().size() - 1; i++)
    {
      target = target->get_sptr_property(*assoc_path->get_children()[i]);
    }

    if (Lisple::Type::MAP.is_type_of(*args[0]))
    {
      args[0]->set_property(assoc_key, value);
    }
    else if (Lisple::Type::HOST_OBJECT.is_type_of(*args[0]))
    {
      args[0]->set_property(&ctx, *assoc_key, value);
    }
    else
    {
      throw Lisple::TypeError("Cannot set key " + assoc_key->to_string() + " of " +
                              args[0]->to_string());
    }

    return args[0];
  }

  FUNC_IMPL(GetFunction,
            SIG((FN_ARGS((&Type::ANY), (&Type::ANY)),
                 EXEC_DISPATCH(&GetFunction::exec_get))))

  /** GetFunction - get */
  EXEC_BODY(GetFunction, exec_get)
  {
    return Dict::get_property(args[0], args[1]);
  }

} // namespace Lisple
