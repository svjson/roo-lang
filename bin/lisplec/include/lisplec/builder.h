#ifndef LISPLEC_BUILDER_H
#define LISPLEC_BUILDER_H

#include <lisplec/options.h>
#include <lisplec/project.h>

namespace Lisplec
{
  void build_project(const Options& options, const GeneratedProject& project);
} // namespace Lisplec

#endif
