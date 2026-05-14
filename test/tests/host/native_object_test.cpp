
#include <optional>
#include <string>
#include <vector>

#include <lisple/context.h>
#include <lisple/form.h>
#include <lisple/host/accessor.h>
#include <lisple/host/object.h>
#include <lisple/lang.h>
#include <lisple/namespace.h>
#include <lisple/reader.h>
#include <lisple/runtime.h>
#include <lisple/runtime/dict.h>
#include <lisple/type.h>

#include "host/test_adapters/optional_native_adapters.h"
#include "host/test_adapters/product_native_adapters.h"
#include "host/test_adapters/vehicle_impl.h"
#include "host/test_adapters/vehicle_native_adapters.h"
#include <gtest/gtest.h>

namespace
{
  struct ByteBox
  {
    uint8_t value = 0;
  };

  inline Lisple::HostTypeRef BYTE_BOX("ByteBox");

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

  inline Lisple::HostTypeRef KEYWORD_BOX("KeywordBox");

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
    std::vector<LispleTest::OptionalChild> children{{5}, {8}};
    std::vector<LispleTest::OptionalChild*> child_ptrs{};

    std::vector<int>& values_by_method() { return values; }
    std::vector<LispleTest::OptionalChild>& children_by_method() { return children; }
    std::vector<LispleTest::OptionalChild*>& child_ptrs_by_method() { return child_ptrs; }
  };

  inline Lisple::HostTypeRef VECTOR_BOX("VectorBox");

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
                                               Lisple::Key);
  ADAPTER_PROP_GET(VectorBoxAdapter,
                   FIELD(children),
                   VECTOR_COPY(ADAPTER(LispleTest::Native::OptionalChildAdapter)));
  ADAPTER_PROP_GET(VectorBoxAdapter,
                   FIELD(child_ptrs),
                   VECTOR_COPY(ADAPTER_P(LispleTest::Native::OptionalChildAdapter)));
  NOBJ_PROP_GET_VALUE_VECTOR_COPY__METHOD(VectorBoxAdapter, method_values, values_by_method);
  NOBJ_PROP_GET_ADAPTER_VECTOR_COPY__METHOD(VectorBoxAdapter,
                                            method_children,
                                            LispleTest::Native::OptionalChildAdapter,
                                            children_by_method);
  NOBJ_PROP_GET_ADAPTER_P_VECTOR_COPY__METHOD(VectorBoxAdapter,
                                              method_child_ptrs,
                                              LispleTest::Native::OptionalChildAdapter,
                                              child_ptrs_by_method);
} // namespace

TEST(NativeObjectAdapter_VehicleModel, type)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

  // Then
  EXPECT_EQ(val->type, Lisple::RTValue::Type::NATIVE_OBJECT);
  EXPECT_EQ(adapter.get_host_type(), &LispleTest::VEHICLE_MODEL_TYPE);

  EXPECT_TRUE(Lisple::Type::NATIVE_OBJECT.is_type_of(*val));
  EXPECT_TRUE(LispleTest::VEHICLE_MODEL_TYPE.is_type_of(*val));

  EXPECT_FALSE(Lisple::Type::ARRAY.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::SEQ.is_type_of(*val));
  EXPECT_FALSE(Lisple::Type::MAP.is_type_of(*val));
  EXPECT_FALSE(LispleTest::Native::PRODUCT.is_type_of(*val));
}

TEST(NativeObjectAdapter_PrimitiveField, get_set_uint8_field)
{
  // Given
  Lisple::sptr_rtval val = ByteBoxAdapter::make_unique();
  ByteBoxAdapter& adapter = val->adapter<ByteBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_value(), *Lisple::RTValue::number(0));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("value")),
            *Lisple::RTValue::number(0));

  // When
  auto value = Lisple::RTValue::number(255);
  adapter.set_value(nullptr, value);

  // Then
  EXPECT_EQ(adapter.get_object().value, 255);
  EXPECT_EQ(*adapter.get_value(), *Lisple::RTValue::number(255));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("value")),
            *Lisple::RTValue::number(255));
}

