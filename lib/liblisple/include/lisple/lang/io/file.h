#ifndef LISPLE__LANG__IO__FILE_H
#define LISPLE__LANG__IO__FILE_H

#include <lisple/exec.h>

namespace Lisple
{
  FUNC(SlurpBangFunction, slurp)
  FUNC(SpitBangFunction, spit)
  FUNC(SlurpEdnBangFunction, slurp_edn)
  FUNC(SpitEdnBangFunction, spit_edn)
} // namespace Lisple

#endif
