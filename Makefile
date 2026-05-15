BUILD_TYPE ?= Release
PREFIX ?= $(HOME)/.local

.PHONY: configure build install test clean

TEST_BINARY := test/testlisple
ifeq ($(OS),Windows_NT)
  TEST_BINARY := test/testlisple.exe
endif

configure:
	cmake -S . -B build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX)

build: configure
	cmake --build build

install: build
	cmake --build build --target install

test: build
	cmake --build build --target testlisple
	./build/$(TEST_BINARY) --skip-benchmarks

clean:
	rm -rf build
