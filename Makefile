BUILD_TYPE ?= Release
FILTER ?=
GTEST_FILTER_ARG := $(if $(FILTER),--gtest_filter=$(FILTER),)
NATIVE_PACKAGE_STAGE := $(CURDIR)/build/native-package-stage/pkg
ROO_LANG_INDEX_VERSION ?= $(shell cat $(CURDIR)/VERSION)
ROO_LANG_INDEX_DIR := $(CURDIR)/build/indexes/roo-lang/$(ROO_LANG_INDEX_VERSION)
ROO_LANG_INDEX_PATH := $(ROO_LANG_INDEX_DIR)/roo-symbols.edn
ROO_LANG_INDEX_INSTALL_DIR = $(PREFIX)/share/roo/indexes/roo-lang/$(ROO_LANG_INDEX_VERSION)
ROO_LANG_INDEX_AUDIT_FLAGS := \
	--output-format text \
	--require-summary \
	--require-param-docs \
	--require-signatures \
	--allow-zero-arity roo.io/current-directory! \
	--allow-zero-arity roo.io/home-directory! \
	--allow-zero-arity roo/epoch-ms \
	--fail-on warning

LOCAL_PREFIX := $(HOME)/.local
PREFIX ?= $(LOCAL_PREFIX)

.PHONY: configure configure-server-tests build relink dev-native-packages dev-native-package-links stage-native-packages install build-proof build-lookup build-roo-lang-index audit-roo-lang-index build-proofread install-loom install-proof install-lookup install-roo-lang-index install-proofread install-workbook install-footsteps release test test\:all test\:support test\:lang test\:package test\:proof test\:proofread test\:workbook test\:footsteps test\:rooc test\:cli test\:roo-cli test\:loom-cli test\:lookup-cli test\:benchmark test\:server clean

SUPPORT_TEST_BINARY := lib/libroo-support/test/testsupport
TEST_BINARY := lib/libroo/test/testroo
PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage
PROOF_TEST_BINARY := pkg/proof/test/testproof
ROOC_TEST_BINARY := bin/rooc/test/testrooc
SERVER_TEST_BINARY := lib/libroo-server/test/testserver
LOOM_BINARY := loom
LOOKUP_BINARY := lookup
PROOF_NATIVE_LIBRARY := libproof-native.so
LOOKUP_NATIVE_LIBRARY := liblookup-native.so
PROOFREAD_NATIVE_LIBRARY := libproofread-native.so
PROOFREAD_BINARY := proofread
ifeq ($(OS),Windows_NT)
  SUPPORT_TEST_BINARY := lib/libroo-support/test/testsupport.exe
  TEST_BINARY := lib/libroo/test/testroo.exe
  PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage.exe
  PROOF_TEST_BINARY := pkg/proof/test/testproof.exe
  ROOC_TEST_BINARY := bin/rooc/test/testrooc.exe
  SERVER_TEST_BINARY := lib/libroo-server/test/testserver.exe
  LOOM_BINARY := loom.exe
  LOOKUP_BINARY := lookup.exe
  PROOF_NATIVE_LIBRARY := proof-native.dll
  LOOKUP_NATIVE_LIBRARY := lookup-native.dll
  PROOFREAD_NATIVE_LIBRARY := proofread-native.dll
  PROOFREAD_BINARY := proofread.exe
endif
ifeq ($(shell uname -s),Darwin)
  PROOF_NATIVE_LIBRARY := libproof-native.dylib
  LOOKUP_NATIVE_LIBRARY := liblookup-native.dylib
  PROOFREAD_NATIVE_LIBRARY := libproofread-native.dylib
endif

ifeq ($(OS),Windows_NT)
  DEV_NATIVE_ARTIFACT = cmake -E copy_if_different $(1) $(2)
else
  DEV_NATIVE_ARTIFACT = cmake -E create_symlink $(1) $(2)
endif

ifeq ($(shell uname -s),Darwin)
  SET_PACKAGE_NATIVE_RPATH = install_name_tool -add_rpath @loader_path/../../../../../lib $(1) 2>/dev/null || true
