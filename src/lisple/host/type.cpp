
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
    : TypeRef(Value::Type::OBJECT, Form::HOST_OBJECT, name)
    , make_fn(make_fn)
  {
  }

  HostTypeRef::HostTypeRef(const std::string& name,
                           const HostTypeRef* parent_type,
                           const std::optional<std::string>& make_fn)
    : TypeRef(Value::Type::OBJECT, Form::HOST_OBJECT, name)
    , parent_type(parent_type)
    , make_fn(make_fn)
  {
  }

  bool HostTypeRef::is_type_of(const Value& val) const
  {
    if (val.type == Value::Type::OBJECT)
    {
      return this->is_type_of(*std::get<sptr_sobject>(val.value));
    }

    if (val.type == Value::Type::NATIVE_OBJECT)
    {
      const HostTypeRef* obj_type = val.nobj()->get_host_type();
      return obj_type == this || (obj_type->parent_type && (*obj_type->parent_type) == this);
    }

    return false;
  }

  bool HostTypeRef::is_type_of(const AST::ASTNode& obj) const
  {
    if (auto* wrapper = dynamic_cast<const AST::RuntimeValueWrapper*>(&obj))
    {
      if (wrapper->val->type == Value::Type::NATIVE_OBJECT)
      {
        return is_type_of(*wrapper->val);
      }
      return is_type_of(*wrapper->val);
    }

    return false;
  }

  CoercionResult HostTypeRef::coerce(Context& ctx, sptr_val& obj) const
  {
    if (make_fn)
    {
      sptr_val function = ctx.lookup(*make_fn);

      if (*function == *Constant::NIL || !Type::EXEC.is_type_of(*function))
      {
        throw InvocationException(
          "Coercion failed. Review Host AST::ASTNode configuration - Make Function '" +
          *make_fn + "' is not executable: " + function->to_string());
      }
      auto& make_exec = function->exec();

      for (auto& sig : make_exec.signatures)
      {
        if (sig->get_arguments().size() != 1) continue;

        if (sig->get_arguments().front().matches(*obj))
        {
          sptr_val_v arg_list{obj};
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
