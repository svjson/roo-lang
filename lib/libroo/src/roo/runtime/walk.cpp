#include "roo/runtime/walk.h"

#include <roo/host/object.h>
#include <roo/type.h>

namespace Roo::Walk
{
  Shape shape_of(const Value& node)
  {
    if (node.type == Value::Type::NIL)
    {
      return Shape::LEAF;
    }

    if (Type::MAP.is_type_of(node))
    {
      return Shape::MAP_SHAPE;
    }

    if (node.type == Value::Type::NATIVE_OBJECT)
    {
      return node.nobj()->structural_kind() == NativeObjectStructuralKind::VECTOR
               ? Shape::SEQ_SHAPE
               : Shape::MAP_SHAPE;
    }

    if (Type::STRICT_SEQ.is_type_of(node))
    {
      return Shape::SEQ_SHAPE;
    }

    return Shape::LEAF;
  }

  sptr_val_v children_of(const Value& node, Shape shape)
  {
    if (shape == Shape::LEAF)
    {
      return {};
    }

    switch (node.type)
    {
    case Value::Type::MAP:
    case Value::Type::LIST:
    case Value::Type::VECTOR:
      return node.elements();
    case Value::Type::NATIVE_OBJECT:
      return node.nobj()->native_children();
    default:
      return {};
    }
  }
} // namespace Roo::Walk
