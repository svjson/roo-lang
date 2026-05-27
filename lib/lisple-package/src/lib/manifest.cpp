#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <lisple/exception.h>
#include <lisple/form.h>
#include <lisple/io/dir_root_file_system.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>

#include <lisple-package/manifest.h>

namespace Lisple::Package
{
  namespace
  {
    std::string field_name(const Lisple::sptr_ast_node& key, const std::string& source_name)
    {
      if (key->get_type() != Form::KEYWORD)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': expected keyword field name, got " + key->to_string());
      }
      return key->as<AST::Keyword>().get_identifier();
    }

    std::string atom_string(const Lisple::sptr_ast_node& node,
                            const std::string& field,
                            const std::string& source_name)
    {
      switch (node->get_type())
      {
      case Form::STRING:
        return node->as<AST::String>().value;
      case Form::SYMBOL:
        return node->as<AST::Symbol>().value;
      case Form::KEYWORD:
        return node->as<AST::Keyword>().value;
      default:
        throw LispleException(
          "Invalid package manifest '" + source_name + "': field :" + field +
          " expected string, symbol, or keyword, got " + node->to_string());
      }
    }

    std::vector<std::string> vector_of_atoms(const Lisple::sptr_ast_node& node,
                                             const std::string& field,
                                             const std::string& source_name)
    {
      if (node->get_type() != Form::VECTOR)
      {
        throw LispleException("Invalid package manifest '" + source_name + "': field :" +
                              field + " expected vector, got " + node->to_string());
      }

      std::vector<std::string> values;
      auto& children = node->get_children();
      values.reserve(children.size());
      for (auto& child : children)
      {
        values.push_back(atom_string(child, field, source_name));
      }
      return values;
    }

