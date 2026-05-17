#include "runtime_fixture.h"
#include <gtest/gtest.h>

using IoNamespace = LispleTest::RuntimeTestFixture;

TEST_F(IoNamespace, slurp_bang_reads_file_contents)
{
  fs.add_file("notes.txt", "plain text\n");

  EXPECT_EQ(runtime.eval(R"((lisple.io/slurp! "notes.txt"))")->to_string(),
            R"("plain text
")");
}

TEST_F(IoNamespace, spit_bang_writes_file_contents)
{
  EXPECT_EQ(runtime.eval(R"((lisple.io/spit! "notes.txt" "plain text"))")->to_string(),
            "nil");

  ASSERT_TRUE(fs.has_file("notes.txt"));
  EXPECT_EQ(fs.get_file("notes.txt"), "plain text");
}

TEST_F(IoNamespace, slurp_edn_bang_reads_single_data_form)
{
  fs.add_file("data.edn", "{:name \"Lisple\" :numbers [1 2 3] :ok true}");

  EXPECT_EQ(runtime.eval(R"((lisple.io/slurp-edn! "data.edn"))")->to_string(),
            R"({:name "Lisple" :numbers [1 2 3] :ok true})");
}

TEST_F(IoNamespace, spit_edn_bang_writes_value_representation)
{
  runtime.eval(R"((lisple.io/spit-edn! "data.edn" {:a 1 :b [true nil]}))");

  ASSERT_TRUE(fs.has_file("data.edn"));
  EXPECT_EQ(fs.get_file("data.edn"), "{:a 1 :b [true nil]}");
}

TEST_F(IoNamespace, io_namespace_can_be_required_with_alias)
{
  fs.add_file("data.edn", "[:ok 1]");

  EXPECT_EQ(runtime
              .eval(R"(
    (ns my-app.core (:require [lisple.io :as io]))
    (io/slurp-edn! "data.edn")
  )")
              ->to_string(),
            "[:ok 1]");
}

TEST_F(IoNamespace, io_namespace_cannot_be_modified_from_lisple)
{
  EXPECT_THROW(runtime.eval("(ns lisple.io) (def x 1)"), Lisple::NamespaceException);
}
