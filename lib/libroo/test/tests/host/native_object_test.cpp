
#include <optional>
#include <string>
#include <vector>

#include "host/test_adapters/optional_native_adapters.h"
#include "host/test_adapters/product_native_adapters.h"
#include "host/test_adapters/vehicle_impl.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest.h>
#include <roo/context.h>
#include <roo/form.h>
#include <roo/host/accessor.h>
#include <roo/host/object.h>
#include <roo/lang.h>
#include <roo/namespace.h>
#include <roo/reader.h>
#include <roo/runtime.h>
#include <roo/runtime/dict.h>
#include <roo/type.h>

namespace
{
  struct ByteBox
  {
    uint8_t value = 0;
  };

  inline Roo::HostTypeRef BYTE_BOX("ByteBox");

  NATIVE_ADAPTER(ByteBoxAdapter, ByteBox, (value), (value));

  NATIVE_ADAPTER_IMPL(ByteBoxAdapter, ByteBox, &BYTE_BOX, (rw, "value", value));

  NOBJ_PROP_GET_SET__FIELD(ByteBoxAdapter, value);

  struct KeywordBox
  {
    std::string kind = "field-kind";
    std::optional<std::string> optional_kind = std::nullopt;
    std::optional<std::string> optional_mode = std::nullopt;

    std::string get_mode() { return "method-mode"; }
    std::optional<std::string> get_optional_mode() { return optional_mode; }
    void set_optional_mode(const std::optional<std::string>& mode) { optional_mode = mode; }
  };

  inline Roo::HostTypeRef KEYWORD_BOX("KeywordBox");

  NATIVE_ADAPTER(KeywordBoxAdapter,
                 KeywordBox,
                 (kind, mode, optional_kind, optional_mode),
                 (kind, optional_kind, optional_mode));

  NATIVE_ADAPTER_IMPL(KeywordBoxAdapter,
                      KeywordBox,
                      &KEYWORD_BOX,
                      ("kind", kind),
                      ("mode", mode),
                      (rw, "optional-kind", optional_kind),
                      ("optional-mode", optional_mode));

  NOBJ_PROP_GET_SET_KEYWORD__FIELD(KeywordBoxAdapter, kind);
  NOBJ_PROP_GET_KEYWORD__METHOD(KeywordBoxAdapter, mode);
  NOBJ_PROP_GET_SET_OPT_KEYWORD__FIELD(KeywordBoxAdapter, optional_kind);
  NOBJ_PROP_GET_SET_OPT_KEYWORD__METHOD(KeywordBoxAdapter, optional_mode);

  struct VectorBox
  {
    std::vector<int> values{1, 2, 3};
    std::vector<std::string> keyword_values{"first", "second"};
    std::vector<RooTest::OptionalChild> children{{5}, {8}};
    std::vector<RooTest::OptionalChild*> child_ptrs{};

    std::vector<int>& values_by_method() { return values; }
    std::vector<RooTest::OptionalChild>& children_by_method() { return children; }
    std::vector<RooTest::OptionalChild*>& child_ptrs_by_method() { return child_ptrs; }
  };

  inline Roo::HostTypeRef VECTOR_BOX("VectorBox");

  NATIVE_ADAPTER(VectorBoxAdapter,
                 VectorBox,
                 (values,
                  keyword_values,
                  children,
                  child_ptrs,
                  method_values,
                  method_children,
                  method_child_ptrs),
                 (keyword_values));

  NATIVE_ADAPTER_IMPL(VectorBoxAdapter,
                      VectorBox,
                      &VECTOR_BOX,
                      ("values", values),
                      (rw, "keyword-values", keyword_values),
                      ("children", children),
                      ("child-ptrs", child_ptrs),
                      ("method-values", method_values),
                      ("method-children", method_children),
                      ("method-child-ptrs", method_child_ptrs));

