
#ifndef __LISP_READER_FIXTURE_H_
#define __LISP_READER_FIXTURE_H_

#include <lisple/context.h>
#include <lisple/lisp_reader.h>
#include <lisple/lang.h>
#include <lisple/host.h>
#include <lisple/dir_root_file_system.h>

#include "fake_file_system.h"

namespace LispleTest
{
  const std::string COMMON_TESTDATA_DIR = "test_resources/script/common";

  class LispReaderFixture
  {
   public:
    LispReaderFixture()
      : dir_fs("test_resources/script")
      , lisp_reader(&fs)
      , ctx(lisp_reader)
    {
    }

    LispReaderFixture(const std::string path)
      : dir_fs(path)
      , lisp_reader(&dir_fs)
      , ctx(lisp_reader)
    {
    }

    LispleTest::FakeFileSystem fs;
    Lisple::DirRootFileSystem dir_fs;
    Lisple::Reader parser;
    Lisple::LispReader lisp_reader{ &fs };
    Lisple::Context ctx { lisp_reader };

    template <class T>
    std::unique_ptr<T>& define_and_get_host_object(const std::string& name, const std::string& lisp)
    {
      lisp_reader.eval("(def " + name + " " + lisp + ")");
      return lisp_reader.get_current_namespace()
        .lookup(Lisple::Word(name))->as<Lisple::HostObject<T>>()
        .get_object_ptr();
    }

    template <class T>
    T& lookup_host_object(const std::string& name)
    {
      return lisp_reader.get_current_namespace()
        .lookup(Lisple::Word(name))->as<Lisple::HostObject<T>>()
        .get_object();
    }
  };
}

#endif
