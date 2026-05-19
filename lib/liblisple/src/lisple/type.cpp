
#include "lisple/type.h"

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/host/object.h>
#include <lisple/runtime/seq.h>

namespace Lisple
{
  /* TypeRef */
  TypeRef::TypeRef(Value::Type value_type, Form form_type, const std::string& name)
    : value_type(value_type)
    , form_type(form_type)
    , name(name)
  {
  }

  bool TypeRef::is_type_of(const Value& val) const
  {
    if (val == *Constant::NIL) return true;
    if (form_type == Form::HOST_SEQ)
    {
      if (val.type == Value::Type::NATIVE_OBJECT)
      {
        return val.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR;
      }
      if (val.type == Value::Type::OBJECT)
      {
        return val.obj()->get_type() == Form::HOST_SEQ;
      }
      return false;
    }
    return val.type == value_type;
  }

  bool TypeRef::is_type_of(const AST::ASTNode& obj) const
  {
    if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
    {
      return is_type_of(*wrapper->val);
    }
    if (obj == *AST::NIL) return true;
    return obj.get_type() == form_type;
  }

  CoercionResult TypeRef::coerce(Context&, sptr_val&) const
  {
    return CoercionResult{false, nullptr};
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
    : TypeRef(Value::Type::ANY, Form::ANY, "<any>")
  {
  }

  bool AnyRef::is_type_of(const Value&) const
  {
    return true;
  }

  bool AnyRef::is_type_of(const AST::ASTNode&) const
  {
    return true;
  }

  /* MultiRef */
  MultiRef::MultiRef(std::vector<const TypeRef*> types, const std::string& name)
    : TypeRef(Value::Type::ANY, Form::ANY, name)
    , types(types)
  {
  }

  bool MultiRef::is_type_of(const Value& val) const
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

  bool MultiRef::is_type_of(const AST::ASTNode& obj) const
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

  CoercionResult MultiRef::coerce(Context& ctx, sptr_val& obj) const
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
    : TypeRef(Value::Type::ANY, Form::ANY, name)
    , seq_type(seq_type)
    , child_type(child_type)
  {
  }

  bool SeqRef::is_type_of(const Value& val) const
  {
    if (!seq_type->is_type_of(val))
    {
      return false;
    }

    if (auto* children = std::get_if<sptr_val_v>(&val.value))
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

  bool SeqRef::is_type_of(const AST::ASTNode& obj) const
  {
    if (!seq_type->is_type_of(obj))
    {
      return false;
    }

    for (auto& child : const_cast<AST::ASTNode&>(obj).get_children())
    {
      if (!child_type->is_type_of(*child))
      {
        return false;
      }
    }
    return true;
  }

  CoercionResult SeqRef::coerce(Context& ctx, sptr_val& obj) const
  {
    if (seq_type->is_type_of(*obj))
    {
      sptr_val_v coerced_elements;

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
      sptr_val coerced_seq;
      switch (seq_type->form_type)
      {
      case Form::VECTOR:
        coerced_seq = Value::vector(std::move(coerced_elements));
        break;
      case Form::LIST:
        coerced_seq = Value::list(std::move(coerced_elements));
        break;
      default:
        return CoercionResult{false, nullptr};
      }

      return CoercionResult{true, coerced_seq};
    }

    return CoercionResult{false, nullptr};
  }

} // namespace Lisple
