
#ifndef __NAMESPACE_H_
#define __NAMESPACE_H_

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <lisple/form.h>
#include <lisple/scope.h>
#include <lisple/type.h>

namespace Lisple
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
    Namespace(Type type, const std::string& name, std::map<std::string, sptr_sobject> lang);
    Namespace(Type type,
              const std::string& name,
              std::map<std::string, sptr_sobject> lang,
              std::map<std::string, sptr_rtval> lang_symbols);

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
    sptr_sobject find(const Word& identifier) const;

    /*!
     * @brief Used internally to service Namespace::lookup_symbol
     */
    sptr_rtval find_symbol(const Word& identifier) const;

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
    bool has(const Word& identifier) const override;

    /*!
     * @brief Find a stored identifier in this namespace, its imported
     * namespaces or, if specifically qualified, its aliased namespaces.
     */
    sptr_sobject lookup(const Word& identifier) const override;

    sptr_rtval lookup_symbol(const Word& identifier) const;

    using Scope::mutate;
    void mutate(const Word& identifier, const sptr_sobject& obj) override;
    void mutate(const Word& identifier, const sptr_rtval& val);

    /*
     * Creates a language namespace. Intented for internal use only.
     */
    static Namespace make_lang(std::map<std::string, sptr_sobject> lang,
                               std::map<std::string, sptr_rtval> lang_symbols);

    friend class Runtime;
  };
} // namespace Lisple

#endif