TEST(NativeObjectAdapter_KeywordGetter, get_keyword_field)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_kind(), *Lisple::RTValue::keyword("field-kind"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("kind")),
            *Lisple::RTValue::keyword("field-kind"));
}

TEST(NativeObjectAdapter_KeywordGetter, set_keyword_field)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // When
  auto kind = Lisple::RTValue::keyword("updated-kind");
  adapter.set_kind(nullptr, kind);

  // Then
  EXPECT_EQ(adapter.get_object().kind, "updated-kind");
  EXPECT_EQ(*adapter.get_kind(), *Lisple::RTValue::keyword("updated-kind"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("kind")),
            *Lisple::RTValue::keyword("updated-kind"));
}

TEST(NativeObjectAdapter_KeywordGetter, get_keyword_method)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_mode(), *Lisple::RTValue::keyword("method-mode"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("mode")),
            *Lisple::RTValue::keyword("method-mode"));
}

TEST(NativeObjectAdapter_KeywordGetter, get_set_optional_keyword_field)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_optional_kind(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("optional-kind")),
            *Lisple::Constant::NIL);

  // When
  auto kind = Lisple::RTValue::keyword("field-kind");
  adapter.set_optional_kind(nullptr, kind);

  // Then
  ASSERT_TRUE(adapter.get_object().optional_kind.has_value());
  EXPECT_EQ(*adapter.get_object().optional_kind, "field-kind");
  EXPECT_EQ(*adapter.get_optional_kind(), *Lisple::RTValue::keyword("field-kind"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("optional-kind")),
            *Lisple::RTValue::keyword("field-kind"));

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_optional_kind(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().optional_kind.has_value());
  EXPECT_EQ(*adapter.get_optional_kind(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_KeywordGetter, get_optional_keyword_method)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_optional_mode(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("optional-mode")),
            *Lisple::Constant::NIL);

  // When
  adapter.get_object().optional_mode = "method-mode";

  // Then
  EXPECT_EQ(*adapter.get_optional_mode(), *Lisple::RTValue::keyword("method-mode"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("optional-mode")),
            *Lisple::RTValue::keyword("method-mode"));
}

