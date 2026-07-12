BUILD_TYPE ?= Release
FILTER ?=
GTEST_FILTER_ARG := $(if $(FILTER),--gtest_filter=$(FILTER),)

LOCAL_PREFIX := $(HOME)/.local
PATH_HAS_LOCAL_BIN := $(findstring $(LOCAL_PREFIX)/bin,$(PATH))
ifeq ($(PATH_HAS_LOCAL_BIN),)
  PREFIX ?= /usr/local
else
  PREFIX ?= $(LOCAL_PREFIX)
endif

.PHONY: configure configure-server-tests build relink install build-proof build-lookup build-proofread install-loom install-proof install-lookup install-proofread install-workbook release test test\:all test\:lang test\:package test\:proof test\:workbook test\:rooc test\:cli test\:roo-cli test\:loom-cli test\:lookup-cli test\:benchmark test\:server clean

TEST_BINARY := lib/libroo/test/testroo
PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage
PROOF_TEST_BINARY := pkg/proof/test/testproof
ROOC_TEST_BINARY := bin/rooc/test/testrooc
SERVER_TEST_BINARY := lib/libroo-server/test/testserver
LOOM_BINARY := loom
LOOKUP_BINARY := lookup
PROOF_NATIVE_LIBRARY := libproof-native.so
PROOFREAD_NATIVE_LIBRARY := libproofread-native.so
PROOFREAD_BINARY := proofread
ifeq ($(OS),Windows_NT)
  TEST_BINARY := lib/libroo/test/testroo.exe
  PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage.exe
  PROOF_TEST_BINARY := pkg/proof/test/testproof.exe
  ROOC_TEST_BINARY := bin/rooc/test/testrooc.exe
  SERVER_TEST_BINARY := lib/libroo-server/test/testserver.exe
  LOOM_BINARY := loom.exe
  LOOKUP_BINARY := lookup.exe
  PROOF_NATIVE_LIBRARY := proof-native.dll
  PROOFREAD_NATIVE_LIBRARY := proofread-native.dll
  PROOFREAD_BINARY := proofread.exe
endif
ifeq ($(shell uname -s),Darwin)
  PROOF_NATIVE_LIBRARY := libproof-native.dylib
  PROOFREAD_NATIVE_LIBRARY := libproofread-native.dylib
endif

RELINK_ARTIFACTS := \
	$(CURDIR)/build/roo \
	$(CURDIR)/build/rooc \
	$(CURDIR)/build/lib/libroo/libroo.a \
	$(CURDIR)/build/lib/libroo/libroo.so \
	$(CURDIR)/build/lib/libroo/test/testroo \
	$(CURDIR)/build/lib/libroo-package/libroo-package.a \
	$(CURDIR)/build/lib/libroo-package/libroo-package.so \
	$(CURDIR)/build/lib/libroo-package/test/libroo-package-test-native.so \
	$(CURDIR)/build/lib/libroo-package/test/testpackage \
	$(CURDIR)/build/lib/libroo-server/libroo-server.a \
	$(CURDIR)/build/lib/libroo-server/libroo-server.so \
	$(CURDIR)/build/lib/libroo-server/roo-server \
	$(CURDIR)/build/lib/libroo-server/test/testserver \
	$(CURDIR)/build/bin/rooc/test/testrooc \
	$(CURDIR)/build/pkg/proof/native/$(PROOF_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proof/native/libproof_native.a \
	$(CURDIR)/build/pkg/proof/test/testproof \
	$(CURDIR)/build/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proofread/proofread \
	$(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY)

configure:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DROO_SERVER_BUILD_TESTS=OFF

configure-server-tests:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DROO_SERVER_BUILD_TESTS=ON

build: configure
	cmake --build build

relink: configure
	cmake -E rm -f $(RELINK_ARTIFACTS)
	cmake --build build

install: build
	cmake --build build --target install

install-loom: build
	./build/rooc build $(CURDIR)/pkg/loom --build-dir $(CURDIR)/build/loom-install --name loom
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/loom-install/build/$(LOOM_BINARY) $(PREFIX)/bin/$(LOOM_BINARY)

build-proof: build
	cmake --build build --target proof_native

install-proof: build-proof
	cmake -E make_directory $(PREFIX)/share/roo/pkg/proof/src
	cmake -E make_directory $(PREFIX)/share/roo/pkg/proof/native
	cmake -E copy_directory $(CURDIR)/pkg/proof/src $(PREFIX)/share/roo/pkg/proof/src
	cmake -E copy_if_different $(CURDIR)/pkg/proof/package.edn $(PREFIX)/share/roo/pkg/proof/package.edn
	cmake -E copy_if_different $(CURDIR)/pkg/proof/README.md $(PREFIX)/share/roo/pkg/proof/README.md
	cmake -E copy_if_different $(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY) $(PREFIX)/share/roo/pkg/proof/native/$(PROOF_NATIVE_LIBRARY)

build-lookup: build
	./build/rooc build $(CURDIR)/pkg/lookup --build-dir $(CURDIR)/build/lookup-install --name lookup

install-lookup: build-lookup
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) $(PREFIX)/bin/$(LOOKUP_BINARY)

build-proofread: build
	./build/rooc build $(CURDIR)/pkg/proofread --build-dir $(CURDIR)/build/proofread-install --name proofread

install-proofread: build-proofread
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/proofread-install/build/$(PROOFREAD_BINARY) $(PREFIX)/bin/$(PROOFREAD_BINARY)

install-workbook: build
	cmake -E make_directory $(PREFIX)/share/roo/pkg/workbook/src
	cmake -E copy_directory $(CURDIR)/pkg/workbook/src $(PREFIX)/share/roo/pkg/workbook/src
	cmake -E copy_if_different $(CURDIR)/pkg/workbook/package.edn $(PREFIX)/share/roo/pkg/workbook/package.edn
	cmake -E copy_if_different $(CURDIR)/pkg/workbook/README.md $(PREFIX)/share/roo/pkg/workbook/README.md

release:
	sh $(CURDIR)/tools/release/package.sh $(VERSION)

test: test\:lang test\:package test\:proof test\:workbook test\:rooc
test: test\:cli

test\:all: test\:lang test\:package test\:proof test\:workbook test\:rooc test\:cli test\:server

test\:lang: build
	cmake --build build --target testroo
	./build/$(TEST_BINARY) --skip-benchmarks $(GTEST_FILTER_ARG)

test\:package: build
	cmake --build build --target testpackage
	./build/$(PACKAGE_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:proof: build
	cmake --build build --target testproof
	./build/$(PROOF_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:workbook: build
	cd $(CURDIR)/pkg/workbook/test && $(CURDIR)/build/roo proof

test\:rooc: build
	cmake --build build --target testrooc
	./build/$(ROOC_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:cli: test\:roo-cli test\:loom-cli test\:lookup-cli

test\:roo-cli: build
	sh $(CURDIR)/bin/roo/test/run-cli-tests.sh $(CURDIR)

test\:loom-cli: build
	sh $(CURDIR)/pkg/loom/test/run-cli-tests.sh $(CURDIR)

test\:lookup-cli: build
	sh $(CURDIR)/pkg/lookup/test/run-cli-tests.sh $(CURDIR)

test\:benchmark: build
	cmake --build build --target testroo
	./build/$(TEST_BINARY) --benchmark $(GTEST_FILTER_ARG)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY) $(GTEST_FILTER_ARG)

clean:
	rm -rf build