  ADAPTER_PROP_GET(VectorBoxAdapter, FIELD(values), VECTOR_COPY(VALUE));
  NOBJ_PROP_GET_SET_KEYWORD_VECTOR_COPY__FIELD(VectorBoxAdapter,
                                               keyword_values,
                                               Roo::AST::Keyword);
  ADAPTER_PROP_GET(VectorBoxAdapter,
                   FIELD(children),
                   VECTOR_COPY(ADAPTER(RooTest::Native::OptionalChildAdapter)));
  ADAPTER_PROP_GET(VectorBoxAdapter,
                   FIELD(child_ptrs),
                   VECTOR_COPY(ADAPTER_P(RooTest::Native::OptionalChildAdapter)));
  NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD(VectorBoxAdapter, method_values, values_by_method);
  NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD(VectorBoxAdapter,
                                            method_children,
                                            RooTest::Native::OptionalChildAdapter,
                                            children_by_method);
  NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD(VectorBoxAdapter,
                                              method_child_ptrs,
                                              RooTest::Native::OptionalChildAdapter,
                                              child_ptrs_by_method);
} // namespace

TEST(NativeObjectAdapter_VehicleModel, type)
{
  // Given
  Roo::sptr_val val =
    RooTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  RooTest::Native::VehicleModelAdapter& adapter =
    val->adapter<RooTest::Native::VehicleModelAdapter>();

  // Then
  EXPECT_EQ(val->type, Roo::Value::Type::NATIVE_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &RooTest::VEHICLE_MODEL_TYPE);

  EXPECT_TRUE(Roo::Type::NATIVE_OBJECT.is_type_of(*val));
  EXPECT_TRUE(RooTest::VEHICLE_MODEL_TYPE.is_type_of(*val));

  EXPECT_FALSE(Roo::Type::VECTOR.is_type_of(*val));
  EXPECT_FALSE(Roo::Type::SEQ.is_type_of(*val));
  EXPECT_FALSE(Roo::Type::MAP.is_type_of(*val));
  EXPECT_FALSE(RooTest::Native::PRODUCT.is_type_of(*val));
}

TEST(NativeObjectAdapter_PrimitiveField, get_set_uint8_field)
{
  // Given
  Roo::sptr_val val = ByteBoxAdapter::make_unique();
  ByteBoxAdapter& adapter = val->adapter<ByteBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_value(), *Roo::Value::number(0));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("value")),
            *Roo::Value::number(0));

  // When
  auto value = Roo::Value::number(255);
  adapter.set_value(nullptr, value);

  // Then
  EXPECT_EQ(adapter.get_object().value, 255);
  EXPECT_EQ(*adapter.get_value(), *Roo::Value::number(255));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("value")),
            *Roo::Value::number(255));
}

TEST(NativeObjectAdapter_KeywordGetter, get_keyword_field)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_kind(), *Roo::Value::keyword("field-kind"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("kind")),
            *Roo::Value::keyword("field-kind"));
}

TEST(NativeObjectAdapter_KeywordGetter, set_keyword_field)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // When
  auto kind = Roo::Value::keyword("updated-kind");
  adapter.set_kind(nullptr, kind);

  // Then
  EXPECT_EQ(adapter.get_object().kind, "updated-kind");
  EXPECT_EQ(*adapter.get_kind(), *Roo::Value::keyword("updated-kind"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("kind")),
            *Roo::Value::keyword("updated-kind"));
}

TEST(NativeObjectAdapter_KeywordGetter, get_keyword_method)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_mode(), *Roo::Value::keyword("method-mode"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("mode")),
            *Roo::Value::keyword("method-mode"));
}

TEST(NativeObjectAdapter_KeywordGetter, get_set_optional_keyword_field)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_optional_kind(), *Roo::Constant::NIL);
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("optional-kind")),
            *Roo::Constant::NIL);

  // When
  auto kind = Roo::Value::keyword("field-kind");
  adapter.set_optional_kind(nullptr, kind);

  // Then
  ASSERT_TRUE(adapter.get_object().optional_kind.has_value());
  EXPECT_EQ(*adapter.get_object().optional_kind, "field-kind");
  EXPECT_EQ(*adapter.get_optional_kind(), *Roo::Value::keyword("field-kind"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("optional-kind")),
            *Roo::Value::keyword("field-kind"));

  // When
  auto nil = Roo::Constant::NIL;
  adapter.set_optional_kind(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().optional_kind.has_value());
  EXPECT_EQ(*adapter.get_optional_kind(), *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_KeywordGetter, get_optional_keyword_method)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_optional_mode(), *Roo::Constant::NIL);
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("optional-mode")),
            *Roo::Constant::NIL);

  // When
  adapter.get_object().optional_mode = "method-mode";

  // Then
  EXPECT_EQ(*adapter.get_optional_mode(), *Roo::Value::keyword("method-mode"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("optional-mode")),
            *Roo::Value::keyword("method-mode"));
}