TEST(NativeObjectAdapter_KeywordGetter, set_optional_keyword_method)
{
  // Given
  Lisple::sptr_rtval val = KeywordBoxAdapter::make_unique();
  KeywordBoxAdapter& adapter = val->adapter<KeywordBoxAdapter>();

  // When
  auto mode = Lisple::RTValue::keyword("method-mode");
  adapter.set_optional_mode(nullptr, mode);

  // Then
  ASSERT_TRUE(adapter.get_object().optional_mode.has_value());
  EXPECT_EQ(*adapter.get_object().optional_mode, "method-mode");
  EXPECT_EQ(*adapter.get_optional_mode(), *Lisple::RTValue::keyword("method-mode"));

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_optional_mode(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().optional_mode.has_value());
  EXPECT_EQ(*adapter.get_optional_mode(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_OptionalField, get_set_optional_field)
{
  // Given
  Lisple::sptr_rtval val = LispleTest::Native::OptionalBoxAdapter::make_unique();
  LispleTest::Native::OptionalBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_count(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("count")),
            *Lisple::Constant::NIL);

  // When
  auto count = Lisple::RTValue::number(7);
  adapter.set_count(nullptr, count);

  // Then
  ASSERT_TRUE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_object().count, 7);
  EXPECT_EQ(*adapter.get_count(), *Lisple::RTValue::number(7));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("count")),
            *Lisple::RTValue::number(7));

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_count(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().count.has_value());
  EXPECT_EQ(*adapter.get_count(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_OptionalAdapterField, get_set_optional_adapter_field)
{
  // Given
  Lisple::sptr_rtval val = LispleTest::Native::OptionalChildBoxAdapter::make_unique();
  LispleTest::Native::OptionalChildBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalChildBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("child")),
            *Lisple::Constant::NIL);

  // When
  auto child = LispleTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Lisple::RTValue::number(42);
  child->adapter<LispleTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_TRUE(adapter.get_object().child.has_value());
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(adapter.get_child()
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            42);

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_FALSE(adapter.get_object().child.has_value());
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_PointerAdapterField, get_set_pointer_adapter_field)
{
  // Given
  Lisple::sptr_rtval child = LispleTest::Native::OptionalChildAdapter::make_unique();
  auto child_value = Lisple::RTValue::number(42);
  child->adapter<LispleTest::Native::OptionalChildAdapter>().set_value(nullptr, child_value);

  Lisple::sptr_rtval val = LispleTest::Native::OptionalChildPtrBoxAdapter::make_unique();
  LispleTest::Native::OptionalChildPtrBoxAdapter& adapter =
    val->adapter<LispleTest::Native::OptionalChildPtrBoxAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);

  // When
  adapter.set_child(nullptr, child);

  // Then
  ASSERT_NE(adapter.get_object().child, nullptr);
  EXPECT_EQ(adapter.get_object().child->value, 42);
  EXPECT_EQ(adapter.get_child()
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            42);

  // When
  auto nil = Lisple::Constant::NIL;
  adapter.set_child(nullptr, nil);

  // Then
  EXPECT_EQ(adapter.get_object().child, nullptr);
  EXPECT_EQ(*adapter.get_child(), *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_VectorCopyField, get_value_vector_copy_field)
{
  // Given
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Lisple::sptr_rtval values = adapter.get_values();
  values->mut_elements().push_back(Lisple::RTValue::number(99));

  // Then
  ASSERT_EQ(values->type, Lisple::RTValue::Type::VECTOR);
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
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Lisple::sptr_rtval values = adapter.get_keyword_values();
  values->mut_elements().push_back(Lisple::RTValue::keyword("extra"));

  // Then
  ASSERT_EQ(values->type, Lisple::RTValue::Type::VECTOR);
  ASSERT_EQ(values->elements().size(), 3);
  EXPECT_EQ(*values->elements().at(0), *Lisple::RTValue::keyword("first"));
  EXPECT_EQ(*values->elements().at(1), *Lisple::RTValue::keyword("second"));
  EXPECT_EQ(*values->elements().at(2), *Lisple::RTValue::keyword("extra"));
  EXPECT_EQ(adapter.get_object().keyword_values.size(), 2);

  // When
  auto updated = Lisple::RTValue::vector(
    {Lisple::RTValue::keyword("third"), Lisple::RTValue::string("fourth")});
  adapter.set_keyword_values(nullptr, updated);

  // Then
  ASSERT_EQ(adapter.get_object().keyword_values.size(), 2);
  EXPECT_EQ(adapter.get_object().keyword_values.at(0), "third");
  EXPECT_EQ(adapter.get_object().keyword_values.at(1), "fourth");
  EXPECT_EQ(*adapter.get_keyword_values()->elements().at(0),
            *Lisple::RTValue::keyword("third"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("keyword-values")),
            *adapter.get_keyword_values());
}

