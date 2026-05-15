BUILD_TYPE ?= Release

LOCAL_PREFIX := $(HOME)/.local
PATH_HAS_LOCAL_BIN := $(findstring $(LOCAL_PREFIX)/bin,$(PATH))
ifeq ($(PATH_HAS_LOCAL_BIN),)
  PREFIX ?= /usr/local
else
  PREFIX ?= $(LOCAL_PREFIX)
endif

.PHONY: configure configure-server-tests build install test test\:all test\:lang test\:benchmark test\:server clean

TEST_BINARY := test/testlisple
SERVER_TEST_BINARY := lib/lisple-server/test/testserver
ifeq ($(OS),Windows_NT)
  TEST_BINARY := test/testlisple.exe
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

test: test\:lang

test\:all: test\:lang test\:server

test\:lang: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY) --skip-benchmarks

test\:benchmark: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY)

clean:
	rm -rf build