TEST(NativeObjectAdapter_KeywordGetter, set_optional_keyword_method)
{
  // Given
  Roo::sptr_val val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // When
  auto mode = Roo::Value::keyword("method-mode");
  adapter.set_optional_mode(nullptr, mode);

  // Then
  ASSERT_TRUE(adapter.get_object().optional_mode.has_value());
  EXPECT_EQ(*adapter.get_object().optional_mode, "method-mode");
  EXPECT_EQ(*adapter.get_optional_mode(), *Roo::Value::keyword("method-mode"));

  // When
  auto nil = Roo::Constant::NIL;
  adapter.set_optional_mode(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().optional_mode.has_value());
  EXPECT_EQ(*adapter.get_optional_mode(), *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_Product, set_price)
{
  // Given
  Roo::sptr_val val = RooTest::Native::ProductAdapter::make_unique("A", 10.0f, 1);
  RooTest::Native::ProductAdapter& adapter = val->adapter<RooTest::Native::ProductAdapter>();

  // Then
  EXPECT_FLOAT_EQ(adapter.get_object().get_price(), 10.0f);
  EXPECT_EQ(*adapter.get_price(), *Roo::Value::number(10.0));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("price")),
            *Roo::Value::number(10.0));

  // When
  auto price = Roo::Value::number(99.5);
  adapter.set_price(nullptr, price);

  // Then
  EXPECT_FLOAT_EQ(adapter.get_object().get_price(), 99.5f);
  EXPECT_EQ(*adapter.get_price(), *Roo::Value::number(99.5));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("price")),
            *Roo::Value::number(99.5));
}

TEST(NativeObjectAdapter_Product, set_sku)
{
  // Given
  Roo::sptr_val val = RooTest::Native::ProductAdapter::make_unique("A", 10.0f, 1);
  RooTest::Native::ProductAdapter& adapter = val->adapter<RooTest::Native::ProductAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_sku(), *Roo::Value::number(1));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("sku")),
            *Roo::Value::number(1));

  // When
  auto sku = Roo::Value::number(7);
  adapter.set_sku(nullptr, sku);

  // Then
  EXPECT_EQ(*adapter.get_sku(), *Roo::Value::number(7));
  EXPECT_EQ(adapter.get_object().get_sku(), 7);
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("sku")),
            *Roo::Value::number(7));
}