else ifeq ($(OS),Windows_NT)
  SET_PACKAGE_NATIVE_RPATH = true
else
  SET_PACKAGE_NATIVE_RPATH = patchelf --set-rpath '$$ORIGIN/../../../../../lib' $(1)
endif

RELINK_ARTIFACTS := \
	$(CURDIR)/build/roo \
	$(CURDIR)/build/rooc \
	$(CURDIR)/build/lib/libroo-support/libroo-support.a \
	$(CURDIR)/build/lib/libroo-support/libroo-support.so* \
	$(CURDIR)/build/lib/libroo-support/test/testsupport \
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
	$(CURDIR)/build/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proofread/proofread \
	$(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY) \
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
	$(MAKE) dev-native-package-links

relink: configure
	cmake -E rm -f $(RELINK_ARTIFACTS)
	cmake --build build
	$(MAKE) dev-native-package-links

dev-native-packages: configure
	cmake --build build --target proof_native lookup_native proofread_native
	$(MAKE) dev-native-package-links

dev-native-package-links:
	cmake -E make_directory $(CURDIR)/pkg/proof/native
	cmake -E make_directory $(CURDIR)/pkg/lookup/native
	cmake -E make_directory $(CURDIR)/pkg/proofread/native
	cmake -E rm -f $(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY)
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/proof/native/$(PROOF_NATIVE_LIBRARY),$(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY),$(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY),$(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY))

stage-native-packages: configure
	cmake --build build --target stage_native_packages

install: build
	cmake --build build --target install

install-loom: build stage-native-packages
	./build/rooc build $(NATIVE_PACKAGE_STAGE)/loom --build-dir $(CURDIR)/build/loom-install --name loom
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/loom-install/build/$(LOOM_BINARY) $(PREFIX)/bin/$(LOOM_BINARY)

build-proof: build stage-native-packages

install-proof: build-proof
	cmake -E make_directory $(PREFIX)/share/roo/pkg/proof/src
	cmake -E make_directory $(PREFIX)/share/roo/pkg/proof/native
	cmake -E copy_directory $(NATIVE_PACKAGE_STAGE)/proof/src $(PREFIX)/share/roo/pkg/proof/src
	cmake -E copy_if_different $(NATIVE_PACKAGE_STAGE)/proof/package.edn $(PREFIX)/share/roo/pkg/proof/package.edn
	cmake -E copy_if_different $(NATIVE_PACKAGE_STAGE)/proof/README.md $(PREFIX)/share/roo/pkg/proof/README.md
	cmake -E copy_if_different $(NATIVE_PACKAGE_STAGE)/proof/native/$(PROOF_NATIVE_LIBRARY) $(PREFIX)/share/roo/pkg/proof/native/$(PROOF_NATIVE_LIBRARY)
	$(call SET_PACKAGE_NATIVE_RPATH,$(PREFIX)/share/roo/pkg/proof/native/$(PROOF_NATIVE_LIBRARY))

build-lookup: configure
	cmake --build build --target rooc_cli stage_native_packages
	./build/rooc build $(NATIVE_PACKAGE_STAGE)/lookup --build-dir $(CURDIR)/build/lookup-install --name lookup

build-roo-lang-index: build-lookup
	cmake -E make_directory $(ROO_LANG_INDEX_DIR)
	$(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) index --root lib/libroo/include/roo/lang --root lib/libroo/src/roo/lang -o $(ROO_LANG_INDEX_PATH)

audit-roo-lang-index: build-roo-lang-index
	$(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) audit $(ROO_LANG_INDEX_AUDIT_FLAGS) $(ROO_LANG_INDEX_PATH)

install-lookup: build-lookup
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) $(PREFIX)/bin/$(LOOKUP_BINARY)

install-roo-lang-index: audit-roo-lang-index
	cmake -E make_directory $(ROO_LANG_INDEX_INSTALL_DIR)
	cmake -E copy_if_different $(ROO_LANG_INDEX_PATH) $(ROO_LANG_INDEX_INSTALL_DIR)/roo-symbols.edn

