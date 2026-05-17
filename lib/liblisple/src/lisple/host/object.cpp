
#include "lisple/host/object.h"

#include "lisple/runtime/value.h"

namespace Lisple
{
  /*!
   * @brief Empty constant AccessorTable that is spliced in by
   * NATIVE_ADAPTER_IMPL whenever a Native AST::ASTNode Adapter object without accessors
   * is constructed.
   */
  const NAccessorTable NO_N_ACCESSORS = NAccessorTable();

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
  const n_acc_set_t n_no_setter = [](NativeObjectBase*, Context*, sptr_val&)
  {
    throw InvocationException("Property not mutable");
  };

  const HostTypeRef* NativeObjectBase::get_host_type() const
  {
    return get_traits()->type_ref;
  }

  const NAccessorTable& NativeObjectBase::accessor_table() const
  {
    return get_traits()->accessor_table;
  }

  sptr_val NativeObjectBase::get_property(const Value& property) const
  {
    auto* accessors = get_traits()->accessor_table.lookup(property);
    if (nullptr == accessors || nullptr == accessors->getter)
    {
      return Lisple::Constant::NIL;
    }

    return accessors->getter(this);
  }

  bool NativeObjectBase::has_property(const Value& property) const
  {
    auto* accessors = get_traits()->accessor_table.lookup(property);
    return nullptr != accessors && nullptr != accessors->getter;
  }

  void NativeObjectBase::set_property(const Value& property, const sptr_val& value)
  {
    sptr_val v = value;
    set_property(property, v);
  }

  void NativeObjectBase::set_property(const Value& property, sptr_val& value)
  {
    auto* accessors = get_traits()->accessor_table.lookup(property);
    if (nullptr == accessors || nullptr == accessors->setter)
    {
      throw InvocationException("Property '" + property.to_string() + "' not mutable");
    }

    accessors->setter(this, nullptr, value);
  }

  std::string NativeObjectBase::to_string() const
  {
    return Value::map(native_children())->to_string();
  }

  sptr_val_v NativeObjectBase::native_children() const
  {
    sptr_val_v elements;
    for (auto key : this->accessor_table().keys)
    {
      elements.push_back(key);
      elements.push_back(get_property(*key));
    }

    return elements;
  }

  size_t NativeObjectBase::size() const
  {
    return this->accessor_table().keys.size();
  }

  NativeObjectStructuralKind NativeObjectBase::structural_kind() const
  {
    return NativeObjectStructuralKind::MAP;
  }

  bool NativeObjectBase::equals_value(const Value& other) const
  {
    if (other.type == Value::Type::MAP)
    {
      const sptr_val_v& other_elements = other.elements();
      sptr_val_v elements = native_children();
      if (elements.size() != other_elements.size()) return false;
      for (size_t i = 0; i < elements.size(); i++)
      {
        if (!(*elements[i] == *other_elements[i])) return false;
      }
      return true;
    }

    if (other.type == Value::Type::NATIVE_OBJECT)
    {
      sptr_native_obj other_native = other.nobj();
      if (structural_kind() != other_native->structural_kind()) return false;

      sptr_val_v elements = native_children();
      sptr_val_v other_elements = other_native->native_children();
      if (elements.size() != other_elements.size()) return false;
      for (size_t i = 0; i < elements.size(); i++)
      {
        if (!(*elements[i] == *other_elements[i])) return false;
      }
      return true;
    }

    return false;
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

  NAccessorTable::NAccessorTable(
    std::initializer_list<std::pair<std::string, NAccessors>> entries)
  {
    for (auto& [k, a] : entries)
    {
      keys.push_back(Value::keyword(k));
      accessor_map.emplace(k, a);
    }
  }

  bool NAccessorTable::has_key(const AST::ASTNode& key) const
  {
    return accessor_map.count(key.to_string());
  }

  const NAccessors* NAccessorTable::lookup(const Value& key) const
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

  NAccessorTable merge_acc(const NAccessorTable& base, const key_n_acc_map& additional)
  {
    NAccessorTable result;
    result.keys = base.keys;
    result.accessor_map = base.accessor_map;
    for (auto& [k, a] : additional)
    {
      auto [it, inserted] = result.accessor_map.emplace(k->str(), a);
      if (inserted) result.keys.push_back(k);
    }
    return result;
  }

  NAccessorTable merge_acc(const NAccessorTable& base, const NAccessorTable& additional)
  {
    NAccessorTable result;
    result.keys = base.keys;
    result.accessor_map = base.accessor_map;
    for (auto& k : additional.keys)
    {
      auto [it, inserted] =
        result.accessor_map.emplace(k->str(), additional.accessor_map.at(k->str()));
      if (inserted) result.keys.push_back(k);
    }
    return result;
  }

} // namespace Lisple
