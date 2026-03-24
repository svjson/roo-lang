
#ifndef __RUNTIME_H_
#define __RUNTIME_H_

#include "namespace.h"
#include "reader.h"
#include "type.h"
#include <map>
#include <string>

namespace Lisple
{
  class Array;
  class Context;
  class List;
  class Map;
  class Seq;
  class Word;
  class FileSystem;

  class Runtime
  {
    /*!
     * @brief The Lisple language namespace, containing all language built-in
     * functions and identifiers
     */
    Namespace lang;
    FileSystem* fs;
    Reader sexp_reader;

    std::map<const std::string, Namespace> namespaces;

    Namespace* current_namespace = nullptr;

   public:
    /*!
     * @brief Creates a vanilla Lisple runtime with only the language built-in
     * functions and no file system access.
     */
    Runtime();

    /*!
     * @brief Creates a vanilla Lisple runtime with only the language built-in
     * functions and file system access via the provided FileSystem
     * implementation
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(FileSystem* fs);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespace in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespace will typically be the host application namespace.
     * The Runtime instance will assume ownership of the Namespace instance
     */
    Runtime(Namespace& ns);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespace in
     * addition to the language built-in functions and file system access via
     * the provided FileSystem implementation.
     *
     * The provided namespace will typically be the host application namespace.
     * The Runtime instance will assume ownership of the Namespace instance
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(Namespace& host_namespace, FileSystem* fs);

    /*!
     * @brief Creates a Lisple runtime with host-provided namespaces in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     */
    Runtime(std::map<const std::string, Namespace> namespaces);

    /*!
     * @brief Creates a Lisple runtime with host-provided namespaces in
     * addition to the language built-in functions and no file system access.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     */
    Runtime(const std::string& main_ns, std::map<const std::string, Namespace> namespaces);

    /*!
     * @brief Creates a Lisple runtime with a host-provided namespaces in
     * addition to the language built-in functions and file system access via
     * the provided FileSystem implementation.
     *
     * The provided namespaces will typically be the host application
     * namespaces.
     * The Runtime instance will assume ownership of the provided std::map
     * of namespaces.
     *
     * It is up to the host application to manage the life-cycle of the provided
     * FileSystem object and avoid leaving any dangling pointers to it.
     */
    Runtime(const std::string& main_ns,
            std::map<const std::string, Namespace> namespaces,
            FileSystem* fs);

    /*!
     * @brief Switch the namespace in which the lisple engine currently operates.
     * If the namespace does not currently exist, it will be created.
     *
     * If the previous namespace is empty, it will be discarded.
     */
    void switch_namespace(const std::string& ns);

    /*!
     * @brief Returns a reference to a namespace without switching the namespace
     * within the reader.
     *
     * Optionally, create it if it exists. If this is done, it is up to
     * the client/host program to evict it if it is empty.
     *
     * If not created, this method will return nullptr.
     */
    Namespace* ns(const std::string& name, bool create_if_absent = false);

    /*!
     * @brief Returns a reference to the current namespace of the reader context
     */
    Namespace& get_current_namespace();

    /*!
     * @brief Tests if this Runtime instance may access any file system
     * abstraction, and thus can read files from disk or virtual a file system.
     */
    bool has_file_system_access() const;

    void read_file(const std::string& file_name);
    void read_file(Context& ctx, const std::string& file_name);

    sptr_sobject eval(const std::string& str);
    sptr_sobject eval(Context& ctx, const std::string& str);
    sptr_sobject eval(const sptr_sobject& sexp);
    sptr_sobject eval(Context& ctx, const sptr_sobject& sexp);

    /*!
     * @brief Call a Lisple function by name with the provided arguments
     */
    sptr_sobject call_fn(const std::string& identifier, sptr_sobject_v& args);

    /*!
     * @brief Call a lisple function by name with a single argument
     */
    sptr_sobject call_fn(const std::string& identifier, sptr_sobject& arg);

    sptr_sobject_v eval_sexpression(Context& ctx, Seq& sexp);

    sptr_sobject eval_list(Context& ctx, List& list);
    sptr_sobject eval_array(Context& ctx, Array& array);
    sptr_sobject eval_map(Context& ctx, Map& map);
    sptr_sobject exec(Context& ctx, sptr_sobject_v& list);

    sptr_sobject lookup(const Word& identifier);
    sptr_rtval lookup_value(const Word& identifier);
    sptr_sobject lookup(const Word& identifier, sptr_sobject fallback);
    sptr_sobject lookup(const std::string& identifier);
    Namespace& get_ns_of(const Word& identifier);

   private:
    /*!
     * @brief Import another namespace into the current namespace, making all
     * identifiers of that namespace available to the current.
     *
     * This does not copy anything into the current namespace, but the
     * imported namespace is referenced. This means that any changes
     * to the imported namespace will be reflected in the current.
     *
     * Any duplicate identifiers between the namespace will cause the
     * the identifiers from the imported one to be shadowed and
     * inacceessible from the importing namespace.
     *
     * Importing a namespace does not trigger loading of new namespaces -
     * any namespaces to be imported must already be loaded.
     */
    void import_namespace(const std::string& namespace_name);

    /*!
     * @brief Define an alias for another namespace within the current
     * namespace.
     */
    void define_namespace_alias(const std::string& ns_name, const std::string& alias);

    friend class Context;
  };
} // namespace Lisple

#endif
