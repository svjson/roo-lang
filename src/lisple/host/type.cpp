
#include "lisple/host/type.h"

#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/host/object.h>

namespace Lisple
{
  /**
   * HostTypeRef implementation
   */
  HostTypeRef::HostTypeRef(const std::string& name,
                           const std::optional<std::string>& make_fn)
    : TypeRef(RTValue::Type::OBJECT, Form::HOST_OBJECT, name)
    , make_fn(make_fn)
  {
  }

  HostTypeRef::HostTypeRef(const std::string& name,
                           const HostTypeRef* parent_type,
                           const std::optional<std::string>& make_fn)
    : TypeRef(RTValue::Type::OBJECT, Form::HOST_OBJECT, name)
    , parent_type(parent_type)
    , make_fn(make_fn)
  {
  }

  bool HostTypeRef::is_type_of(const RTValue& val) const
  {
    if (val.type == RTValue::Type::OBJECT)
    {
      return this->is_type_of(*std::get<sptr_sobject>(val.value));
    }

    if (val.type == RTValue::Type::NATIVE_OBJECT)
    {
      const HostTypeRef* obj_type = val.nobj()->get_host_type();
      return obj_type == this || (obj_type->parent_type && (*obj_type->parent_type) == this);
    }

    return false;
  }

  bool HostTypeRef::is_type_of(const Object& obj) const
  {
    if (auto* wrapper = dynamic_cast<const RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == RTValue::Type::NATIVE_OBJECT)
      {
        return is_type_of(*wrapper->val);
      }
      return is_type_of(*wrapper->val);
    }

    return false;
  }

  CoercionResult HostTypeRef::coerce(Context& ctx, sptr_rtval& obj) const
  {
    if (make_fn)
    {
      sptr_rtval function = ctx.lookup(*make_fn);

      if (*function == *Constant::NIL || !Type::EXEC.is_type_of(*function))
      {
        throw InvocationException(
          "Coercion failed. Review Host Object configuration - Make Function '" + *make_fn +
          "' is not executable: " + function->to_string());
      }
      auto& make_exec = std::get<sptr_sobject>(function->value)->as<Executable>();

      for (auto& sig : make_exec.signatures)
      {
        if (sig->get_arguments().size() != 1) continue;

        if (sig->get_arguments().front().matches(*obj))
        {
          sptr_rtval_v arg_list{obj};
          try
          {
            return CoercionResult{true, sig->invoke(ctx, arg_list)};
          }
          catch (const LispleException& e)
          {
            // Ignore
          }
        }
      }
    }

    return CoercionResult{false, nullptr};
  }

  bool HostTypeRef::is_host_object() const
  {
    return true;
  }

} // namespace Lisple
