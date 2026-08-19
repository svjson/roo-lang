#include <roo/runtime/value.h>
#include <roo/runtime/walk.h>

#include <gtest/gtest.h>

using Roo::Value;
using Roo::Walk::Control;
using Roo::Walk::walk;

namespace
{
  /*!
   * @brief Records the to_string() of every leaf node visited, and the
   * open/close of every composite, in traversal order - enough to assert
   * on shape and ordering without needing a real consumer (like
   * pretty-printing) in the loop.
   */
  struct RecordingVisitor
  {
    std::vector<std::string> events;

    Control enter(const Value& node)
    {
      switch (node.type)
      {
      case Value::Type::MAP:
        events.push_back("enter-map");
        return Control::DESCEND;
      case Value::Type::LIST:
        events.push_back("enter-list");
        return Control::DESCEND;
      case Value::Type::VECTOR:
        events.push_back("enter-vector");
        return Control::DESCEND;
      default:
        events.push_back("leaf:" + node.to_string());
        return Control::SKIP;
      }
    }

    void leave(const Value& node)
    {
      switch (node.type)
      {
      case Value::Type::MAP:
        events.push_back("leave-map");
        break;
      case Value::Type::LIST:
        events.push_back("leave-list");
        break;
      case Value::Type::VECTOR:
        events.push_back("leave-vector");
        break;
      default:
        break;
      }
    }
  };

  /*!
   * @brief Stops as soon as it sees a leaf equal to `target`, proving Stop
   * aborts the remainder of the traversal instead of merely skipping one
   * subtree.
   */
  struct StoppingVisitor
  {
    std::string target;
    std::vector<std::string> seen;

    Control enter(const Value& node)
    {
      if (node.type == Value::Type::MAP || node.type == Value::Type::LIST ||
          node.type == Value::Type::VECTOR)
      {
        return Control::DESCEND;
      }
      seen.push_back(node.to_string());
      return node.to_string() == target ? Control::STOP : Control::SKIP;
    }

    void leave(const Value&) {}
  };
} // namespace

TEST(Walk, visits_a_leaf_without_calling_leave)
{
  // Given
  RecordingVisitor visitor;

  // When
  walk(*Value::number(1), visitor);

  // Then
  EXPECT_EQ(visitor.events, std::vector<std::string>({"leaf:1"}));
}

TEST(Walk, visits_vector_elements_in_order_between_enter_and_leave)
{
  // Given
  RecordingVisitor visitor;
  auto vec = Value::vector({Value::number(1), Value::number(2), Value::number(3)});

  // When
  walk(*vec, visitor);

  // Then
  EXPECT_EQ(visitor.events,
            std::vector<std::string>(
              {"enter-vector", "leaf:1", "leaf:2", "leaf:3", "leave-vector"}));
}

TEST(Walk, visits_map_keys_and_values_in_slot_order)
{
  // Given
  RecordingVisitor visitor;
  auto map = Value::map(
    {Value::keyword("a"), Value::number(1), Value::keyword("b"), Value::number(2)});

  // When
  walk(*map, visitor);

  // Then
  EXPECT_EQ(visitor.events,
            std::vector<std::string>(
              {"enter-map", "leaf::a", "leaf:1", "leaf::b", "leaf:2", "leave-map"}));
}

TEST(Walk, recurses_into_nested_composites)
{
  // Given
  RecordingVisitor visitor;
  auto nested = Value::vector({Value::number(1), Value::list({Value::number(2)})});

  // When
  walk(*nested, visitor);

  // Then
  EXPECT_EQ(
    visitor.events,
    std::vector<std::string>(
      {"enter-vector", "leaf:1", "enter-list", "leaf:2", "leave-list", "leave-vector"}));
}

TEST(Walk, skip_omits_children_but_does_not_call_leave)
{
  // Given
  struct SkipCompositesVisitor
  {
    std::vector<std::string> events;

    Control enter(const Value& node)
    {
      if (node.type == Value::Type::VECTOR)
      {
        events.push_back("skip-vector");
        return Control::SKIP;
      }
      events.push_back("leaf:" + node.to_string());
      return Control::SKIP;
    }

    void leave(const Value&) { events.push_back("unexpected-leave"); }
  };

  SkipCompositesVisitor visitor;
  auto vec = Value::vector({Value::number(1), Value::number(2)});

  // When
  walk(*vec, visitor);

  // Then
  EXPECT_EQ(visitor.events, std::vector<std::string>({"skip-vector"}));
}

TEST(Walk, stop_aborts_the_remainder_of_the_walk)
{
  // Given
  StoppingVisitor visitor{"2", {}};
  auto vec = Value::vector({Value::number(1), Value::number(2), Value::number(3)});

  // When
  walk(*vec, visitor);

  // Then
  EXPECT_EQ(visitor.seen, std::vector<std::string>({"1", "2"}));
}

TEST(Walk, generic_child_provider_expands_only_descended_nodes)
{
  struct Node
  {
    std::string value;
    int depth;
  };

  struct Children
  {
    std::vector<std::string> expanded;

    std::vector<Node> children(const Node& node)
    {
      expanded.push_back(node.value);
      if (node.value == "root") return {{"keep", 1}, {"skip", 1}};
      if (node.value == "keep") return {{"leaf", 2}};
      if (node.value == "skip") return {{"never", 2}};
      return {};
    }
  } children;

  struct Visitor
  {
    std::vector<std::string> visited;

    Control enter(const Node& node)
    {
      visited.push_back(node.value + ":" + std::to_string(node.depth));
      if (node.value == "skip" || node.value == "leaf") return Control::SKIP;
      return Control::DESCEND;
    }

    void leave(const Node&) {}
  } visitor;

  walk(Node{"root", 0}, children, visitor);

  EXPECT_EQ(visitor.visited,
            std::vector<std::string>({"root:0", "keep:1", "leaf:2", "skip:1"}));
  EXPECT_EQ(children.expanded, std::vector<std::string>({"root", "keep"}));
}

TEST(WalkShapeOf, nil_is_always_a_leaf)
{
  // Given/When/Then: every TypeRef in type.h (including MAP and STRICT_SEQ's
  // members) reports itself as a match for nil, so nil must be excluded
  // before any type-based check runs, or it would be misclassified as
  // whichever composite shape happens to be checked first.
  EXPECT_EQ(Roo::Walk::shape_of(*Roo::Constant::NIL), Roo::Walk::Shape::LEAF);
}

TEST(WalkShapeOf, map_is_map_shaped)
{
  auto map = Value::map({Value::keyword("a"), Value::number(1)});
  EXPECT_EQ(Roo::Walk::shape_of(*map), Roo::Walk::Shape::MAP_SHAPE);
}

TEST(WalkShapeOf, list_and_vector_are_seq_shaped)
{
  EXPECT_EQ(Roo::Walk::shape_of(*Value::list({})), Roo::Walk::Shape::SEQ_SHAPE);
  EXPECT_EQ(Roo::Walk::shape_of(*Value::vector({})), Roo::Walk::Shape::SEQ_SHAPE);
}

TEST(WalkShapeOf, scalars_are_leaves)
{
  EXPECT_EQ(Roo::Walk::shape_of(*Value::number(1)), Roo::Walk::Shape::LEAF);
  EXPECT_EQ(Roo::Walk::shape_of(*Value::string("s")), Roo::Walk::Shape::LEAF);
  EXPECT_EQ(Roo::Walk::shape_of(*Value::boolean(true)), Roo::Walk::Shape::LEAF);
}