build-proofread: build stage-native-packages
	./build/rooc build $(NATIVE_PACKAGE_STAGE)/proofread --build-dir $(CURDIR)/build/proofread-install --name proofread

install-proofread: build-proofread
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/proofread-install/build/$(PROOFREAD_BINARY) $(PREFIX)/bin/$(PROOFREAD_BINARY)

install-workbook: build
	cmake -E make_directory $(PREFIX)/share/roo/pkg/workbook/src
	cmake -E copy_directory $(CURDIR)/pkg/workbook/src $(PREFIX)/share/roo/pkg/workbook/src
	cmake -E copy_if_different $(CURDIR)/pkg/workbook/package.edn $(PREFIX)/share/roo/pkg/workbook/package.edn
	cmake -E copy_if_different $(CURDIR)/pkg/workbook/README.md $(PREFIX)/share/roo/pkg/workbook/README.md

install-footsteps: build
	cmake -E make_directory $(PREFIX)/share/roo/pkg/footsteps/src
	cmake -E copy_directory $(CURDIR)/pkg/footsteps/src $(PREFIX)/share/roo/pkg/footsteps/src
	cmake -E copy_if_different $(CURDIR)/pkg/footsteps/package.edn $(PREFIX)/share/roo/pkg/footsteps/package.edn
	cmake -E copy_if_different $(CURDIR)/pkg/footsteps/README.md $(PREFIX)/share/roo/pkg/footsteps/README.md

release:
	sh $(CURDIR)/tools/release/package.sh $(VERSION)

test: test\:support test\:lang test\:package test\:proof test\:proofread test\:workbook test\:footsteps test\:rooc
test: test\:cli

test\:all: test\:support test\:lang test\:package test\:proof test\:proofread test\:workbook test\:footsteps test\:rooc test\:cli test\:server

test\:support: build
	cmake --build build --target testsupport
	./build/$(SUPPORT_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:lang: build
	cmake --build build --target testroo
	./build/$(TEST_BINARY) --skip-benchmarks $(GTEST_FILTER_ARG)

test\:package: build
	cmake --build build --target testpackage
	./build/$(PACKAGE_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:proof: build
	cmake --build build --target testproof
	./build/$(PROOF_TEST_BINARY) $(GTEST_FILTER_ARG)
	cd $(NATIVE_PACKAGE_STAGE)/proof/test && $(CURDIR)/build/roo proof

test\:proofread: configure
	cmake --build build --target roo_cli
	cmake --build build --target stage_native_packages
	cd $(NATIVE_PACKAGE_STAGE)/proofread/test && $(CURDIR)/build/roo proof

test\:workbook: build stage-native-packages
	cd $(NATIVE_PACKAGE_STAGE)/workbook/test && $(CURDIR)/build/roo proof

test\:footsteps: build stage-native-packages
	cd $(NATIVE_PACKAGE_STAGE)/footsteps/test && $(CURDIR)/build/roo proof

test\:rooc: build stage-native-packages
	cmake --build build --target testrooc
	./build/$(ROOC_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:cli: test\:roo-cli test\:loom-cli test\:lookup-cli

test\:roo-cli: build stage-native-packages
	ROO_PACKAGE_STAGE_ROOT=$(NATIVE_PACKAGE_STAGE) sh $(CURDIR)/bin/roo/test/run-cli-tests.sh $(CURDIR)

test\:loom-cli: build stage-native-packages
	ROO_PACKAGE_STAGE_ROOT=$(NATIVE_PACKAGE_STAGE) sh $(CURDIR)/pkg/loom/test/run-cli-tests.sh $(CURDIR)

test\:lookup-cli: build stage-native-packages
	ROO_PACKAGE_STAGE_ROOT=$(NATIVE_PACKAGE_STAGE) sh $(CURDIR)/pkg/lookup/test/run-cli-tests.sh $(CURDIR)

test\:benchmark: build
	cmake --build build --target testroo
	./build/$(TEST_BINARY) --benchmark $(GTEST_FILTER_ARG)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY) $(GTEST_FILTER_ARG)

clean:
	rm -rf build
