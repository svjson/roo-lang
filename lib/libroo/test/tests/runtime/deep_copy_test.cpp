#include <roo/exception.h>
#include <roo/form.h>
#include <roo/host/object.h>
#include <roo/host/type.h>
#include <roo/lang/func.h>
#include <roo/lang/num.h>
#include <roo/runtime/deep_copy.h>

#include <gtest/gtest.h>

namespace
{
  const Roo::HostTypeRef NATIVE_GRAPH_VALUE_TYPE("test/native-graph-value");

  class NativeGraphValue final : public Roo::NativeObjectBase
  {
   private:
    Roo::NativeObjectStructuralKind kind;
    Roo::sptr_val_v children;

   public:
    NativeGraphValue(Roo::NativeObjectStructuralKind kind,
                     Roo::sptr_val_v children)
      : kind(kind)
      , children(std::move(children))
    {
    }

    const Roo::NativeObjectTraits* get_traits() const override
    {
      static const Roo::NAccessorTable accessors;
      static const Roo::NativeObjectTraits traits(&NATIVE_GRAPH_VALUE_TYPE, accessors);
      return &traits;
    }

    Roo::sptr_val_v native_children() const override { return children; }

    Roo::NativeObjectStructuralKind structural_kind() const override { return kind; }

    void* self_object_ptr() const override
    {
      return const_cast<NativeGraphValue*>(this);
    }
  };

  Roo::sptr_val native_value(Roo::NativeObjectStructuralKind kind,
                             Roo::sptr_val_v children)
  {
    return Roo::Value::native_object(
      std::make_shared<NativeGraphValue>(kind, std::move(children)));
  }
} // namespace

TEST(DeepCopy, rebuilds_roo_collections_and_preserves_aliases_across_roots)
{
  Roo::sptr_val shared = Roo::Value::map(
    {Roo::Value::keyword("answer"), Roo::Value::list({Roo::Value::number(42)})});
  Roo::sptr_val outer = Roo::Value::vector({shared, shared});

  Roo::sptr_val_v copied = Roo::deep_copy(Roo::sptr_val_v{outer, shared});

  ASSERT_EQ(copied.size(), 2);
  ASSERT_NE(copied[0], outer);
  ASSERT_EQ(copied[0]->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(copied[0]->elements().size(), 2);
  EXPECT_EQ(copied[0]->elements()[0], copied[0]->elements()[1]);
  EXPECT_EQ(copied[0]->elements()[0], copied[1]);
  EXPECT_NE(copied[1], shared);
  ASSERT_EQ(copied[1]->type, Roo::Value::Type::MAP);
  EXPECT_EQ(copied[1]->elements()[0], shared->elements()[0]);
  EXPECT_NE(copied[1]->elements()[1], shared->elements()[1]);
  EXPECT_EQ(copied[1]->elements()[1]->type, Roo::Value::Type::LIST);
}

TEST(DeepCopy, shares_atomic_and_opaque_leaves)
{
  Roo::sptr_val scalar = Roo::Value::string("shared");
  Roo::sptr_val native_function = Roo::IncFunction::make();
  Roo::sptr_ast_node ast = Roo::AST::Number::make(7);
  Roo::sptr_val ast_value = Roo::Value::object(ast);
  Roo::sptr_val source = Roo::Value::vector({scalar, native_function, ast_value});

  Roo::sptr_val copied = Roo::deep_copy(source);

  ASSERT_NE(copied, source);
  EXPECT_EQ(copied->elements()[0], scalar);
  EXPECT_EQ(copied->elements()[1], native_function);
  EXPECT_EQ(copied->elements()[2], ast_value);
}

TEST(DeepCopy, demotes_native_maps_and_sequences_to_roo_shapes)
{
  Roo::sptr_val native_sequence =
    native_value(Roo::NativeObjectStructuralKind::VECTOR,
                 {Roo::Value::number(1), Roo::Value::number(2)});
  Roo::sptr_val native_map =
    native_value(Roo::NativeObjectStructuralKind::MAP,
                 {Roo::Value::keyword("items"), native_sequence});

  Roo::sptr_val copied = Roo::deep_copy(native_map);

  ASSERT_EQ(copied->type, Roo::Value::Type::MAP);
  ASSERT_EQ(copied->elements().size(), 2);
  EXPECT_EQ(copied->elements()[0], native_map->nobj()->native_children()[0]);
  ASSERT_EQ(copied->elements()[1]->type, Roo::Value::Type::VECTOR);
  EXPECT_NE(copied->elements()[1], native_sequence);
  EXPECT_EQ(copied->elements()[1]->elements()[0],
            native_sequence->nobj()->native_children()[0]);
}

TEST(DeepCopy, rejects_cycles)
{
  Roo::sptr_val cyclic = Roo::Value::vector({});
  cyclic->mut_elements().push_back(cyclic);

  EXPECT_THROW(Roo::deep_copy(cyclic), Roo::RooException);
}

TEST(DeepCopy, rejects_native_maps_with_an_unmatched_key)
{
  Roo::sptr_val malformed = native_value(Roo::NativeObjectStructuralKind::MAP,
                                         {Roo::Value::keyword("unmatched")});

  EXPECT_THROW(Roo::deep_copy(malformed), Roo::RooException);
}

TEST(RuntimeTransferDeepCopy, shares_stateless_native_functions)
{
  Roo::sptr_val native_function = Roo::IncFunction::make();

  EXPECT_EQ(Roo::deep_copy_for_runtime_transfer(native_function), native_function);
}

TEST(RuntimeTransferDeepCopy, rejects_stateful_functions_and_ast_values)
{
  Roo::sptr_val native_function = Roo::IncFunction::make();
  Roo::sptr_val stateful_function = Roo::Value::executable(
    std::make_shared<Roo::JuxtedFunction>(Roo::sptr_val_v{native_function}));
  Roo::sptr_ast_node ast = Roo::AST::Number::make(7);
  Roo::sptr_val ast_value = Roo::Value::object(ast);

  EXPECT_THROW(Roo::deep_copy_for_runtime_transfer(stateful_function), Roo::RooException);
  EXPECT_THROW(Roo::deep_copy_for_runtime_transfer(ast_value), Roo::RooException);
}