TEST(NativeObjectAdapter_OptionalField, get_set_optional_field)
{
  // Given
  Roo::sptr_val val = RooTest::Native::OptionalBoxAdapter::make_unique();
  RooTest::Native::OptionalBoxAdapter& adapter =
    val->adapter<RooTest::Native::OptionalBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_count(), *Roo::Constant::NIL);
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("count")),
            *Roo::Constant::NIL);

  // When
  auto count = Roo::Value::number(7);
  adapter.set_count(nullptr, count);

  // Then
  ASSERT_TRUE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_object().count, 7);
  EXPECT_EQ(*adapter.get_count(), *Roo::Value::number(7));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("count")),
            *Roo::Value::number(7));

  // When
  auto nil = Roo::Constant::NIL;
  adapter.set_count(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_count(), *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_OptionalAdapterField, get_set_optional_adapter_field)
{
  // Given
  Roo::sptr_val val = RooTest::Native::OptionalChildBoxAdapter::make_unique();
  RooTest::Native::OptionalChildBoxAdapter& adapter =
    val->adapter<RooTest::Native::OptionalChildBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Roo::Constant::NIL);
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("child")),
            *Roo::Constant::NIL);

  // When
  auto child = RooTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Roo::Value::number(42);
  child->adapter<RooTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_TRUE(adapter.get_object().child.has_value());
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(
    adapter.get_child()->adapter<RooTest::Native::OptionalChildAdapter>().get_object().value,
    42);

  // When
  auto nil = Roo::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().child.has_value());
  EXPECT_EQ(*adapter.get_child(), *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_PointerAdapterField, get_set_pointer_adapter_field)
{
  // Given
  Roo::sptr_val child = RooTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Roo::Value::number(42);
  child->adapter<RooTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);

  Roo::sptr_val val = RooTest::Native::OptionalChildPtrBoxAdapter::make_unique();
  RooTest::Native::OptionalChildPtrBoxAdapter& adapter =
    val->adapter<RooTest::Native::OptionalChildPtrBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Roo::Constant::NIL);

  // When
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_NE(adapter.get_object().child, nullptr);
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(
    adapter.get_child()->adapter<RooTest::Native::OptionalChildAdapter>().get_object().value,
    42);

  // When
  auto nil = Roo::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_EQ(adapter.get_object().child, nullptr);
  EXPECT_EQ(*adapter.get_child(), *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_VectorCopyField, get_value_vector_copy_field)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Roo::sptr_val values = adapter.get_values();
  values->mut_elements().push_back(Roo::Value::number(99));

  // Then
  ASSERT_EQ(values->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(values->elements().size(), 4);
  EXPECT_EQ(values->elements().at(0)->i64(), 1);
  EXPECT_EQ(values->elements().at(1)->i64(), 2);
  EXPECT_EQ(values->elements().at(2)->i64(), 3);
  EXPECT_EQ(values->elements().at(3)->i64(), 99);
  EXPECT_EQ(adapter.get_object().values.size(), 3);
}

TEST(NativeObjectAdapter_VectorCopyField, get_set_keyword_vector_copy_field)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Roo::sptr_val values = adapter.get_keyword_values();
  values->mut_elements().push_back(Roo::Value::keyword("extra"));

  // Then
  ASSERT_EQ(values->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(values->elements().size(), 3);
  EXPECT_EQ(*values->elements().at(0), *Roo::Value::keyword("first"));
  EXPECT_EQ(*values->elements().at(1), *Roo::Value::keyword("second"));
  EXPECT_EQ(*values->elements().at(2), *Roo::Value::keyword("extra"));
  EXPECT_EQ(adapter.get_object().keyword_values.size(), 2);

  // When
  auto updated =
    Roo::Value::vector({Roo::Value::keyword("third"), Roo::Value::string("fourth")});
  adapter.set_keyword_values(nullptr, updated);

  // Then
  ASSERT_EQ(adapter.get_object().keyword_values.size(), 2);
  EXPECT_EQ(adapter.get_object().keyword_values.at(0), "third");
  EXPECT_EQ(adapter.get_object().keyword_values.at(1), "fourth");
  EXPECT_EQ(*adapter.get_keyword_values()->elements().at(0), *Roo::Value::keyword("third"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("keyword-values")),
            *adapter.get_keyword_values());
}

