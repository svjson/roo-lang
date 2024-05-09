
#include "type.h"

#include "form.h"

namespace Lisple
{
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

  const std::string& TypeRef::to_string() const
  {
    return name;
  }

  AnyRef::AnyRef()
    : TypeRef(Lisple::Form::ANY, "<any>")
  {
  }

  bool AnyRef::is_type_of(const Lisple::Object&) const
  {
    return true;
  }

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
}
