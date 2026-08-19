#ifndef ROO__RUNTIME_WALK_H
#define ROO__RUNTIME_WALK_H

#include <roo/runtime/value.h>

namespace Roo::Walk
{
  /*!
   * @brief Result of visiting one Value node, returned by Visitor::enter.
   *
   * DESCEND - recurse into this node's children (only meaningful when
   * shape_of(node) is not Shape::LEAF; harmless otherwise), then call
   * Visitor::leave once they have all been visited.
   * SKIP - do not recurse and do not call Visitor::leave. The visitor has
   * already produced whatever it wants for this node (or has decided to
   * omit it entirely, e.g. past a depth/element limit).
   * STOP - abort the walk immediately, propagating up through every
   * enclosing call.
   */
  enum class Control
  {
    DESCEND,
    SKIP,
    STOP
  };

  /*!
   * @brief What a node looks like structurally, for walking purposes.
   *
   * LEAF - no children: NIL and scalars. Host objects are never opaque
   * here - every host object is either MAP_SHAPE (the default, including
   * one with nothing to expose, which walks as zero children and prints as
   * `{}`) or SEQ_SHAPE for the sequence specialization (a wrapped
   * std::vector, et al.). Native objects and their adapters have no
   * literal source syntax and cannot exist as AST, so Value::Type::OBJECT
   * is never walkable - it is always a LEAF.
   * MAP_SHAPE - key/value pairs: a MAP, or a NATIVE_OBJECT reporting
   * NativeObjectStructuralKind::MAP (NativeObjectBase's default).
   * SEQ_SHAPE - a flat sequence of elements: a LIST or VECTOR, or a
   * NATIVE_OBJECT reporting NativeObjectStructuralKind::VECTOR.
   *
   * NIL is checked first and always resolves to LEAF - every TypeRef in
   * type.h reports itself as a match for NIL, so it must never reach any
   * type-matching logic here.
   */
  enum class Shape
  {
    LEAF,
    MAP_SHAPE,
    SEQ_SHAPE
  };

  Shape shape_of(const Value& node);

  /*!
   * @brief The flat children of `node`, given `shape` (as returned by
   * shape_of(node)) - key0, value0, key1, value1, ... for MAP_SHAPE; plain
   * elements for SEQ_SHAPE. Empty for LEAF.
   */
  sptr_val_v children_of(const Value& node, Shape shape);

  /*!
   * @brief Recursively visit an arbitrary node tree whose immediate children are
   * materialized on demand by `child_provider`.
   *
   * `ChildProvider` supplies:
   *
   *   std::vector<Node> children(const Node& node);
   *
   * The provider is called only after `Visitor::enter` returns DESCEND. This
   * makes the traversal suitable for trees backed by external resources: a
   * skipped node is never expanded, and a provider can finish and release the
   * resource used to enumerate one level before its children are visited.
   *
   * `Node` may carry traversal metadata alongside the underlying value. The
   * walker treats it as opaque and passes it unchanged to the provider and
   * visitor.
   */
  template <typename Node, typename ChildProvider, typename Visitor>
  Control walk(const Node& node, ChildProvider& child_provider, Visitor& visitor)
  {
    const Control control = visitor.enter(node);
    if (control == Control::STOP)
    {
      return Control::STOP;
    }

    if (control == Control::DESCEND)
    {
      const auto children = child_provider.children(node);
      for (const auto& child : children)
      {
        if (walk(child, child_provider, visitor) == Control::STOP)
        {
          return Control::STOP;
        }
      }
      visitor.leave(node);
    }

    return Control::DESCEND;
  }

  /*!
   * @brief Recursively visit `node` and, if shape_of(node) is not
   * Shape::LEAF and Visitor::enter returns Control::DESCEND, its children
   * (MAP_SHAPE children are visited key then value, in slot order;
   * SEQ_SHAPE children in element order).
   *
   * This is the entire "generic walking mechanism": it knows nothing about
   * why a node is being visited or what should be done with it - that is
   * entirely up to `Visitor`, which supplies:
   *
   *   Control enter(const Value& node);
   *   void leave(const Value& node);   // only called after a DESCEND
   *
   * and is free to carry whatever state it needs (an accumulator, a stack
   * of partially-built results, a roo callable to invoke per node, ...).
   * Being a template rather than a virtual interface, each Visitor produces
   * its own fully-specialized, inlinable instantiation of `walk` - a native
   * Visitor (e.g. building a pretty-printed layout) pays nothing beyond its
   * own bookkeeping; only a Visitor that itself does something costly per
   * node (e.g. invoking a roo callable) pays for that.
   */
  template <typename Visitor> Control walk(const Value& node, Visitor& visitor)
  {
    struct ValueNode
    {
      sptr_val owner;
      const Value* value;
    };

    struct ValueChildren
    {
      std::vector<ValueNode> children(const ValueNode& parent)
      {
        std::vector<ValueNode> result;
        for (const sptr_val& child : children_of(*parent.value, shape_of(*parent.value)))
        {
          result.push_back({child, child.get()});
        }
        return result;
      }
    } child_provider;

    struct ValueVisitor
    {
      Visitor& visitor;

      Control enter(const ValueNode& current) { return visitor.enter(*current.value); }

      void leave(const ValueNode& current) { visitor.leave(*current.value); }
    } value_visitor{visitor};

    return walk(ValueNode{nullptr, &node}, child_provider, value_visitor);
  }
} // namespace Roo::Walk

#endif