TEST(NativeObjectAdapter_VectorCopyField, get_adapter_vector_copy_field)
{
  // Given
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Lisple::sptr_rtval children = adapter.get_children();
  children->mut_elements().push_back(Lisple::Constant::NIL);

  // Then
  ASSERT_EQ(children->type, Lisple::RTValue::Type::VECTOR);
  ASSERT_EQ(children->elements().size(), 3);
  EXPECT_EQ(children->elements()
              .at(0)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(children->elements()
              .at(1)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            8);
  EXPECT_EQ(*children->elements().at(2), *Lisple::Constant::NIL);
  EXPECT_EQ(adapter.get_object().children.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyField, get_pointer_adapter_vector_copy_field)
{
  // Given
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();
  adapter.get_object().child_ptrs.push_back(&adapter.get_object().children.at(0));
  adapter.get_object().child_ptrs.push_back(nullptr);

  // When
  Lisple::sptr_rtval child_ptrs = adapter.get_child_ptrs();
  child_ptrs->mut_elements().push_back(Lisple::Constant::NIL);

  // Then
  ASSERT_EQ(child_ptrs->type, Lisple::RTValue::Type::VECTOR);
  ASSERT_EQ(child_ptrs->elements().size(), 3);
  EXPECT_EQ(child_ptrs->elements()
              .at(0)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(*child_ptrs->elements().at(1), *Lisple::Constant::NIL);
  EXPECT_EQ(*child_ptrs->elements().at(2), *Lisple::Constant::NIL);
  EXPECT_EQ(adapter.get_object().child_ptrs.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyMethod, get_value_vector_copy_method)
{
  // Given
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Lisple::sptr_rtval values = adapter.get_method_values();
  values->mut_elements().push_back(Lisple::RTValue::number(99));

  // Then
  ASSERT_EQ(values->type, Lisple::RTValue::Type::VECTOR);
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
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();

  // When
  Lisple::sptr_rtval children = adapter.get_method_children();
  children->mut_elements().push_back(Lisple::Constant::NIL);

  // Then
  ASSERT_EQ(children->type, Lisple::RTValue::Type::VECTOR);
  ASSERT_EQ(children->elements().size(), 3);
  EXPECT_EQ(children->elements()
              .at(0)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(children->elements()
              .at(1)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            8);
  EXPECT_EQ(*children->elements().at(2), *Lisple::Constant::NIL);
  EXPECT_EQ(adapter.get_object().children.size(), 2);
}

TEST(NativeObjectAdapter_VectorCopyMethod, get_pointer_adapter_vector_copy_method)
{
  // Given
  Lisple::sptr_rtval val = VectorBoxAdapter::make_unique();
  VectorBoxAdapter& adapter = val->adapter<VectorBoxAdapter>();
  adapter.get_object().child_ptrs.push_back(&adapter.get_object().children.at(0));
  adapter.get_object().child_ptrs.push_back(nullptr);

  // When
  Lisple::sptr_rtval child_ptrs = adapter.get_method_child_ptrs();
  child_ptrs->mut_elements().push_back(Lisple::Constant::NIL);

  // Then
  ASSERT_EQ(child_ptrs->type, Lisple::RTValue::Type::VECTOR);
  ASSERT_EQ(child_ptrs->elements().size(), 3);
  EXPECT_EQ(child_ptrs->elements()
              .at(0)
              ->adapter<LispleTest::Native::OptionalChildAdapter>()
              .get_object()
              .value,
            5);
  EXPECT_EQ(*child_ptrs->elements().at(1), *Lisple::Constant::NIL);
  EXPECT_EQ(*child_ptrs->elements().at(2), *Lisple::Constant::NIL);
  EXPECT_EQ(adapter.get_object().child_ptrs.size(), 2);
}

TEST(NativeObjectAdapter_VehicleModel, get_property)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

  // Then
  EXPECT_EQ(*adapter.get_model_name(), *Lisple::RTValue::string("Runaway Train"));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("model-name")),
            *Lisple::RTValue::string("Runaway Train"));

  EXPECT_EQ(*adapter.get_seats(), *Lisple::RTValue::number(500));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("seats")),
            *Lisple::RTValue::number(500));

  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("does-not-exist")),
            *Lisple::Constant::NIL);
}

TEST(NativeObjectAdapter_VehicleModel, set_property)
{
  // Given
  Lisple::sptr_rtval val =
    LispleTest::Native::VehicleModelAdapter::make_unique("Runaway Train", 500);
  LispleTest::Native::VehicleModelAdapter& adapter =
    val->adapter<LispleTest::Native::VehicleModelAdapter>();

  LispleTest::VehicleModel& vehicle = adapter.get_object();
  EXPECT_EQ(vehicle.get_model_name(), "Runaway Train");
  EXPECT_EQ(vehicle.get_seats(), 500);

  // When
  vehicle.set_seats(800);

  // Then
  EXPECT_EQ(*adapter.get_seats(), *Lisple::RTValue::number(800));
  EXPECT_EQ(*Lisple::Dict::get_property(val, Lisple::RTValue::keyword("seats")),
            *Lisple::RTValue::number(800));
  EXPECT_EQ(vehicle.get_seats(), 800);
}
