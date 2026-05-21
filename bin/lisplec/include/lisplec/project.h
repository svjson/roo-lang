#ifndef LISPLEC_PROJECT_H
#define LISPLEC_PROJECT_H

#include <string>
#include <vector>

#include <lisple-package/manifest.h>
#include <lisplec/options.h>

namespace Lisplec
{
  struct EmbeddedFile
  {
    std::string key;
    std::string source;
  };

  struct GeneratedProject
  {
    std::string executable_name;
    Lisple::Package::LoadPlan plan;
    std::vector<EmbeddedFile> files;
  };

  GeneratedProject prepare_project(const Options& options);
} // namespace Lisplec

#endif
