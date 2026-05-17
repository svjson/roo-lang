BUILD_TYPE ?= Release

LOCAL_PREFIX := $(HOME)/.local
PATH_HAS_LOCAL_BIN := $(findstring $(LOCAL_PREFIX)/bin,$(PATH))
ifeq ($(PATH_HAS_LOCAL_BIN),)
  PREFIX ?= /usr/local
else
  PREFIX ?= $(LOCAL_PREFIX)
endif

.PHONY: configure configure-server-tests build install test test\:all test\:lang test\:package test\:proof test\:benchmark test\:server clean

TEST_BINARY := lib/liblisple/test/testlisple
PACKAGE_TEST_BINARY := lib/lisple-package/test/testpackage
PROOF_TEST_BINARY := pkg/proof/test/testproof
SERVER_TEST_BINARY := lib/lisple-server/test/testserver
ifeq ($(OS),Windows_NT)
  TEST_BINARY := lib/liblisple/test/testlisple.exe
  PACKAGE_TEST_BINARY := lib/lisple-package/test/testpackage.exe
  PROOF_TEST_BINARY := pkg/proof/test/testproof.exe
  SERVER_TEST_BINARY := lib/lisple-server/test/testserver.exe
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

test: test\:lang test\:package test\:proof

test\:all: test\:lang test\:package test\:proof test\:server

test\:lang: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY) --skip-benchmarks

test\:package: build
	cmake --build build --target testpackage
	./build/$(PACKAGE_TEST_BINARY)

test\:proof: build
	cmake --build build --target testproof
	./build/$(PROOF_TEST_BINARY)

test\:benchmark: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY)

clean:
	rm -rf build
