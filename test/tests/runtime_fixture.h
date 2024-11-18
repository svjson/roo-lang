
#ifndef __RUNTIME_FIXTURE_H_
#define __RUNTIME_FIXTURE_H_

#include <lisple/context.h>
#include <lisple/runtime.h>
#include <lisple/lang.h>
#include <lisple/host.h>
#include <lisple/dir_root_file_system.h>

#include "fake_file_system.h"

namespace LispleTest
{
  const std::string COMMON_TESTDATA_DIR = "test_resources/script/common";

  class RuntimeFixture
  {
   public:
    RuntimeFixture()
      : dir_fs("test_resources/script")
      , runtime(&fs)
      , ctx(runtime)
    {
    }

    RuntimeFixture(const std::string path)
      : dir_fs(path)
      , runtime(&dir_fs)
      , ctx(runtime)
    {
    }

    LispleTest::FakeFileSystem fs;
    Lisple::DirRootFileSystem dir_fs;
    Lisple::Reader parser;
    Lisple::Runtime runtime{ &fs };
    Lisple::Context ctx { runtime };

    template <class T>
    std::unique_ptr<T>& define_and_get_host_object(const std::string& name, const std::string& lisp)
    {
      runtime.eval("(def " + name + " " + lisp + ")");
      return runtime.get_current_namespace()
        .lookup(Lisple::Word(name))->as<Lisple::HostObject<T>>()
        .get_object_ptr();
    }

    template <class T>
    T& lookup_host_object(const std::string& name)
    {
      return runtime.get_current_namespace()
        .lookup(Lisple::Word(name))->as<Lisple::HostObject<T>>()
        .get_object();
    }
  };
}

namespace Lisple
{
  void PrintTo(const Lisple::Object& obj, std::ostream* os);
}



#endif
