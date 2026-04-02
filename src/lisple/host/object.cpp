
#include "lisple/host/object.h"

namespace Lisple
{
  /*!
   * @brief Empty constant AccessorTable that is spliced in by
   * HOST_ADAPTER_IMPL whenever a Host Object Adapter object without accessors
   * is constructed.
   */
  const NAccessorTable NO_ACCESSORS = NAccessorTable();

  /*!
   * @brief Default function that is spliced in by accessor macros when a property
   * is defined without a getter.
   */
  const n_acc_get_t n_no_getter = [](const NativeObjectBase*)
  {
    return Lisple::Constant::NIL;
  };

  /*!
   * @brief Default function that is sliced in by accessor macros when a
   * property is defined without a setter. Invariably raises an exception, as it
   * should never be invoked.
   */
  const n_acc_set_t n_no_setter = [](NativeObjectBase*, Context*, sptr_rtval&)
  {
    throw InvocationException("Property not mutable");
  };

  const HostTypeRef* NativeObjectBase::get_host_type() const
  {
    return get_traits()->type_ref;
  }

  sptr_rtval NativeObjectBase::get_property(const RTValue& property) const
  {
    auto* accessors = get_traits()->accessor_table.lookup(property);
    if (nullptr == accessors || nullptr == accessors->getter)
    {
      return Lisple::Constant::NIL;
    }

    return accessors->getter(this);
  }

  /**
   * NAccessors implementation
   */
  NAccessors::NAccessors(const n_acc_get_t& getter, const n_acc_set_t& setter)
    : getter(getter)
    , setter(setter)
  {
  }

  /**
   * AccessorTable implementation
   */
  NAccessorTable::NAccessorTable(const key_n_acc_map& acc_map)
  {
    for (auto& [k, a] : acc_map)
    {
      keys.push_back(k);
      accessor_map.emplace(k->str(), a);
    }
  }

  bool NAccessorTable::has_key(const Object& key) const
  {
    return accessor_map.count(key.to_string());
  }

  const NAccessors* NAccessorTable::lookup(const RTValue& key) const
  {
    auto it = accessor_map.find(key.str());
    if (it != accessor_map.end())
    {
      return &it->second;
    }

    return nullptr;
  }

  NativeObjectTraits::NativeObjectTraits(const HostTypeRef* type_ref,
                                         const NAccessorTable& accessor_table)
    : type_ref(type_ref)
    , accessor_table(accessor_table)
  {
  }

} // namespace Lisple
