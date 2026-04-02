#ifndef LISPLE__HOST__TYPE_H
#define LISPLE__HOST__TYPE_H

#include <optional>

#include <lisple/exception.h>
#include <lisple/type.h>

// clang-format off

/*
 * __HOST_TYPE
 * __HOST_TYPE__NO_MAKE_FN
 * HOST_TYPE
 *
 * Macro short-hand for defining HostTypeRef constants.
 */
#define __HOST_TYPE(CONST, NAME, MAKE_FN) \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME, MAKE_FN);

#define __HOST_TYPE__NO_MAKE_FN(CONST, NAME)                            \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter             \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST = Lisple::HostTypeRef(NAME);

#define HOST_TYPE(CONST, NAME, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_TYPE, __HOST_TYPE__NO_MAKE_FN)(CONST, NAME, ##__VA_ARGS__)

/*
 * __HOST_SUB_TYPE
 * __HOST_SUB_TYPE__NO_MAKE_FN
 * HOST_SUB_TYPE
 *
 * Macro short-hand for defining HostTypeRef constants derived from
 * other HostTypeRef constants.
 */
#define __HOST_SUB_TYPE(CONST, NAME, PARENT_TYPE, MAKE_FN)              \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter that is a   \
   * subtype of another Adapter, and with a registered make-function for\
   * automatic coercion.                                                \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST =                              \
    Lisple::HostTypeRef(NAME, PARENT_TYPE, MAKE_FN);

#define __HOST_SUB_TYPE__NO_MAKE_FN(CONST, NAME, PARENT_TYPE)           \
  /*!                                                                   \
   * @brief Type definition for a Lisple HostObject Adapter that is a   \
   * subtype of another Adapter.                                        \
   */                                                                   \
  inline const Lisple::HostTypeRef CONST =                              \
    Lisple::HostTypeRef(NAME, PARENT_TYPE);

#define HOST_SUB_TYPE(CONST, NAME, PARENT_TYPE, ...) __SELECT_MACRO__2(0, ##__VA_ARGS__, __HOST_SUB_TYPE, __HOST_SUB_TYPE__NO_MAKE_FN)(CONST, NAME, PARENT_TYPE, ##__VA_ARGS__)

// clang-format on

namespace Lisple
{
  /*!
   * @brief TypeRef implementation specialized for HostObject adapters
   *
   * This includes both base types and derived types. In order for type testing
   * to work properly, it is recommended that derived types specify the @ref
   * parent_type.
   *
   * Optionally contains a qualified name of a make-function, assisting in
   * coercion from raw lisple data structures to object instances.
   */
  class HostTypeRef : public TypeRef
  {

   public:
    /*!
     * @brief Optional parent type for derived types
     */
    std::optional<const HostTypeRef*> parent_type = std::nullopt;
    /*!
     * @brief Optional reference to make-function
     */
    std::optional<std::string> make_fn = std::nullopt;

    /*!
     * @brief Constructs a type reference with an optional make function name
     */
    HostTypeRef(const std::string& name,
                const std::optional<std::string>& make_fn = std::nullopt);

    /*!
     * @brief Constructs a type reference for a derived type and with an
     * optional make function name.
     */
    HostTypeRef(const std::string& name,
                const HostTypeRef* parent_type,
                const std::optional<std::string>& make_fn = std::nullopt);

    /*!
     * @brief Tests if a value is an instance of this type.
     *
     * @return true if the value refers to an object of this concrete
     * type or if the type if a base of the concrete type of the
     * object.
     */
    bool is_type_of(const RTValue& obj) const override;

    /*!
     * @brief Tests if an object is an instance of this type.
     *
     * @return true if the object is of this concrete type or if the type if a
     * base of the concrete type of the object.
     */
    bool is_type_of(const Object& obj) const override;

    /*!
     * @brief Attempts to coerce a Lisple Object of a different type to this
     * this type by using the make function provided by the type.
     */
    CoercionResult<Object> coerce(Context& ctx, sptr_sobject& obj) const override;

    /*!
     * @brief Attempts to coerce a Lisple Runtime Value of a different type to this
     * this type by using the make function provided by the type.
     */
    CoercionResult<RTValue> coerce(Context& ctx, sptr_rtval& val) const override;

    /*!
     * @brief Tests if this type is a HostObject type.
     *
     * @return true
     */
    bool is_host_object() const override;
  };

  /*!
   * @brief Abstract base class for wrapping the object held by a Host Object
   * Adapter.
   */
  template <class T> class ValueHolder
  {
   public:
    virtual ~ValueHolder() = default;

    virtual T& get_object() = 0;
    virtual std::unique_ptr<T>& get_object_ptr() = 0;
  };

  /*!
   * @brief ValueHolder-implementation that owns the Host Object instance,
   * containing it in a std::unique_ptr
   */
  template <class T> class HostObjectValue : public ValueHolder<T>
  {
    std::unique_ptr<T> object;

   public:
    HostObjectValue(std::unique_ptr<T>& object)
      : object(std::move(object))
    {
    }
    T& get_object() override { return *object; }
    std::unique_ptr<T>& get_object_ptr() override { return object; }
  };

  /*!
   * @brief ValueHolder-implementation that holds a reference to the Host Object
   * for cases where ownership cannot be given to the Adapter.
   */
  template <class T> class HostObjectRef : public ValueHolder<T>
  {
    T& object_reference;

   public:
    HostObjectRef(T& ref)
      : object_reference(ref)
    {
    }
    T& get_object() override { return object_reference; }
    std::unique_ptr<T>& get_object_ptr() override
    {
      throw InvocationException("Cannot give up ownership of Object Reference");
    }
  };

} // namespace Lisple

#endif /* LISPLE__HOST__TYPE_H */
