#ifndef ROOC_PROJECT_H
#define ROOC_PROJECT_H

#include <string>
#include <vector>

#include <roo-package/manifest.h>
#include <rooc/options.h>

namespace Rooc
{
  struct EmbeddedFile
  {
    std::string key;
    std::string source;
  };

  struct GeneratedProject
  {
    std::string executable_name;
    Roo::Package::LoadPlan plan;
    std::vector<EmbeddedFile> files;
  };

  GeneratedProject prepare_project(const Options& options);
} // namespace Rooc

#endif
