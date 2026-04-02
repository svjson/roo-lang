
#include "lisple/host.h"

#include <lisple/context.h>
#include <lisple/exception.h>
#include <lisple/exec.h>
#include <lisple/form.h>
#include <lisple/type.h>

namespace Lisple
{
  /*!
   * @brief Empty constant AccessorTable that is spliced in by
   * HOST_ADAPTER_IMPL whenever a Host Object Adapter object without accessors
   * is constructed.
   */
  const AccessorTable NO_ACCESSORS = AccessorTable();

  /*!
   * @brief Default function that is spliced in by accessor macros when a property
   * is defined without a getter.
   */
  const acc_get_t no_getter = [](const AbstractHostObject*)
  {
    return NIL;
  };

  /*!
   * @brief Default function that is sliced in by accessor macros when a
   * property is defined without a setter. Invariably raises an exception, as it
   * should never be invoked.
   */
  const acc_set_t no_setter = [](AbstractHostObject*, Context*, Object&)
  {
    throw InvocationException("Property not mutable");
  };

  AdapterTraits::AdapterTraits(const HostTypeRef* type_ref,
                               const AccessorTable& accessor_table)
    : type_ref(type_ref)
    , accessor_table(accessor_table)
  {
  }

  /**
   * AccessorTable implementation
   */
  AccessorTable::AccessorTable(const key_acc_map& acc_map)
  {
    for (auto& [k, a] : acc_map)
    {
      keys.push_back(k);
      accessor_map.emplace(k->to_string(), a);
    }
  }

  bool AccessorTable::has_key(const Object& key) const
  {
    return accessor_map.count(key.to_string());
  }

  const Accessors& AccessorTable::lookup(const Object& key) const
  {
    return accessor_map.at(key.to_string());
  }

  /**
   * AbstractHostObject base implementation for all Host Object Adapters
   */
  AbstractHostObject::AbstractHostObject(Form form)
    : Seq(form)
  {
  }

  const HostTypeRef* AbstractHostObject::get_host_type() const
  {
    return get_traits()->type_ref;
  }

  const AccessorTable& AbstractHostObject::accessor_table() const
  {
    return get_traits()->accessor_table;
  }

  const std::vector<std::shared_ptr<Object>> AbstractHostObject::keys() const
  {
    return accessor_table().keys;
  }

  bool AbstractHostObject::has_key(const Object& key) const
  {
    return accessor_table().has_key(key);
  }

  sptr_sobject AbstractHostObject::get_sptr_property(const Object& key) const
  {
    auto& acc = accessor_table();
    if (acc.has_key(key))
    {
      return acc.lookup(key).getter(this);
    }
    return NIL;
  }

  void AbstractHostObject::set_property(const Object& key, sptr_sobject& value)
  {
    this->set_property(nullptr, key, value);
  }

  void AbstractHostObject::set_property(Context* ctx, const Object& key, sptr_sobject& value)
  {
    auto& acc = accessor_table();
    if (acc.has_key(key))
    {
      acc.lookup(key).setter(this, ctx, *value);
      return;
    }
    throw InvocationException("No such property: " + key.to_string());
  }

  bool AbstractHostObject::operator==(const Object& other) const
  {
    return this == &other;
  }

  sptr_sobject_v& AbstractHostObject::get_children()
  {
    sync_children();
    return children;
  }

  std::string AbstractHostObject::to_string(int depth) const
  {
    sync_children();
    return Seq::to_string(depth);
  }

  const std::string AbstractHostObject::lpar() const
  {
    return "{";
  }

  const std::string AbstractHostObject::rpar() const
  {
    return "}";
  }

  Accessors::Accessors(const acc_get_t& getter, const acc_set_t& setter)
    : getter(getter)
    , setter(setter)
  {
  }

  AccessorTable merge_acc(const AccessorTable& al1, const key_acc_map& kam2)
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

    return AccessorTable(merged_map);
  }

} // namespace Lisple
