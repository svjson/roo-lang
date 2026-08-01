#ifndef LOOKUP_NATIVE_SOURCE_VALUES_H
#define LOOKUP_NATIVE_SOURCE_VALUES_H

#include <filesystem>
#include <string>

#include <roo/form.h>
#include <roo/runtime/value.h>
#include <roo/source.h>

namespace Roo::Lookup
{
  std::string read_file(const std::filesystem::path& path);
  sptr_val source_value(const SourceMap& source_map, const SourceRef& source);
  sptr_val range_value(const SourceRef& source);
  sptr_val form_entry_value(const SourceMap& source_map, const sptr_ast_node& form);
} // namespace Roo::Lookup

#endif
