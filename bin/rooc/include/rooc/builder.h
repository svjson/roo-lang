#ifndef ROOC_BUILDER_H
#define ROOC_BUILDER_H

#include <rooc/options.h>
#include <rooc/project.h>

namespace Rooc
{
  void build_project(const Options& options, const GeneratedProject& project);
} // namespace Rooc

#endif
