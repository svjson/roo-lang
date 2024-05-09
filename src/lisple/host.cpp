
#include "host.h"

#include "form.h"
#include "type.h"
#include "lisple_exception.h"

namespace Lisple
{
  /**
   * Empty constant AccessorLookup that is spliced in by HOST_ADAPTER_IMPL
   * whenever a Host Object Adapter object without accessors is constructed.
   */
  const AccessorLookup NO_ACCESSORS = AccessorLookup();

  /**
   * Default function that is spliced in by accessor macros when a property is
   * defined without a getter.
   */
  const acc_get_t no_getter = [](const AbstractHostObject*) { return Lisple::NIL; };

  /**
   * Default function that is sliced in by accessor macros when a property is
   * defined without a setter. Invariably raises an exception, as it should
   * never be invoked.
   */
  const acc_set_t no_setter = [](AbstractHostObject*, Context*, Object&)
  {
    throw Lisple::InvocationException("Property not mutable");
  };

  /**
   * HostTypeRef implementation
   */
  HostTypeRef::HostTypeRef(Lisple::HostObjectType host_type, const std::string& name)
    : Lisple::TypeRef(Lisple::Form::HOST_OBJECT, name)
    , host_type(host_type)
  {
  }

  bool HostTypeRef::is_type_of(const Lisple::Object& obj) const
  {
    if (TypeRef::is_type_of(obj))
    {
      return obj.as<Lisple::AbstractHostObject>().get_host_type() == host_type;
    }
    return false;
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
  AbstractHostObject::AbstractHostObject(Lisple::HostObjectType type)
    : Lisple::Object(Lisple::Form::HOST_OBJECT)
    , host_type(type)
  {
  }

  AbstractHostObject::AbstractHostObject(Lisple::HostObjectType type, const AccessorLookup& accessors)
    : Lisple::Object(Lisple::Form::HOST_OBJECT)
    , host_type(type)
    , accessors(accessors)
  {
  }

  std::string AbstractHostObject::to_string() const
  {
    return "<host-object>";
  }

  Lisple::HostObjectType AbstractHostObject::get_host_type() const
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

  Lisple::sptr_sobject AbstractHostObject::get_sptr_property(const Object& key) const
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
    throw Lisple::InvocationException("No such property: " + key.to_string());
  }

  bool AbstractHostObject::operator==(const Lisple::Object& other) const
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
