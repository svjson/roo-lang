
#ifndef __NAMESPACE_H_
#define __NAMESPACE_H_

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <roo/form.h>
#include <roo/scope.h>
#include <roo/type.h>

namespace Roo
{
  class Namespace : public Scope
  {
   public:
    enum class Type : uint8_t
    {
      LANG = 0x00,
      USER = 0x01
    };

    struct Origin
    {
      enum class Type : uint8_t
      {
        NATIVE = 0x00,
        FILE = 0x01
      };

      Type type = Type::NATIVE;
      std::optional<std::string> source_path;

      static Origin native() { return {}; }
      static Origin file(const std::string& path) { return {Type::FILE, path}; }
    };

   private:
    const Type type;
    const std::string name = "";
    Origin origin;

    /*!
     * @brief Fully imported namespaces, ie through (:require somenamespace)
     */
    std::vector<Namespace*> imported_namespaces;

    /*!
     * @brief Locally aliased namespaces, ie through
     * (:require [somenamespace :as sn])
     */
    std::map<const std::string, Namespace*> aliased_namespaces;

    Namespace(Type type, const std::string& name);
    Namespace(Type type,
              const std::string& name,
              std::map<std::string, sptr_val> lang_symbols);

    /*!
     * @brief Fully import another namespaces
     */
    void import_full(Namespace& ns);

    /*!
     * @brief Import an aliased namespace
     */
    void import_aliased(Namespace& ns, const std::string& alias);

    /*!
     * @brief Used internally to service Namespace::has and Namespace::lookup
     */
    const sptr_val* find_local(const std::string& identifier) const;

   public:
    /*!
     * @brief Create a new user namespace with a specific name.
     *
     * @throws NamespaceException if the name doesn't conform to naming rules.
     */
    Namespace(const std::string& name);

    const std::string& get_name() const;
    Type get_type() const;
    bool empty() const;

    const Origin& get_origin() const { return origin; }
    void set_origin(Origin o) { origin = std::move(o); }

    /*!
     * @brief Tests if an identifier is stored in this namespace, its imported
     * namespaces or, if specifically qualified, its aliased namespaces.
     */
    bool has(const std::string& identifier) const override;

    /*!
     * @brief Find a stored identifier in this namespace, its imported
     * namespaces or, if specifically qualified, its aliased namespaces.
     */
    const sptr_val* find(const std::string& identifier) const override;
    const sptr_val* find(const Value& identifier) const override;
    const sptr_val& lookup(const std::string& identifier) const override;
    const sptr_val& lookup(const Value& identifier) const override;

    void store(const std::string& symbol, const sptr_val& value);

    using Scope::mutate;
    void mutate(const std::string& identifier, const sptr_val& val) override;

    /*
     * Creates a language namespace. Intented for internal use only.
     */
    static Namespace make_lang(std::map<std::string, sptr_val> lang_symbols);
    static Namespace make_lang(const std::string& name,
                               std::map<std::string, sptr_val> lang_symbols);

    friend class Runtime;
  };
} // namespace Roo

#endif
