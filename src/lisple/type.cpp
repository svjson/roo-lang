
#include "lisple/type.h"

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /* TypeRef */
  TypeRef::TypeRef(RTValue::Type value_type, Form form_type, const std::string& name)
    : value_type(value_type)
    , form_type(form_type)
    , name(name)
  {
  }

  bool TypeRef::is_type_of(const RTValue& val) const
  {
    if (val == *Constant::NIL) return true;
    return val.type == value_type;
  }

  bool TypeRef::is_type_of(const Object& obj) const
  {
    if (obj == *NIL) return true;
    return obj.get_type() == form_type;
  }

  CoercionResult<Object> TypeRef::coerce(Context&, sptr_sobject&) const
  {
    return CoercionResult<Object>{false, nullptr};
  }

  CoercionResult<RTValue> TypeRef::coerce(Context&, sptr_rtval&) const
  {
    return CoercionResult<RTValue>{false, nullptr};
  }

  CoercionResult<Object> TypeRef::coerce(Runtime& reader, sptr_sobject& obj) const
  {
    Context ctx(reader);
    return this->coerce(ctx, obj);
  }

  bool TypeRef::is_host_object() const
  {
    return false;
  }

  const std::string& TypeRef::to_string() const
  {
    return name;
  }

  /* AnyRef */
  AnyRef::AnyRef()
    : TypeRef(RTValue::Type::ANY, Form::ANY, "<any>")
  {
  }

  bool AnyRef::is_type_of(const RTValue&) const
  {
    return true;
  }

  bool AnyRef::is_type_of(const Object&) const
  {
    return true;
  }

  /* MultiRef */
  MultiRef::MultiRef(std::vector<const TypeRef*> types, const std::string& name)
    : TypeRef(RTValue::Type::ANY, Form::ANY, name)
    , types(types)
  {
  }

  bool MultiRef::is_type_of(const RTValue& val) const
  {
    for (const TypeRef* ref : types)
    {
      if (ref->is_type_of(val))
      {
        return true;
      }
    }
    return false;
  }

  bool MultiRef::is_type_of(const Object& obj) const
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

  CoercionResult<Object> MultiRef::coerce(Context& ctx, sptr_sobject& obj) const
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

  CoercionResult<RTValue> MultiRef::coerce(Context& ctx, sptr_rtval& obj) const
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
    : TypeRef(RTValue::Type::ANY, Form::ANY, name)
    , seq_type(seq_type)
    , child_type(child_type)
  {
  }

  bool SeqRef::is_type_of(const RTValue& val) const
  {
    if (!seq_type->is_type_of(val))
    {
      return false;
    }

    if (auto* children = std::get_if<sptr_rtval_v>(&val.value))
    {
      for (auto& child : *children)
      {
        if (!child_type->is_type_of(*child))
        {
          return false;
        }
      }
      return true;
    }

    return false;
  }

  bool SeqRef::is_type_of(const Object& obj) const
  {
    if (!seq_type->is_type_of(obj))
    {
      return false;
    }

    for (auto& child : const_cast<Object&>(obj).get_children())
    {
      if (!child_type->is_type_of(*child))
      {
        return false;
      }
    }
    return true;
  }

  CoercionResult<Object> SeqRef::coerce(Context& ctx, sptr_sobject& obj) const
  {
    if (seq_type->is_type_of(*obj))
    {
      sptr_sobject_v coerced_elements;

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
        return CoercionResult<Object>{false, nullptr};
      }

      return CoercionResult{true, coerced_seq};
    }

    return CoercionResult<Object>{false, nullptr};
  }

  CoercionResult<RTValue> SeqRef::coerce(Context& ctx, sptr_rtval& obj) const
  {
    if (seq_type->is_type_of(*obj))
    {
      sptr_rtval_v coerced_elements;

      for (auto& child : Lisple::get_children(*obj))
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
      sptr_rtval coerced_seq;
      switch (seq_type->form_type)
      {
      case Form::ARRAY:
        coerced_seq = RTValue::vector(std::move(coerced_elements));
        break;
      case Form::LIST:
        coerced_seq = RTValue::list(std::move(coerced_elements));
        break;
      default:
        return CoercionResult<RTValue>{false, nullptr};
      }

      return CoercionResult{true, coerced_seq};
    }

    return CoercionResult<RTValue>{false, nullptr};
  }

} // namespace Lisple
