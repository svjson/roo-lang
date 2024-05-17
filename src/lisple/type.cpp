
#include "type.h"

#include "form.h"

namespace Lisple
{
  /* TypeRef */
  TypeRef::TypeRef(Lisple::Form form_type, const std::string& name)
    : form_type(form_type)
    , name(name)
  {
  }

  bool TypeRef::is_type_of(const Lisple::Object& obj) const
  {
    if (obj == *NIL) return true;
    return obj.get_type() == form_type;
  }

  CoercionResult TypeRef::coerce(Context&, sptr_sobject&) const
  {
    return CoercionResult { false, nullptr };
  }

  const std::string& TypeRef::to_string() const
  {
    return name;
  }

  /* AnyRef */
  AnyRef::AnyRef()
    : TypeRef(Lisple::Form::ANY, "<any>")
  {
  }

  bool AnyRef::is_type_of(const Lisple::Object&) const
  {
    return true;
  }

  /* MultiRef */
  MultiRef::MultiRef(std::vector<const Lisple::TypeRef*> types, const std::string& name)
    : TypeRef(Lisple::Form::ANY, name)
    , types(types)
  {
  }

  bool MultiRef::is_type_of(const Lisple::Object& obj) const
  {
    for (const TypeRef* ref : types)
    {
      if (ref->is_type_of(obj))
      {
        return true;
      }
    }
    return false;
  }

  CoercionResult MultiRef::coerce(Context& ctx, sptr_sobject& obj) const
  {
    for (auto type : types)
    {
      CoercionResult result = type->coerce(ctx, obj);
      if (result.success)
      {
        return result;
      }
    }
    return TypeRef::coerce(ctx, obj);
  }

  /* SeqRef */
  SeqRef::SeqRef(const TypeRef* seq_type, const TypeRef* child_type, const std::string& name)
    : TypeRef(Form::ANY, name)
    , seq_type(seq_type)
    , child_type(child_type)
  {
  }

  bool SeqRef::is_type_of(const Lisple::Object& obj) const
  {
    if (!seq_type->is_type_of(obj))
    {
      return false;
    }

    for (auto& child : const_cast<Lisple::Object&>(obj).get_children())
    {
      if (!child_type->is_type_of(*child))
      {
        return false;
      }
    }
    return true;
  }

  CoercionResult SeqRef::coerce(Context& ctx, sptr_sobject& obj) const
  {
    if (seq_type->is_type_of(*obj))
    {
      Lisple::sptr_sobject_v coerced_elements;

      for (auto& child : obj->get_children())
      {
        if (child_type->is_type_of(*child))
        {
          coerced_elements.push_back(child);
        }
        else
        {
          CoercionResult coercion = child_type->coerce(ctx, child);
          if (coercion.success)
          {
            coerced_elements.push_back(coercion.result);
          }
          else
          {
            return coercion;
          }
        }
      }
      sptr_sobject coerced_seq;
      switch (seq_type->form_type)
      {
      case Form::ARRAY:
        coerced_seq = std::make_shared<Array>(coerced_elements);
        break;
      case Form::LIST:
        coerced_seq = std::make_shared<List>(coerced_elements);
        break;
      default:
        return CoercionResult { false, nullptr };
      }

      return CoercionResult { true, coerced_seq };
    }

    return CoercionResult { false, nullptr };
  }

}
