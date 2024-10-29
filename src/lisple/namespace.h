
#ifndef __NAMESPACE_H_
#define __NAMESPACE_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "form.h"
#include "scope.h"
#include "type.h"

namespace Lisple
{
  class Namespace : public Lisple::Scope
  {
   public:
    enum class Type : uint8_t
    {
      LANG = 0x00,
      USER = 0x01
    };

   private:
    const Type type;
    const std::string name = "";

    /**
     * Fully imported namespaces, ie through (:require somenamespace)
     */
    std::vector<Namespace*> imported_namespaces;

    /**
     * Locally aliased namespaces, ie through (:require [somenamespace :as sn])
     */
    std::map<const std::string, Namespace*> aliased_namespaces;

    Namespace(Type type, const std::string& name);
    Namespace(Type type, const std::string& name, std::map<std::string, Lisple::sptr_sobject> lang);

    /**
     * \brief Fully import another namespaces
     */
    void import_full(Namespace& ns);

    /**
     * \brief Import an aliased namespace
     */
    void import_aliased(Namespace& ns, const std::string& alias);

    /**
     * Used internally to service Namspace::has and Namespace::lookup
     */
    sptr_sobject find(const Word& identifier) const;

   public:
    /**
     * Create a new user namespace with a specific name.
     *
     * @throws NamespaceException if the name doesn't conform to naming rules.
     */
    Namespace(const std::string& name);

    const std::string& get_name() const;
    Type get_type() const;
    bool empty() const;

    /**
     * Query if an identifier is stored in this namespace, its imported
     * namespaces or, if specifically qualified, its aliased namespaces.
     */
    bool has(const Lisple::Word& identifier) const override;

    /**
     * Find a stored identifier in this namespace, its imported namespaces
     * or, if specifically qualified, its aliased namespaces.
     */
    sptr_sobject lookup(const Lisple::Word& identifier) const override;

    void mutate(const Lisple::Word& identifier, const Lisple::sptr_sobject& obj) override;

    /*
     * Creates a language namespace. Intented for internal use only.
     */
    static Namespace make_lang(std::map<std::string, Lisple::sptr_sobject> lang);

    friend class LispReader;
  };
}

#endif
