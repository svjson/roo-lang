
#include "struct.h"

#include "../host.h"
#include "../runtime/dict.h"
#include "../runtime/value.h"

namespace Lisple
{
  /* AssocFunction - assoc */
  FUNC_IMPL(AssocFunction,
            MULTI_SIG((FN_ARGS((&Type::MAP), (&Type::ANY), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::assoc_map)),
                      (FN_ARGS((&Type::HOST_OBJECT), (&Type::ANY), (&Type::ANY)),
                       EXEC_DISPATCH(&AssocFunction::assoc_ho))))

  FUNC_BODY(AssocFunction, assoc_map)
  {
    Map& map = args[0]->as<Map>();
    sptr_sobject_v new_content;
    sptr_sobject assoc_key = args[1];
    sptr_sobject value = args.back();

    for (auto key : map.key_ptrs())
    {
      new_content.push_back(key);
      if (*key != *assoc_key)
      {
        new_content.push_back(map.get_sptr_property(*key));
      }
      else
      {
        new_content.push_back(value);
        value.reset();
      }
    }

    if (value.get())
    {
      new_content.push_back(assoc_key);
      new_content.push_back(value);
    }

    return std::make_shared<Map>(std::move(new_content));
  }

  FUNC_BODY(AssocFunction, assoc_ho)
  {
    AbstractHostObject& source = args[0]->as<AbstractHostObject>();
    sptr_sobject_v new_content;
    sptr_sobject assoc_key = args[1];
    sptr_sobject value = args.back();

    for (auto key : source.keys())
    {
      new_content.push_back(key);
      if (*key != *assoc_key)
      {
        new_content.push_back(source.get_sptr_property(*key));
      }
      else
      {
        new_content.push_back(value);
        value.reset();
      }
    }

    if (value.get())
    {
      new_content.push_back(assoc_key);
      new_content.push_back(value);
    }

    return std::make_shared<Map>(std::move(new_content));
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
    throw LispleException("Setting seq element by index with assoc! is not supported.");
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

} // namespace Lisple
