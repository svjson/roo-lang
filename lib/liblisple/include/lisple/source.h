#ifndef LISPLE__SOURCE_H
#define LISPLE__SOURCE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lisple
{
  struct SourcePosition
  {
    uint32_t line = 0;
    uint32_t column = 0;

    bool valid() const { return line > 0 && column > 0; }
  };

  struct SourceSpan
  {
    SourcePosition start;
    SourcePosition end;

    bool valid() const { return start.valid(); }
  };

  struct SourceRef
  {
    uint32_t file_id = 0;
    SourceSpan span;

    bool valid() const { return file_id > 0 && span.valid(); }
  };

  class SourceMap
  {
    std::vector<std::string> files;
    std::unordered_map<std::string, uint32_t> file_ids;

   public:
    uint32_t intern_file(const std::string& file_name)
    {
      auto existing = file_ids.find(file_name);
      if (existing != file_ids.end())
      {
        return existing->second;
      }

      const uint32_t id = static_cast<uint32_t>(files.size() + 1);
      files.push_back(file_name);
      file_ids.emplace(file_name, id);
      return id;
    }

    const std::string& file_name(uint32_t file_id) const
    {
      static const std::string unknown = "<unknown>";
      if (file_id == 0 || file_id > files.size())
      {
        return unknown;
      }
      return files[file_id - 1];
    }

    std::string describe(const SourceRef& source) const
    {
      if (!source.valid())
      {
        return "<unknown>";
      }

      return file_name(source.file_id) + ":" + std::to_string(source.span.start.line) +
             ":" + std::to_string(source.span.start.column);
    }
  };

  struct RuntimeOptions
  {
    bool source_diagnostics = false;
    bool call_stack_diagnostics = false;
  };
} // namespace Lisple

#endif
