#ifndef LISPLEC_GENERATOR_H
#define LISPLEC_GENERATOR_H

#include <lisplec/options.h>
#include <lisplec/project.h>

namespace Lisplec
{
  void generate_project(const Options& options, const GeneratedProject& project);
} // namespace Lisplec

#endif
