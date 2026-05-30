
#include "roo/exception.h"
#include "roo/exec.h"
#include "roo/reader.h"
#include "roo/runtime/node.h"
#include "roo/type.h"

#include <roo/bind.h>
#include <roo/context.h>
#include <roo/form.h>
#include <roo/runtime/exec_node.h>
#include <roo/runtime/lower.h>

#include "runtime_fixture.h"
#include <gtest/gtest.h>

using LowerTest = RooTest::RuntimeTestFixture;
TEST_F(LowerTest, number__literal_node)
{
  // Given
  Roo::LowerContext lctx;
  Roo::sptr_ast_node number = Roo::AST::Number::make(4);

  // When
  auto node = Roo::lower_expr(lctx, number);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::LiteralNode>(node->data));
}

TEST_F(LowerTest, list__call_node__no_ctx__lookup_node_callee)
{
  Roo::LowerContext lctx;
  Roo::sptr_ast_node expr = Roo::AST::List::make(
    {Roo::AST::Symbol::make("+"), Roo::AST::Number::make(1), Roo::AST::Number::make(1)});

  // When
  auto node = Roo::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::CallNode>(node->data));
  Roo::CallNode& call_node = std::get<Roo::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Roo::LookupNode>(call_node.callee->data));
}

TEST_F(LowerTest, list__call_node__ctx__literal_node_callee)
{
  Roo::LowerContext lctx{&ctx};
  Roo::sptr_ast_node expr = Roo::AST::List::make(
    {Roo::AST::Symbol::make("+"), Roo::AST::Number::make(1), Roo::AST::Number::make(1)});

  // When
  auto node = Roo::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::CallNode>(node->data));
  Roo::CallNode& call_node = std::get<Roo::CallNode>(node->data);
  ASSERT_TRUE(std::holds_alternative<Roo::LiteralNode>(call_node.callee->data));
}

TEST_F(LowerTest, list__key_lookup__no_ctx__lookup_node)
{
  Roo::LowerContext lctx;
  Roo::sptr_ast_node expr = Roo::AST::List::make(
    {Roo::AST::Keyword::make("name"), Roo::AST::Symbol::make("my-map")});

  // When
  auto node = Roo::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::KeyLookupNode>(node->data));
  Roo::KeyLookupNode& key_node = std::get<Roo::KeyLookupNode>(node->data);
  EXPECT_EQ(*key_node.keyword, *Roo::Value::keyword("name"));
  EXPECT_TRUE(std::holds_alternative<Roo::LookupNode>(key_node.target->data));
}

TEST_F(LowerTest, list__key_lookup__ctx__lookup_node)
{
  Roo::LowerContext lctx{&ctx};
  Roo::sptr_ast_node expr = Roo::AST::List::make(
    {Roo::AST::Keyword::make("name"), Roo::AST::Symbol::make("my-map")});

  // When
  auto node = Roo::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::KeyLookupNode>(node->data));
  Roo::KeyLookupNode& key_node = std::get<Roo::KeyLookupNode>(node->data);
  EXPECT_EQ(*key_node.keyword, *Roo::Value::keyword("name"));
  EXPECT_TRUE(std::holds_alternative<Roo::LookupNode>(key_node.target->data));
}

TEST_F(LowerTest, list_special_form__ctx__custom_lower)
{
  Roo::LowerContext lctx{&ctx};
  Roo::sptr_ast_node expr = Roo::AST::List::make(
    {Roo::AST::Symbol::make("->"),
     Roo::AST::Number::make(10),
     Roo::AST::List::make({Roo::AST::Symbol::make("+"), Roo::AST::Number::make(5)}),
     Roo::AST::Symbol::make("range")});

  // When
  auto node = Roo::lower_expr(lctx, expr);

  // Then
  ASSERT_TRUE(std::holds_alternative<Roo::CallNode>(node->data));
}

SPECIAL_FORM_DECL(DefThing, def_thing)

SPECIAL_FORM_IMPL(DefThing,
                  SIG((FN_ARGS((&Roo::Type::SYMBOL), (&Roo::Type::MAP)),
                       EXEC_DISPATCH(&DefThing::execnode_def_thing))));

SFORM_LOWER_IMPL(DefThing)
{
  return std::make_unique<Roo::ExecNode>(Roo::Constant::NIL);
}

EXECNODE_BODY(DefThing, execnode_def_thing)
{
  throw Roo::RooException("execnode_def_thing should not be called");
}

class CustomNamespace : public Roo::Namespace
{
 public:
  CustomNamespace()
    : Roo::Namespace("custom")
  {
    values.emplace("defthing", DefThing::make());
  }
};

TEST_F(LowerTest, lower_custom_special_form)
{
  // Given
  Roo::Reader reader;
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<CustomNamespace>());
  auto& runtime = use_runtime_with(std::move(namespaces), nullptr);
  runtime.eval("(ns my-app.core (:require custom))");
  auto& ctx = *configured_context;
  ctx.switch_namespace("my-app.core");
  Roo::LowerContext lctx{&ctx};
  Roo::sptr_ast_node_v def_thing_expr =
    reader.read_sexps("(defthing my-thing {:value \"a lot\"})");

  // When
  auto node = Roo::lower_expr(lctx, def_thing_expr.front());

  // Then
  auto* literal_node = std::get_if<Roo::LiteralNode>(&node->data);
  ASSERT_EQ(*literal_node->value, *Roo::Constant::NIL);
}

TEST_F(LowerTest, lower_namespace_qualified_custom_special_form)
{
  // Given
  Roo::Reader reader;
  std::vector<std::unique_ptr<Roo::Namespace>> namespaces;
  namespaces.push_back(std::make_unique<CustomNamespace>());
  use_runtime_with(std::move(namespaces), nullptr);
  auto& ctx = *configured_context;
  ctx.switch_namespace("my-app.core");
  Roo::LowerContext lctx{&ctx};
  Roo::sptr_ast_node_v def_thing_expr =
    reader.read_sexps("(custom/defthing my-thing {:value \"a lot\"})");

  // When
  Roo::uptr_exec_node node = Roo::lower_expr(lctx, def_thing_expr.front());

  // Then
  auto* literal_node = std::get_if<Roo::LiteralNode>(&node->data);
  ASSERT_EQ(*literal_node->value, *Roo::Constant::NIL);
}
