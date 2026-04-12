
#include "lisple/exception.h"
#include "lisple/exec.h"
#include "lisple/reader.h"
#include "lisple/runtime/node.h"
#include "lisple/type.h"

#include <lisple/bind.h>
#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/runtime.h>
#include <lisple/runtime/exec_node.h>
#include <lisple/runtime/lower.h>

#include <gtest/gtest.h>

TEST(LowerTest, number__literal_node)
{
  // Given
  Lisple::LowerContext lctx;
  Lisple::sptr_sobject number = Lisple::Number::make(4);

  // When
  auto node = Lisple::lower_expr(lctx, number);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::LiteralNode>(node->data));
}

TEST(LowerTest, list__call_node__no_ctx__lookup_node_callee)
{
  Lisple::LowerContext lctx;
  Lisple::sptr_sobject expr = Lisple::List::make(
    {Lisple::Word::make("+"), Lisple::Number::make(1), Lisple::Number::make(1)});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::CallNode>(node->data));
  Lisple::CallNode& call_node = std::get<Lisple::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Lisple::LookupNode>(call_node.callee->data));
}

TEST(LowerTest, list__call_node__ctx__literal_node_callee)
{
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject expr = Lisple::List::make(
    {Lisple::Word::make("+"), Lisple::Number::make(1), Lisple::Number::make(1)});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::CallNode>(node->data));
  Lisple::CallNode& call_node = std::get<Lisple::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Lisple::LiteralNode>(call_node.callee->data));
}

TEST(LowerTest, list__key_lookup__no_ctx__lookup_node)
{
  Lisple::LowerContext lctx;
  Lisple::sptr_sobject expr =
    Lisple::List::make({Lisple::Key::make("name"), Lisple::Word::make("my-map")});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::KeyLookupNode>(node->data));
  Lisple::KeyLookupNode& key_node = std::get<Lisple::KeyLookupNode>(node->data);
  EXPECT_EQ(*key_node.keyword, *Lisple::RTValue::keyword("name"));
  EXPECT_TRUE(std::holds_alternative<Lisple::LookupNode>(key_node.target->data));
}

TEST(LowerTest, list__key_lookup__ctx__lookup_node)
{
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject expr =
    Lisple::List::make({Lisple::Key::make("name"), Lisple::Word::make("my-map")});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::KeyLookupNode>(node->data));
  Lisple::KeyLookupNode& key_node = std::get<Lisple::KeyLookupNode>(node->data);
  EXPECT_EQ(*key_node.keyword, *Lisple::RTValue::keyword("name"));
  EXPECT_TRUE(std::holds_alternative<Lisple::LookupNode>(key_node.target->data));
}

TEST(LowerTest, list_special_form__ctx__custom_lower)
{
  Lisple::Runtime runtime;
  Lisple::Context ctx(runtime);
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject expr = Lisple::List::make(
    {Lisple::Word::make("->"),
     Lisple::Number::make(10),
     Lisple::List::make({Lisple::Word::make("+"), Lisple::Number::make(5)}),
     Lisple::Word::make("range")});

  // When
  auto node = Lisple::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Lisple::CallNode>(node->data));
}

SPECIAL_FORM_DECL(DefThing, def_thing)

SPECIAL_FORM_IMPL(DefThing,
                  SIG((FN_ARGS((&Lisple::Type::WORD), (&Lisple::Type::MAP)),
                       EXEC_DISPATCH(&DefThing::inv_def_thing,
                                     &DefThing::execnode_def_thing))));

SFORM_LOWER_IMPL(DefThing)
{
  return std::make_unique<Lisple::ExecNode>(Lisple::Constant::NIL);
}

MACRO_BODY(DefThing, inv_def_thing)
{
  throw Lisple::LispleException("inv_def_thing should not be called");
}

EXECNODE_BODY(DefThing, execnode_def_thing)
{
  throw Lisple::LispleException("execnode_def_thing should not be called");
}

class CustomNamespace : public Lisple::Namespace
{
 public:
  CustomNamespace()
    : Lisple::Namespace("custom")
  {
    values.emplace("defthing", DefThing::make());
  }
};

TEST(LowerTest, lower_custom_special_form)
{
  // Given
  Lisple::Reader reader;
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<CustomNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);
  runtime.eval("(ns my-app.core (:require custom))");
  Lisple::Context ctx(runtime);
  ctx.switch_namespace("my-app.core");
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject_v def_thing_expr =
    reader.read_sexps("(defthing my-thing {:value \"a lot\"})");

  // When
  auto node = Lisple::lower_expr(lctx, def_thing_expr.front());

  // Then
  auto* literal_node = std::get_if<Lisple::LiteralNode>(&node->data);
  ASSERT_EQ(*literal_node->value, *Lisple::Constant::NIL);
}

TEST(LowerTest, lower_namespace_qualified_custom_special_form)
{
  // Given
  Lisple::Reader reader;
  std::vector<std::unique_ptr<Lisple::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<CustomNamespace>());
  Lisple::Runtime runtime(std::move(namespaces), nullptr);
  Lisple::Context ctx(runtime);
  ctx.switch_namespace("my-app.core");
  Lisple::LowerContext lctx{&ctx};
  Lisple::sptr_sobject_v def_thing_expr =
    reader.read_sexps("(custom/defthing my-thing {:value \"a lot\"})");

  // When
  Lisple::uptr_exec_node node = Lisple::lower_expr(lctx, def_thing_expr.front());

  // Then
  auto* literal_node = std::get_if<Lisple::LiteralNode>(&node->data);
  ASSERT_EQ(*literal_node->value, *Lisple::Constant::NIL);
}
