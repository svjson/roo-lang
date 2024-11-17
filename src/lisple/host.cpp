
#include "host.h"

#include "exec.h"
#include "form.h"
#include "type.h"
#include "context.h"
#include "exception.h"

namespace Lisple
{
  /*!
   * @brief Empty constant AccessorLookup that is spliced in by
   * HOST_ADAPTER_IMPL whenever a Host Object Adapter object without accessors
   * is constructed.
   */
  const AccessorLookup NO_ACCESSORS = AccessorLookup();

  /*!
   * @brief Default function that is spliced in by accessor macros when a property
   * is defined without a getter.
   */
  const acc_get_t no_getter = [](const AbstractHostObject*) { return NIL; };

  /*!
   * @brief Default function that is sliced in by accessor macros when a
   * property is defined without a setter. Invariably raises an exception, as it
   * should never be invoked.
   */
  const acc_set_t no_setter = [](AbstractHostObject*, Context*, Object&)
  {
    throw InvocationException("Property not mutable");
  };

  /**
   * HostTypeRef implementation
   */
  HostTypeRef::HostTypeRef(const std::string& name, const std::optional<std::string>& make_fn)
    : TypeRef(Form::HOST_OBJECT, name)
    , make_fn(make_fn)
  {
  }

  bool HostTypeRef::is_type_of(const Object& obj) const
  {
    if (*NIL != obj && TypeRef::is_type_of(obj))
    {
      return obj.as<AbstractHostObject>().get_host_type() == this;
    }
    return false;
  }

  CoercionResult HostTypeRef::coerce(Context& ctx, sptr_sobject& obj) const
  {
    if (make_fn)
    {
      sptr_sobject function = ctx.lookup(*make_fn);
      if (*function == *NIL || !Type::EXEC.is_type_of(*function))
      {
        throw InvocationException("Coercion failed. Review Host Object configuration - Make Function '" + *make_fn + "' is not executable: " + function->to_string(2));
      }
      auto& make_exec = function->as<Executable>();

      for (auto& sig : make_exec.signatures)
      {
        if (sig->get_arguments().size() != 1) continue;

        if (sig->get_arguments().front().matches(*obj))
        {
          sptr_sobject_v arg_list { obj };
          return CoercionResult { true, sig->invoke(ctx, arg_list) };
        }
      }

    }
    return CoercionResult { false, nullptr };
  }

  bool HostTypeRef::is_host_object() const
  {
    return true;
  }

  /**
   * AccessorLookup implementation
   */
  AccessorLookup::AccessorLookup()
  {
  }

  AccessorLookup::AccessorLookup(const key_acc_map& acc_map)
  {
    for (auto& [k, a] : acc_map)
    {
      keys.push_back(k);
      accessor_map.emplace(k->to_string(), a);
    }
  }

  bool AccessorLookup::has_key(const Object& key) const
  {
    return accessor_map.count(key.to_string());
  }

  const Accessors& AccessorLookup::lookup(const Object& key) const
  {
    return accessor_map.at(key.to_string());
  }

  /**
   * AbstractHostObject base implementation for all Host Object Adapters
   */
  AbstractHostObject::AbstractHostObject(const HostTypeRef* type)
    : Object(Form::HOST_OBJECT)
    , host_type(type)
  {
  }

  AbstractHostObject::AbstractHostObject(const HostTypeRef* type, const AccessorLookup& accessors)
    : Object(Form::HOST_OBJECT)
    , host_type(type)
    , accessors(accessors)
  {
  }

  std::string AbstractHostObject::to_string(int) const
  {
    return "<host-object>";
  }

  const HostTypeRef* AbstractHostObject::get_host_type() const
  {
    return host_type;
  }

  const std::vector<std::shared_ptr<Object>> AbstractHostObject::keys() const
  {
    return accessors.keys;
  }

  bool AbstractHostObject::has_key(const Object& key) const
  {
    return accessors.has_key(key);
  }

  sptr_sobject AbstractHostObject::get_sptr_property(const Object& key) const
  {
    if (accessors.has_key(key))
    {
      return accessors.lookup(key).getter(this);
    }
    return NIL;
  }

  void AbstractHostObject::set_property(const Object& key, sptr_sobject& value)
  {
    this->set_property(nullptr, key, value);
  }

  void AbstractHostObject::set_property(Context* ctx, const Object& key, sptr_sobject& value)
  {
    if (accessors.has_key(key))
    {
      accessors.lookup(key).setter(this, ctx, *value);
      return;
    }
    throw InvocationException("No such property: " + key.to_string());
  }

  bool AbstractHostObject::operator==(const Object& other) const
  {
    return this == &other;
  }

  Accessors::Accessors(const acc_get_t& getter,
                       const acc_set_t& setter)
    : getter(getter)
    , setter(setter)
  {
  }

  AccessorLookup merge_acc(const AccessorLookup& al1, const key_acc_map& kam2)
  {
    key_acc_map merged_map;
    for (auto& key : al1.keys)
    {
      merged_map.emplace(key, al1.lookup(*key));
    }

    for (auto& [key, acc] : kam2)
    {
      merged_map.emplace(key, acc);
    }

    return AccessorLookup(merged_map);
  }

}