    std::vector<Lisple::NamespaceRoot> namespace_root_list(const Lisple::sptr_ast_node& node,
                                                           const std::string& source_name)
    {
      if (node->get_type() != Form::MAP)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': field :namespace-roots expected map, got " +
                              node->to_string());
      }

      std::vector<Lisple::NamespaceRoot> roots;
      auto& children = node->get_children();
      if (children.size() % 2 != 0)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': namespace root map has an uneven number of forms.");
      }

      roots.reserve(children.size() / 2);
      for (size_t i = 0; i < children.size(); i += 2)
      {
        const std::string prefix = atom_string(children[i], "namespace-roots", source_name);
        const auto& path_node = children[i + 1];
        if (path_node->get_type() == Form::VECTOR)
        {
          for (const auto& path : vector_of_atoms(path_node, "namespace-roots", source_name))
          {
            roots.push_back(Lisple::NamespaceRoot{prefix, path});
          }
        }
        else
        {
          roots.push_back(Lisple::NamespaceRoot{
            prefix,
            atom_string(path_node, "namespace-roots", source_name),
          });
        }
      }
      return roots;
    }

    std::map<std::string, std::string> source_map(const Lisple::sptr_ast_node& node,
                                                  const std::string& field,
                                                  const std::string& source_name)
    {
      if (node->get_type() != Form::MAP)
      {
        throw LispleException("Invalid package manifest '" + source_name + "': field :" +
                              field + " expected map, got " + node->to_string());
      }

      std::map<std::string, std::string> result;
      auto& children = node->get_children();
      if (children.size() % 2 != 0)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': field :" + field + " map has an uneven number of forms.");
      }

      for (size_t i = 0; i < children.size(); i += 2)
      {
        result[atom_string(children[i], field, source_name)] = children[i + 1]->to_string();
      }
      return result;
    }

    std::map<std::string, std::string> tool_map(const Lisple::sptr_ast_node& node,
                                                const std::string& source_name)
    {
      if (node->get_type() != Form::MAP)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': field :tools expected map, got " + node->to_string());
      }

      std::map<std::string, std::string> result;
      auto& children = node->get_children();
      if (children.size() % 2 != 0)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': tools map has an uneven number of forms.");
      }

      for (size_t i = 0; i < children.size(); i += 2)
      {
        result[atom_string(children[i], "tools", source_name)] =
          atom_string(children[i + 1], "tools", source_name);
      }
      return result;
    }

    std::map<std::string, sptr_ast_node> map_fields(const Lisple::sptr_ast_node& node,
                                                    const std::string& source_name)
    {
      if (node->get_type() != Form::MAP)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': expected map, got " + node->to_string());
      }

      std::map<std::string, sptr_ast_node> fields;
      auto& children = node->get_children();
      if (children.size() % 2 != 0)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': map has an uneven number of forms.");
      }

      for (size_t i = 0; i < children.size(); i += 2)
      {
        fields[field_name(children[i], source_name)] = children[i + 1];
      }

      return fields;
    }

    Dependency dependency_from_map_entry(const sptr_ast_node& key,
                                         const sptr_ast_node& value,
                                         const std::string& source_name)
    {
      Dependency dependency;
      dependency.name = atom_string(key, "dependencies", source_name);

      switch (value->get_type())
      {
      case Form::STRING:
      case Form::SYMBOL:
      case Form::KEYWORD:
      {
        std::string version_or_location = atom_string(value, "dependencies", source_name);
        if (version_or_location.rfind("file:", 0) == 0)
        {
          dependency.path = version_or_location.substr(std::string("file:").size());
        }
        else
        {
          dependency.version = version_or_location;
        }
        return dependency;
      }

      case Form::MAP:
      {
        auto fields = map_fields(value, source_name);
        if (fields.count("version"))
        {
          dependency.version =
            atom_string(fields.at("version"), "dependencies", source_name);
        }
        if (fields.count("path"))
        {
          dependency.path = atom_string(fields.at("path"), "dependencies", source_name);
        }
        return dependency;
      }

      default:
        throw LispleException(
          "Invalid package manifest '" + source_name + "': dependency '" + dependency.name +
          "' expected version atom or option map, got " + value->to_string());
      }
    }

    std::vector<Dependency> dependency_list(const Lisple::sptr_ast_node& node,
                                            const std::string& source_name)
    {
      std::vector<Dependency> dependencies;

      if (node->get_type() == Form::VECTOR)
      {
        auto& children = node->get_children();
        dependencies.reserve(children.size());
        for (auto& child : children)
        {
          dependencies.push_back(
            Dependency{atom_string(child, "dependencies", source_name), "", ""});
        }
        return dependencies;
      }

      if (node->get_type() == Form::MAP)
      {
        auto& children = node->get_children();
        if (children.size() % 2 != 0)
        {
          throw LispleException("Invalid package manifest '" + source_name +
                                "': dependency map has an uneven number of forms.");
        }

        dependencies.reserve(children.size() / 2);
        for (size_t i = 0; i < children.size(); i += 2)
        {
          dependencies.push_back(
            dependency_from_map_entry(children[i], children[i + 1], source_name));
        }
        return dependencies;
      }

      throw LispleException("Invalid package manifest '" + source_name +
                            "': field :dependencies expected vector or map, got " +
                            node->to_string());
    }

    NativeLibrary native_library_from_map(const sptr_ast_node& node,
                                          const std::string& source_name)
    {
      auto fields = map_fields(node, source_name);
      NativeLibrary library;

      if (fields.count("name"))
      {
        library.name = atom_string(fields.at("name"), "native-libraries", source_name);
      }
      if (fields.count("version"))
      {
        library.version = atom_string(fields.at("version"), "native-libraries", source_name);
      }
      if (fields.count("path"))
      {
        library.path = atom_string(fields.at("path"), "native-libraries", source_name);
      }
      if (fields.count("namespaces"))
      {
        library.namespaces =
          vector_of_atoms(fields.at("namespaces"), "native-libraries", source_name);
      }

      if (library.name.empty())
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': native library missing :name.");
      }

      return library;
    }

    std::vector<NativeLibrary> native_library_list(const Lisple::sptr_ast_node& node,
                                                   const std::string& source_name)
    {
      if (node->get_type() != Form::VECTOR)
      {
        throw LispleException("Invalid package manifest '" + source_name +
                              "': field :native-libraries expected vector, got " +
                              node->to_string());
      }

      std::vector<NativeLibrary> libraries;
      auto& children = node->get_children();
      libraries.reserve(children.size());
      for (auto& child : children)
      {
        if (child->get_type() != Form::MAP)
        {
          throw LispleException("Invalid package manifest '" + source_name +
                                "': field :native-libraries expected maps, got " +
                                child->to_string());
        }
        libraries.push_back(native_library_from_map(child, source_name));
      }
      return libraries;
    }

    std::string join_path(const std::string& root, const std::string& child)
    {
      if (root.empty() || root == ".")
      {
        return child;
      }
      if (child.empty())
      {
        return root;
      }
      if (root.back() == '/')
      {
        return root + child;
      }
      return root + "/" + child;
    }

    std::string normalize_path(const std::string& path)
    {
      return std::filesystem::path(path).lexically_normal().string();
    }

    bool is_absolute_path(const std::string& path)
    {
      return std::filesystem::path(path).is_absolute();
    }

    std::string parent_path(const std::string& path)
    {
      if (path.empty() || path == ".")
      {
        return ".";
      }

      std::string trimmed = path;
      while (trimmed.size() > 1 && trimmed.back() == '/')
      {
        trimmed.pop_back();
      }

      const size_t slash = trimmed.find_last_of('/');
      if (slash == std::string::npos)
      {
        return ".";
      }
      if (slash == 0)
      {
        return "/";
      }
      return trimmed.substr(0, slash);
    }

    std::string manifest_path(const std::string& package_root)
    {
      return join_path(package_root, "package.edn");
    }

    std::string package_root_for_dependency(const std::string& search_root,
                                            const std::string& dependency)
    {
      return join_path(search_root, dependency);
    }

    std::string package_version_root_for_dependency(const std::string& search_root,
                                                    const Dependency& dependency)
    {
      return join_path(package_root_for_dependency(search_root, dependency.name),
                       dependency.version);
    }

    std::string read_manifest_source(Lisple::FileSystem& fs, const std::string& package_root)
    {
      return fs.read(manifest_path(package_root));
    }

    bool can_read_manifest(Lisple::FileSystem& fs, const std::string& package_root)
    {
      try
      {
        (void)read_manifest_source(fs, package_root);
        return true;
      }
      catch (const std::exception&)
      {
        return false;
      }
    }

    bool has_string(const std::vector<std::string>& values, const std::string& value)
    {
      return std::find(values.begin(), values.end(), value) != values.end();
    }

    void append_unique(std::vector<std::string>& values, const std::string& value)
    {
      if (!has_string(values, value))
      {
        values.push_back(value);
      }
    }

    bool has_namespace_root(const std::vector<Lisple::NamespaceRoot>& roots,
                            const Lisple::NamespaceRoot& root)
    {
      return std::find_if(roots.begin(),
                          roots.end(),
                          [&](const Lisple::NamespaceRoot& existing)
                          {
                            return existing.ns_prefix == root.ns_prefix &&
                                   existing.path == root.path;
                          }) != roots.end();
    }

    void append_unique(std::vector<Lisple::NamespaceRoot>& roots,
                       const Lisple::NamespaceRoot& root)
    {
      if (!has_namespace_root(roots, root))
      {
        roots.push_back(root);
      }
    }

    std::string describe_roots(const std::vector<std::string>& roots)
    {
      std::string result;
      for (size_t i = 0; i < roots.size(); i++)
      {
        if (i > 0)
        {
          result += ", ";
        }
        result += roots[i];
      }
      return result;
    }

    std::vector<std::string> candidate_versioned_roots(const std::string& search_root,
                                                       const Dependency& dependency)
    {
      if (!dependency.version.empty())
      {
        return {package_version_root_for_dependency(search_root, dependency)};
      }
      return {};
    }

    std::string dependency_path_root(const std::string& package_root,
                                     const Dependency& dependency)
    {
      if (is_absolute_path(dependency.path))
      {
        return normalize_path(dependency.path);
      }
      return normalize_path(join_path(package_root, dependency.path));
    }

    std::string find_dependency_root(Lisple::FileSystem& fs,
                                     const Dependency& dependency,
                                     const std::string& package_root,
                                     const std::vector<std::string>& search_roots)
    {
      if (!dependency.path.empty())
      {
        const std::string root = dependency_path_root(package_root, dependency);
        try
        {
          (void)read_manifest_source(fs, root);
          return root;
        }
        catch (const std::exception&)
        {
          throw LispleException("Package dependency '" + dependency.name +
                                "' was not found at path: " + root);
        }
      }

      for (const auto& search_root : search_roots)
      {
        for (const auto& candidate : candidate_versioned_roots(search_root, dependency))
        {
          if (can_read_manifest(fs, candidate))
          {
            return candidate;
          }
        }

        const std::string candidate =
          package_root_for_dependency(search_root, dependency.name);
        if (can_read_manifest(fs, candidate))
        {
          return candidate;
        }
      }

      throw LispleException(
        "Package dependency '" + dependency.name +
        "' was not found under search roots: " + describe_roots(search_roots));
    }

    void validate_dependency_version(const Dependency& dependency,
                                     const Manifest& dependency_manifest,
                                     const std::string& dependency_root)
    {
      if (dependency.version.empty())
      {
        return;
      }

      if (dependency_manifest.version != dependency.version)
      {
        throw LispleException("Package dependency '" + dependency.name + "' at '" +
                              dependency_root + "' has version '" +
                              dependency_manifest.version + "', expected '" +
                              dependency.version + "'.");
      }
    }

    struct ResolveState
    {
      Lisple::FileSystem& fs;
      std::vector<std::string> search_roots;
      std::set<std::string> visiting;
      std::set<std::string> visited;
      std::map<std::string, std::pair<std::string, std::string>> resolved_by_name;
      LoadPlan plan;
    };

    bool already_resolved_package(ResolveState& state,
                                  const Manifest& manifest,
                                  const std::string& package_root)
    {
      if (manifest.name.empty())
      {
        return false;
      }

      auto existing = state.resolved_by_name.find(manifest.name);
      if (existing == state.resolved_by_name.end())
      {
        return false;
      }

      const std::string& existing_version = existing->second.first;
      const std::string& existing_root = existing->second.second;
      if (existing_version != manifest.version)
      {
        throw LispleException("Package dependency conflict for '" + manifest.name +
                              "': already resolved version '" + existing_version + "' at '" +
                              existing_root + "', but '" + package_root + "' has version '" +
                              manifest.version + "'.");
      }

      return existing_root != package_root;
    }

    void mark_resolved_package(ResolveState& state,
                               const Manifest& manifest,
                               const std::string& package_root)
    {
      if (!manifest.name.empty())
      {
        state.resolved_by_name[manifest.name] = {manifest.version, package_root};
      }
    }

    void append_package_to_plan(LoadPlan& plan,
                                const Manifest& manifest,
                                const std::string& package_root)
    {
      append_unique(plan.package_roots, package_root);
      PackageInfo package_info;
      package_info.name = manifest.name;
      package_info.version = manifest.version;
      package_info.package_root = package_root;
      package_info.load_roots.reserve(manifest.load_roots.size());
      for (const auto& root : manifest.load_roots)
      {
        const std::string resolved_root = join_path(package_root, root);
        append_unique(plan.load_paths, resolved_root);
        package_info.load_roots.push_back(resolved_root);
      }
      package_info.config = manifest.config;
      package_info.tools = manifest.tools;
      plan.packages.push_back(package_info);
      for (const auto& root : manifest.namespace_roots)
      {
        Lisple::NamespaceRoot resolved = root;
        if (!resolved.path.empty() && !is_absolute_path(resolved.path))
        {
          resolved.path = normalize_path(join_path(package_root, resolved.path));
        }
        append_unique(plan.namespace_roots, resolved);
      }
      for (const auto& native_namespace : manifest.native_namespaces)
      {
        append_unique(plan.native_namespaces, native_namespace);
      }
      for (const auto& native_library : manifest.native_libraries)
      {
        NativeLibrary resolved = native_library;
        resolved.package_root = package_root;
        if (!resolved.path.empty() && !is_absolute_path(resolved.path))
        {
          resolved.path = normalize_path(join_path(package_root, resolved.path));
        }
        plan.native_libraries.push_back(resolved);
        for (const auto& native_namespace : resolved.namespaces)
        {
          append_unique(plan.native_namespaces, native_namespace);
        }
      }
      for (const auto& autoload : manifest.autoloads)
      {
        append_unique(plan.autoloads, autoload);
      }
      for (const auto& entry_point : manifest.entry_points)
      {
        append_unique(plan.entry_points, entry_point);
      }
      if (package_root == plan.package_root && !manifest.main.empty())
      {
        plan.main = manifest.main;
      }
      if (package_root == plan.package_root && !manifest.run.empty())
      {
        plan.run = manifest.run;
      }
    }

    void resolve_package(ResolveState& state, const std::string& package_root)
    {
      if (state.visited.count(package_root))
      {
        return;
      }
      if (state.visiting.count(package_root))
      {
        throw LispleException("Cyclic package dependency involving '" + package_root + "'.");
      }

      state.visiting.insert(package_root);
      Manifest manifest = read_manifest(state.fs, manifest_path(package_root));
      if (already_resolved_package(state, manifest, package_root))
      {
        state.visiting.erase(package_root);
        state.visited.insert(package_root);
        return;
      }

      for (const auto& dependency : manifest.dependencies)
      {
        const std::string dependency_root =
          find_dependency_root(state.fs, dependency, package_root, state.search_roots);
        validate_dependency_version(dependency,
                                    read_manifest(state.fs, manifest_path(dependency_root)),
                                    dependency_root);
        resolve_package(state, dependency_root);
      }

      append_package_to_plan(state.plan, manifest, package_root);
      mark_resolved_package(state, manifest, package_root);
      state.visiting.erase(package_root);
      state.visited.insert(package_root);
    }
  } // namespace

  Manifest parse_manifest(const std::string& source, const std::string& source_name)
  {
    Reader reader;
    sptr_ast_node_v forms = reader.read_sexps(source);
    if (forms.size() != 1)
    {
      throw LispleException("Invalid package manifest '" + source_name +
                            "': expected one top-level map.");
    }

    sptr_ast_node manifest_form = forms.front();
    if (manifest_form->get_type() != Form::MAP)
    {
      throw LispleException("Invalid package manifest '" + source_name +
                            "': expected top-level map.");
    }

    std::map<std::string, sptr_ast_node> fields = map_fields(manifest_form, source_name);

    Manifest manifest;

    if (fields.count("name"))
    {
      manifest.name = atom_string(fields.at("name"), "name", source_name);
    }
    if (fields.count("version"))
    {
      manifest.version = atom_string(fields.at("version"), "version", source_name);
    }
    if (fields.count("description"))
    {
      manifest.description =
        atom_string(fields.at("description"), "description", source_name);
    }
    if (fields.count("dependencies"))
    {
      manifest.dependencies = dependency_list(fields.at("dependencies"), source_name);
    }
    if (fields.count("load-roots"))
    {
      manifest.load_roots =
        vector_of_atoms(fields.at("load-roots"), "load-roots", source_name);
    }
    if (fields.count("namespace-roots"))
    {
      manifest.namespace_roots =
        namespace_root_list(fields.at("namespace-roots"), source_name);
    }
    if (fields.count("native-namespaces"))
    {
      manifest.native_namespaces =
        vector_of_atoms(fields.at("native-namespaces"), "native-namespaces", source_name);
    }
    if (fields.count("native-libraries"))
    {
      manifest.native_libraries =
        native_library_list(fields.at("native-libraries"), source_name);
    }
    if (fields.count("autoloads"))
    {
      manifest.autoloads = vector_of_atoms(fields.at("autoloads"), "autoloads", source_name);
    }
    if (fields.count("config"))
    {
      manifest.config = source_map(fields.at("config"), "config", source_name);
    }
    if (fields.count("tools"))
    {
      manifest.tools = tool_map(fields.at("tools"), source_name);
    }
    if (fields.count("entry-points"))
    {
      manifest.entry_points =
        vector_of_atoms(fields.at("entry-points"), "entry-points", source_name);
    }
    if (fields.count("main"))
    {
      manifest.main = atom_string(fields.at("main"), "main", source_name);
    }
    if (fields.count("run"))
    {
      manifest.run = atom_string(fields.at("run"), "run", source_name);
    }

    return manifest;
  }

  Manifest read_manifest(Lisple::FileSystem& fs, const std::string& manifest_path)
  {
    return parse_manifest(fs.read(manifest_path), manifest_path);
  }

  std::string default_local_repository_root()
  {
    const char* home = std::getenv("HOME");
    if (home && *home)
    {
      return normalize_path(join_path(home, ".local/share/lisple/pkg"));
    }

    return normalize_path("~/.local/share/lisple/pkg");
  }

  std::vector<std::string> default_package_search_roots()
  {
    return {default_local_repository_root()};
  }

  LoadPlan build_load_plan(const Manifest& manifest, const std::string& package_root)
  {
    LoadPlan plan;
    plan.package_root = package_root;
    plan.package_roots.push_back(package_root);
    PackageInfo package_info;
    package_info.name = manifest.name;
    package_info.version = manifest.version;
    package_info.package_root = package_root;
    package_info.config = manifest.config;
    package_info.tools = manifest.tools;
    plan.native_namespaces = manifest.native_namespaces;
    plan.namespace_roots = manifest.namespace_roots;
    plan.native_libraries = manifest.native_libraries;
    plan.autoloads = manifest.autoloads;
    plan.entry_points = manifest.entry_points;
    plan.main = manifest.main;
    plan.run = manifest.run;

    for (auto& native_library : plan.native_libraries)
    {
      native_library.package_root = package_root;
      if (!native_library.path.empty() && !is_absolute_path(native_library.path))
      {
        native_library.path = normalize_path(join_path(package_root, native_library.path));
      }
      for (const auto& native_namespace : native_library.namespaces)
      {
        append_unique(plan.native_namespaces, native_namespace);
      }
    }

    for (auto& namespace_root : plan.namespace_roots)
    {
      if (!namespace_root.path.empty() && !is_absolute_path(namespace_root.path))
      {
        namespace_root.path = normalize_path(join_path(package_root, namespace_root.path));
      }
    }

    plan.load_paths.reserve(manifest.load_roots.size());
    package_info.load_roots.reserve(manifest.load_roots.size());
    for (const auto& root : manifest.load_roots)
    {
      const std::string resolved_root = join_path(package_root, root);
      plan.load_paths.push_back(resolved_root);
      package_info.load_roots.push_back(resolved_root);
    }
    plan.packages.push_back(std::move(package_info));

    return plan;
  }

  LoadPlan resolve_load_plan(Lisple::FileSystem& fs,
                             const std::string& package_root,
                             const ResolveOptions& options)
  {
    ResolveState state{
      fs,
      options.package_search_roots,
      {},
      {},
      {},
      {},
    };
    append_unique(state.search_roots, parent_path(package_root));
    for (const auto& search_root : default_package_search_roots())
    {
      append_unique(state.search_roots, search_root);
    }
    state.plan.package_root = package_root;

    resolve_package(state, package_root);

    return state.plan;
  }

  std::vector<std::string> merge_load_paths(const LoadPlan& plan,
                                            const std::vector<std::string>& extra_load_paths)
  {
    std::vector<std::string> load_paths = extra_load_paths;
    load_paths.insert(load_paths.end(), plan.load_paths.begin(), plan.load_paths.end());
    return load_paths;
  }

  std::unique_ptr<Lisple::FileSystem> make_load_path_file_system(
    const LoadPlan& plan,
    const std::vector<std::string>& extra_load_paths)
  {
    return std::make_unique<Lisple::DirRootFileSystem>(
      merge_load_paths(plan, extra_load_paths));
  }

  void configure_runtime_namespace_roots(Lisple::Runtime& runtime, const LoadPlan& plan)
  {
    runtime.set_namespace_roots(plan.namespace_roots);
  }

  void load_autoloads(Lisple::Runtime& runtime, const LoadPlan& plan)
  {
    for (const auto& autoload : plan.autoloads)
    {
      runtime.eval("(ns lisple.package.autoload (:require " + autoload + "))",
                   "<package-autoload>");
    }
  }
} // namespace Lisple::Package
