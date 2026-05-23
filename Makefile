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

.PHONY: configure configure-server-tests build install install-loom test test\:all test\:lang test\:package test\:proof test\:lisplec test\:cli test\:lisple-cli test\:loom-cli test\:benchmark test\:server clean

TEST_BINARY := lib/liblisple/test/testlisple
PACKAGE_TEST_BINARY := lib/lisple-package/test/testpackage
PROOF_TEST_BINARY := pkg/proof/test/testproof
LISPLEC_TEST_BINARY := bin/lisplec/test/testlisplec
SERVER_TEST_BINARY := lib/lisple-server/test/testserver
LOOM_BINARY := loom
ifeq ($(OS),Windows_NT)
  TEST_BINARY := lib/liblisple/test/testlisple.exe
  PACKAGE_TEST_BINARY := lib/lisple-package/test/testpackage.exe
  PROOF_TEST_BINARY := pkg/proof/test/testproof.exe
  LISPLEC_TEST_BINARY := bin/lisplec/test/testlisplec.exe
  SERVER_TEST_BINARY := lib/lisple-server/test/testserver.exe
  LOOM_BINARY := loom.exe
endif

configure:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DLISPLE_SERVER_BUILD_TESTS=OFF

configure-server-tests:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DLISPLE_SERVER_BUILD_TESTS=ON

build: configure
	cmake --build build

install: build
	cmake --build build --target install

install-loom: build
	./build/lisplec build $(CURDIR)/pkg/loom --build-dir $(CURDIR)/build/loom-install --name loom
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/loom-install/build/$(LOOM_BINARY) $(PREFIX)/bin/$(LOOM_BINARY)

test: test\:lang test\:package test\:proof test\:lisplec
test: test\:cli

test\:all: test\:lang test\:package test\:proof test\:lisplec test\:cli test\:server

test\:lang: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY) --skip-benchmarks $(GTEST_FILTER_ARG)

test\:package: build
	cmake --build build --target testpackage
	./build/$(PACKAGE_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:proof: build
	cmake --build build --target testproof
	./build/$(PROOF_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:lisplec: build
	cmake --build build --target testlisplec
	./build/$(LISPLEC_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:cli: test\:lisple-cli test\:loom-cli

test\:lisple-cli: build
	sh $(CURDIR)/bin/lisple/test/run-cli-tests.sh $(CURDIR)

test\:loom-cli: build
	sh $(CURDIR)/pkg/loom/test/run-cli-tests.sh $(CURDIR)

test\:benchmark: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY) --benchmark $(GTEST_FILTER_ARG)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY) $(GTEST_FILTER_ARG)

clean:
	rm -rf build
