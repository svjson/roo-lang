#ifndef ROOPL__TARGET__DEDICATED_H
#define ROOPL__TARGET__DEDICATED_H

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <roo/type.h>

namespace Roo::Roopl::Target
{
  struct DedicatedOptions
  {
    std::vector<std::string> load_paths;
    std::optional<std::string> package_root;
  };

  struct EvaluationRequest
  {
    std::string source;
    std::string namespace_name;
    std::string source_name;
    std::optional<std::size_t> print_width;
  };

  class Dedicated
  {
   public:
    explicit Dedicated(DedicatedOptions options);
    ~Dedicated();

    Dedicated(const Dedicated&) = delete;
    Dedicated& operator=(const Dedicated&) = delete;

    sptr_val evaluate(const EvaluationRequest& request);
    void close();

   private:
    struct State;
    std::unique_ptr<State> state;
  };
} // namespace Roo::Roopl::Target

#endif /* ROOPL__TARGET__DEDICATED_H */