TEST(NativeObjectAdapter_VectorCopyField, get_adapter_vector_copy_field)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Roo::sptr_val children = adapter.get_children();
  children->mut_elements().push_back(Roo::Constant::NIL);

  // Then
  ASSERT_EQ(children->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(children->elements().size(), 3);
  EXPECT_EQ(children->elements()
              .at(0)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(children->elements()
              .at(1)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            8);
  EXPECT_EQ(*children->elements().at(2), *Roo::Constant::NIL);
  EXPECT_EQ(adapter.get_object().children.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyField, get_pointer_adapter_vector_copy_field)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();
  adapter.get_object().child_ptrs.push_back(&adapter.get_object().children.at(0));
  adapter.get_object().child_ptrs.push_back(nullptr);

  // When
  Roo::sptr_val child_ptrs = adapter.get_child_ptrs();
  child_ptrs->mut_elements().push_back(Roo::Constant::NIL);

  // Then
  ASSERT_EQ(child_ptrs->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(child_ptrs->elements().size(), 3);
  EXPECT_EQ(child_ptrs->elements()
              .at(0)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(*child_ptrs->elements().at(1), *Roo::Constant::NIL);
  EXPECT_EQ(*child_ptrs->elements().at(2), *Roo::Constant::NIL);
  EXPECT_EQ(adapter.get_object().child_ptrs.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyMethod, get_value_vector_copy_method)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Roo::sptr_val values = adapter.get_method_values();
  values->mut_elements().push_back(Roo::Value::number(99));

  // Then
  ASSERT_EQ(values->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(values->elements().size(), 4);
  EXPECT_EQ(values->elements().at(0)->i64(), 1);
  EXPECT_EQ(values->elements().at(1)->i64(), 2);
  EXPECT_EQ(values->elements().at(2)->i64(), 3);
  EXPECT_EQ(values->elements().at(3)->i64(), 99);
  EXPECT_EQ(adapter.get_object().values.size(), 3);
}

TEST(NativeObjectAdapter_VectorCopyMethod, get_adapter_vector_copy_method)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Roo::sptr_val children = adapter.get_method_children();
  children->mut_elements().push_back(Roo::Constant::NIL);

  // Then
  ASSERT_EQ(children->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(children->elements().size(), 3);
  EXPECT_EQ(children->elements()
              .at(0)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(children->elements()
              .at(1)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            8);
  EXPECT_EQ(*children->elements().at(2), *Roo::Constant::NIL);
  EXPECT_EQ(adapter.get_object().children.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyMethod, get_pointer_adapter_vector_copy_method)
{
  // Given
  Roo::sptr_val val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();
  adapter.get_object().child_ptrs.push_back(&adapter.get_object().children.at(0));
  adapter.get_object().child_ptrs.push_back(nullptr);

  // When
  Roo::sptr_val child_ptrs = adapter.get_method_child_ptrs();
  child_ptrs->mut_elements().push_back(Roo::Constant::NIL);

  // Then
  ASSERT_EQ(child_ptrs->type, Roo::Value::Type::VECTOR);
  ASSERT_EQ(child_ptrs->elements().size(), 3);
  EXPECT_EQ(child_ptrs->elements()
              .at(0)
              ->adapter<RooTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(*child_ptrs->elements().at(1), *Roo::Constant::NIL);
  EXPECT_EQ(*child_ptrs->elements().at(2), *Roo::Constant::NIL);
  EXPECT_EQ(adapter.get_object().child_ptrs.size(), 2);
}

TEST(NativeObjectAdapter_VehicleModel, get_property)
{
  // Given
  Roo::sptr_val val =
    RooTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  RooTest::Native::VehicleModelAdapter& adapter =
    val->adapter<RooTest::Native::VehicleModelAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_model_name(), *Roo::Value::string("Runaway Train"));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("model-name")),
            *Roo::Value::string("Runaway Train"));

  EXPECT_EQ(*adapter.get_seats(), *Roo::Value::number(500));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("seats")),
            *Roo::Value::number(500));

  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("does-not-exist")),
            *Roo::Constant::NIL);
}

TEST(NativeObjectAdapter_VehicleModel, set_property)
{
  // Given
  Roo::sptr_val val =
    RooTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  RooTest::Native::VehicleModelAdapter& adapter =
    val->adapter<RooTest::Native::VehicleModelAdapter>();

  RooTest::VehicleModel& vehicle = adapter.get_object();
  EXPECT_EQ(vehicle.get_model_name(), "Runaway Train");
  EXPECT_EQ(vehicle.get_seats(), 500);

  // When
  vehicle.set_seats(800);

  // Then
  EXPECT_EQ(*adapter.get_seats(), *Roo::Value::number(800));
  EXPECT_EQ(*Roo::Dict::get_property(val, Roo::Value::keyword("seats")),
            *Roo::Value::number(800));
  EXPECT_EQ(vehicle.get_seats(), 800);
}
