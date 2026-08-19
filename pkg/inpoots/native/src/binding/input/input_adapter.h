#ifndef INPOOTS__BINDING__INPUT__INPUT_ADAPTER_H
#define INPOOTS__BINDING__INPUT__INPUT_ADAPTER_H

#include <roo/host/object.h>
#include <roo/host/type.h>

#include <terminal/input.h>

namespace Roo::Inpoots
{
  inline const Roo::HostTypeRef INPOOTS_INPUT_TYPE("inpoots-input");

  NATIVE_ADAPTER(InputAdapter, Terminal::Input);
} // namespace Roo::Inpoots

#endif /* INPOOTS__BINDING__INPUT__INPUT_ADAPTER_H */
