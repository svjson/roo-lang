BUILD_TYPE ?= Release
CMAKE_GENERATOR ?= Ninja
CMAKE_BUILD_PARALLEL_LEVEL ?= 1
export CMAKE_BUILD_PARALLEL_LEVEL
FILTER ?=
GTEST_FILTER_ARG := $(if $(FILTER),--gtest_filter=$(FILTER),)
PACKAGE_STAGE := $(CURDIR)/build/package-stage/pkg
PACKAGE_ARTIFACT_DIR := $(CURDIR)/build/package-artifacts
ROO_LANG_INDEX_VERSION ?= $(shell cat $(CURDIR)/VERSION)
ROO_LANG_INDEX_DIR := $(CURDIR)/build/indexes/roo-lang/$(ROO_LANG_INDEX_VERSION)
ROO_LANG_INDEX_PATH := $(ROO_LANG_INDEX_DIR)/symbols.edn
ROO_LANG_INDEX_INSTALL_DIR = $(DESTDIR)$(PREFIX)/share/roo/indexes/roo-lang/$(ROO_LANG_INDEX_VERSION)
ROO_REPOSITORY_PACKAGES := $(notdir $(patsubst %/package.edn,%,$(wildcard $(CURDIR)/pkg/*/package.edn)))
GITHUB_PAGES_DOCS_DIR ?= $(CURDIR)/build/docs/github-pages
GITHUB_PAGES_PACKAGE_INDEXES = $(foreach package,$(ROO_REPOSITORY_PACKAGES),$(wildcard $(PACKAGE_REPOSITORY)/$(package)/*/symbols.edn))
GITHUB_PAGES_DOC_INDEXES = $(ROO_LANG_INDEX_INSTALL_DIR)/symbols.edn $(GITHUB_PAGES_PACKAGE_INDEXES)

LOCAL_PREFIX := $(HOME)/.local
PREFIX ?= $(LOCAL_PREFIX)
PACKAGE_REPOSITORY = $(DESTDIR)$(PREFIX)/share/roo/pkg
BOOTSTRAP_LOOM = $(CURDIR)/build/loom-bootstrap/build/$(LOOM_BINARY)

ROO_BUILD_TARGETS := configure configure-server-tests build bootstrap-loom package-artifacts relink dev-native-packages dev-native-package-links stage-packages install install-packages build-proof build-lookup build-roopl build-roo-lang-index audit-roo-lang-index build-proofread build-boodle build-github-pages-docs install-loom install-proof install-inpoots install-lookup install-roopl install-roo-lang-index install-proofread install-boodle install-i18n install-moordown install-spool install-workbook install-footsteps install-zoology install-soot install-voodoo install-wraparoo install-cli-trooper release test test\:all test\:support test\:lang test\:package test\:proof test\:inpoots test\:roopl test\:proofread test\:boodle test\:moordown test\:workbook test\:footsteps test\:soot test\:voodoo test\:wraparoo test\:i18n test\:spool test\:zoology test\:lookup test\:loom test\:cli-trooper test\:rooc test\:cli test\:roo-cli test\:loom-cli test\:lookup-cli test\:boodle-cli test\:benchmark test\:server clean
.PHONY: $(ROO_BUILD_TARGETS)

ROO_BUILD_LOCK_PATH := $(CURDIR)/.roo-build.lock
ROO_BUILD_LOCK_SCRIPT := $(CURDIR)/cmake/rooBuildLock.cmake
ROO_BUILD_MAKEFILE := $(abspath $(firstword $(MAKEFILE_LIST)))

define ROO_RUN_WITH_BUILD_LOCK
	@cmake \
		"-DROO_BUILD_LOCK_PATH=$(ROO_BUILD_LOCK_PATH)" \
		"-DROO_BUILD_MAKEFILE=$(ROO_BUILD_MAKEFILE)" \
		"-DROO_BUILD_TARGET=$@" \
		"-DROO_MAKE_PROGRAM=$(MAKE)" \
		"-DROO_SOURCE_DIR=$(CURDIR)" \
		-P "$(ROO_BUILD_LOCK_SCRIPT)"
endef

SUPPORT_TEST_BINARY := lib/libroo-support/test/testsupport
TEST_BINARY := lib/libroo/test/testroo
BENCHMARK_TEST_BINARY := lib/libroo/test/benchmarkroo
PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage
PROOF_TEST_BINARY := pkg/proof/test/testproof
INPOOTS_TEST_BINARY := pkg/inpoots/test/testinpoots
ROOC_TEST_BINARY := bin/rooc/test/testrooc
SERVER_TEST_BINARY := lib/libroo-server/test/testserver
LOOM_BINARY := loom
LOOKUP_BINARY := lookup
BOODLE_BINARY := boodle
ROOPL_BINARY := roopl
PROOF_NATIVE_LIBRARY := libproof-native.so
INPOOTS_NATIVE_LIBRARY := libinpoots-native.so
ROOPL_NATIVE_LIBRARY := libroopl-native.so
LOOKUP_NATIVE_LIBRARY := liblookup-native.so
PROOFREAD_NATIVE_LIBRARY := libproofread-native.so
PROOFREAD_BINARY := proofread
ifeq ($(OS),Windows_NT)
  SUPPORT_TEST_BINARY := lib/libroo-support/test/testsupport.exe
  TEST_BINARY := lib/libroo/test/testroo.exe
  BENCHMARK_TEST_BINARY := lib/libroo/test/benchmarkroo.exe
  PACKAGE_TEST_BINARY := lib/libroo-package/test/testpackage.exe
  PROOF_TEST_BINARY := pkg/proof/test/testproof.exe
  INPOOTS_TEST_BINARY := pkg/inpoots/test/testinpoots.exe
  ROOC_TEST_BINARY := bin/rooc/test/testrooc.exe
  SERVER_TEST_BINARY := lib/libroo-server/test/testserver.exe
  LOOM_BINARY := loom.exe
  LOOKUP_BINARY := lookup.exe
  BOODLE_BINARY := boodle.exe
  ROOPL_BINARY := roopl.exe
  PROOF_NATIVE_LIBRARY := proof-native.dll
  INPOOTS_NATIVE_LIBRARY := inpoots-native.dll
  ROOPL_NATIVE_LIBRARY := roopl-native.dll
  LOOKUP_NATIVE_LIBRARY := lookup-native.dll
  PROOFREAD_NATIVE_LIBRARY := proofread-native.dll
  PROOFREAD_BINARY := proofread.exe
endif
ifeq ($(shell uname -s),Darwin)
  PROOF_NATIVE_LIBRARY := libproof-native.dylib
  INPOOTS_NATIVE_LIBRARY := libinpoots-native.dylib
  ROOPL_NATIVE_LIBRARY := libroopl-native.dylib
  LOOKUP_NATIVE_LIBRARY := liblookup-native.dylib
  PROOFREAD_NATIVE_LIBRARY := libproofread-native.dylib
endif

ifeq ($(OS),Windows_NT)
  DEV_NATIVE_ARTIFACT = cmake -E copy_if_different $(1) $(2)
else
  DEV_NATIVE_ARTIFACT = cmake -E create_symlink $(1) $(2)
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
	$(CURDIR)/build/pkg/inpoots/native/$(INPOOTS_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/roopl/native/$(ROOPL_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/inpoots/test/testinpoots \
	$(CURDIR)/build/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY) \
	$(CURDIR)/build/pkg/proofread/proofread \
	$(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/inpoots/native/$(INPOOTS_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/roopl/native/$(ROOPL_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY) \
	$(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY)

ifeq ($(ROO_BUILD_LOCKED),1)

configure:
	cmake -S . -B build \
		-G "$(CMAKE_GENERATOR)" \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DROO_SERVER_BUILD_TESTS=OFF

configure-server-tests:
	cmake -S . -B build \
		-G "$(CMAKE_GENERATOR)" \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
		-DCMAKE_PREFIX_PATH=$(PREFIX) \
		-DROO_SERVER_BUILD_TESTS=ON

build: configure
	cmake --build build
	$(MAKE) dev-native-package-links

bootstrap-loom: configure
	cmake --build build --target bootstrap_loom

relink: configure
	cmake -E rm -f $(RELINK_ARTIFACTS)
	cmake --build build
	$(MAKE) dev-native-package-links

dev-native-packages: configure
	cmake --build build --target proof_native inpoots_native roopl_native lookup_native proofread_native
	$(MAKE) dev-native-package-links

dev-native-package-links:
	cmake -E make_directory $(CURDIR)/pkg/proof/native
	cmake -E make_directory $(CURDIR)/pkg/inpoots/native
	cmake -E make_directory $(CURDIR)/pkg/roopl/native
	cmake -E make_directory $(CURDIR)/pkg/lookup/native
	cmake -E make_directory $(CURDIR)/pkg/proofread/native
	cmake -E rm -f $(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/inpoots/native/$(INPOOTS_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/roopl/native/$(ROOPL_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY)
	cmake -E rm -f $(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY)
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/proof/native/$(PROOF_NATIVE_LIBRARY),$(CURDIR)/pkg/proof/native/$(PROOF_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/inpoots/native/$(INPOOTS_NATIVE_LIBRARY),$(CURDIR)/pkg/inpoots/native/$(INPOOTS_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/roopl/native/$(ROOPL_NATIVE_LIBRARY),$(CURDIR)/pkg/roopl/native/$(ROOPL_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY),$(CURDIR)/pkg/lookup/native/$(LOOKUP_NATIVE_LIBRARY))
	$(call DEV_NATIVE_ARTIFACT,$(CURDIR)/build/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY),$(CURDIR)/pkg/proofread/native/$(PROOFREAD_NATIVE_LIBRARY))

stage-packages: configure
	cmake --build build --target stage_packages

package-artifacts: configure
	cmake --build build --target package_artifacts

install-packages: bootstrap-loom
	$(MAKE) package-artifacts
	cmake -E make_directory $(PACKAGE_REPOSITORY)
	@set -e; for package in $(ROO_REPOSITORY_PACKAGES); do \
		$(BOOTSTRAP_LOOM) install \
			$(PACKAGE_ARTIFACT_DIR)/$$package \
			--package-repository $(PACKAGE_REPOSITORY); \
	done

install-repository-package-%: bootstrap-loom
	cmake --build build --target package_artifact_$*
	cmake -E make_directory $(PACKAGE_REPOSITORY)
	$(BOOTSTRAP_LOOM) install \
		$(PACKAGE_ARTIFACT_DIR)/$* \
		--package-repository $(PACKAGE_REPOSITORY)

install: build
	cmake --install build --config $(BUILD_TYPE) --prefix $(PREFIX) --component Unspecified
	$(MAKE) install-packages
	$(MAKE) install-roo-lang-index
	@printf '\n'
	@printf '%s\n' '==> Roo installation completed successfully'
	@printf '    Install root: %s\n' '$(DESTDIR)$(PREFIX)'
	@printf '    Package repository: %s\n' '$(PACKAGE_REPOSITORY)'
	@printf '    Packages installed by Loom: %s\n' '$(words $(ROO_REPOSITORY_PACKAGES))'
	@printf '    Roo language index: %s\n' '$(ROO_LANG_INDEX_INSTALL_DIR)/symbols.edn'

install-loom: install-repository-package-loom bootstrap-loom
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/loom-bootstrap/build/$(LOOM_BINARY) $(PREFIX)/bin/$(LOOM_BINARY)

build-proof: build stage-packages

install-proof: install-repository-package-proof

install-inpoots: install-repository-package-inpoots

build-lookup: configure
	cmake --build build --target rooc_cli stage_packages
	./build/rooc build $(PACKAGE_STAGE)/lookup --build-dir $(CURDIR)/build/lookup-install --name lookup

build-roo-lang-index: build-lookup
	cmake -E make_directory $(ROO_LANG_INDEX_DIR)
	$(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) index --root lib/libroo/include/roo/lang --root lib/libroo/src/roo/lang --package-name roo --package-version $(ROO_LANG_INDEX_VERSION) -o $(ROO_LANG_INDEX_PATH)

build-github-pages-docs: install build-boodle
	@test -f $(ROO_LANG_INDEX_INSTALL_DIR)/symbols.edn || { \
		printf 'Missing installed Roo language index: %s\n' '$(ROO_LANG_INDEX_INSTALL_DIR)/symbols.edn' >&2; \
		exit 1; \
	}
	@set -e; for package in $(ROO_REPOSITORY_PACKAGES); do \
		found=false; \
		for index in "$(PACKAGE_REPOSITORY)/$$package"/*/symbols.edn; do \
			if [ -f "$$index" ]; then found=true; break; fi; \
		done; \
		if [ "$$found" != true ]; then \
			printf 'Missing installed package index for %s under %s\n' "$$package" '$(PACKAGE_REPOSITORY)' >&2; \
			exit 1; \
		fi; \
	done
	cmake -E make_directory $(GITHUB_PAGES_DOCS_DIR)
	$(CURDIR)/build/boodle-install/build/$(BOODLE_BINARY) generate \
		--format github-pages \
		--package-group "The Roo Language:roo" \
		--package-group "Core Libraries:loom,proof" \
		--package-group "Packages:*" \
		--out $(GITHUB_PAGES_DOCS_DIR) \
		$(GITHUB_PAGES_DOC_INDEXES)

audit-roo-lang-index: build-roo-lang-index
	$(SHELL) $(CURDIR)/scripts/audit-roo-lang-index.sh $(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) $(ROO_LANG_INDEX_PATH)

install-lookup: install-repository-package-lookup build-lookup
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/lookup-install/build/$(LOOKUP_BINARY) $(PREFIX)/bin/$(LOOKUP_BINARY)

install-roo-lang-index: audit-roo-lang-index
	cmake -E make_directory $(ROO_LANG_INDEX_INSTALL_DIR)
	cmake -E copy_if_different $(ROO_LANG_INDEX_PATH) $(ROO_LANG_INDEX_INSTALL_DIR)/symbols.edn

build-proofread: build stage-packages
	./build/rooc build $(PACKAGE_STAGE)/proofread --build-dir $(CURDIR)/build/proofread-install --name proofread

install-proofread: install-repository-package-proofread build-proofread
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/proofread-install/build/$(PROOFREAD_BINARY) $(PREFIX)/bin/$(PROOFREAD_BINARY)

build-boodle: build stage-packages
	./build/rooc build $(PACKAGE_STAGE)/boodle --build-dir $(CURDIR)/build/boodle-install --name boodle

install-boodle: install-repository-package-boodle build-boodle
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/boodle-install/build/$(BOODLE_BINARY) $(PREFIX)/bin/$(BOODLE_BINARY)

build-roopl: build stage-packages
	./build/rooc build $(PACKAGE_STAGE)/roopl --build-dir $(CURDIR)/build/roopl-install --name roopl

install-roopl: install-repository-package-roopl build-roopl
	cmake -E make_directory $(PREFIX)/bin
	cmake -E copy_if_different $(CURDIR)/build/roopl-install/build/$(ROOPL_BINARY) $(PREFIX)/bin/$(ROOPL_BINARY)

install-i18n: install-repository-package-i18n

install-moordown: install-repository-package-moordown

install-spool: install-repository-package-spool

install-workbook: install-repository-package-workbook

install-footsteps: install-repository-package-footsteps

install-zoology: install-repository-package-zoology

install-soot: install-repository-package-soot

install-voodoo: install-repository-package-voodoo

install-wraparoo: install-repository-package-wraparoo

install-cli-trooper: install-repository-package-cli-trooper

release:
	sh $(CURDIR)/tools/release/package.sh $(VERSION)

test: test\:support test\:lang test\:package test\:proof test\:inpoots test\:roopl test\:proofread test\:boodle test\:moordown test\:workbook test\:footsteps test\:soot test\:voodoo test\:wraparoo test\:i18n test\:spool test\:zoology test\:lookup test\:loom test\:cli-trooper test\:rooc
test: test\:cli

test\:all: test\:support test\:lang test\:package test\:proof test\:inpoots test\:roopl test\:proofread test\:boodle test\:moordown test\:workbook test\:footsteps test\:soot test\:voodoo test\:wraparoo test\:i18n test\:spool test\:zoology test\:lookup test\:loom test\:cli-trooper test\:rooc test\:cli test\:server

test\:support: build
	cmake --build build --target testsupport
	./build/$(SUPPORT_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:lang: build
	cmake --build build --target testroo
	./build/$(TEST_BINARY) $(GTEST_FILTER_ARG)

test\:package: build
	cmake --build build --target testpackage
	./build/$(PACKAGE_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:proof: build
	cmake --build build --target testproof
	./build/$(PROOF_TEST_BINARY) $(GTEST_FILTER_ARG)
	cd $(PACKAGE_STAGE)/proof/test && $(CURDIR)/build/roo proof

test\:inpoots: build
	cmake --build build --target testinpoots
	./build/$(INPOOTS_TEST_BINARY) $(GTEST_FILTER_ARG)
	cd $(PACKAGE_STAGE)/inpoots/test && $(CURDIR)/build/roo proof

test\:roopl: build stage-packages
	cd $(PACKAGE_STAGE)/roopl/test && $(CURDIR)/build/roo proof

test\:proofread: configure
	cmake --build build --target roo_cli
	cmake --build build --target stage_packages
	cd $(PACKAGE_STAGE)/proofread/test && $(CURDIR)/build/roo proof

test\:boodle: build stage-packages
	cd $(PACKAGE_STAGE)/boodle/test && $(CURDIR)/build/roo proof

test\:moordown: build stage-packages
	cd $(PACKAGE_STAGE)/moordown/test && $(CURDIR)/build/roo proof

test\:workbook: build stage-packages
	cd $(PACKAGE_STAGE)/workbook/test && $(CURDIR)/build/roo proof

test\:footsteps: build stage-packages
	cd $(PACKAGE_STAGE)/footsteps/test && $(CURDIR)/build/roo proof

test\:soot: build stage-packages
	cd $(PACKAGE_STAGE)/soot/test && $(CURDIR)/build/roo proof

test\:voodoo: build stage-packages
	cd $(PACKAGE_STAGE)/voodoo/test && $(CURDIR)/build/roo proof

test\:wraparoo: build stage-packages
	cd $(PACKAGE_STAGE)/wraparoo/test && $(CURDIR)/build/roo proof

test\:i18n: build stage-packages
	cd $(PACKAGE_STAGE)/i18n/test && $(CURDIR)/build/roo proof

test\:spool: build stage-packages
	cd $(PACKAGE_STAGE)/spool/test && $(CURDIR)/build/roo proof

test\:zoology: build stage-packages
	cd $(PACKAGE_STAGE)/zoology/test && $(CURDIR)/build/roo proof

test\:lookup: build stage-packages
	cd $(PACKAGE_STAGE)/lookup/test && $(CURDIR)/build/roo proof

test\:loom: build stage-packages
	cd $(PACKAGE_STAGE)/loom/test && $(CURDIR)/build/roo proof

test\:cli-trooper: build stage-packages
	cd $(PACKAGE_STAGE)/cli-trooper/test && $(CURDIR)/build/roo proof

test\:rooc: build stage-packages
	cmake --build build --target testrooc
	./build/$(ROOC_TEST_BINARY) $(GTEST_FILTER_ARG)

test\:cli: test\:roo-cli test\:loom-cli test\:lookup-cli test\:boodle-cli

test\:roo-cli: build stage-packages
	ROO_PACKAGE_STAGE_ROOT=$(PACKAGE_STAGE) sh $(CURDIR)/bin/roo/test/run-cli-tests.sh $(CURDIR)

test\:loom-cli: build stage-packages
	ROO_PACKAGE_STAGE_ROOT=$(PACKAGE_STAGE) sh $(CURDIR)/pkg/loom/test/run-cli-tests.sh $(CURDIR)

test\:lookup-cli: build stage-packages
	ROO_PACKAGE_STAGE_ROOT=$(PACKAGE_STAGE) sh $(CURDIR)/pkg/lookup/test/run-cli-tests.sh $(CURDIR)

test\:boodle-cli: build stage-packages
	ROO_PACKAGE_STAGE_ROOT=$(PACKAGE_STAGE) sh $(CURDIR)/pkg/boodle/test/run-cli-tests.sh $(CURDIR)

test\:benchmark: configure
	cmake --build build --target benchmarkroo
	./build/$(BENCHMARK_TEST_BINARY) --benchmark $(GTEST_FILTER_ARG)

test\:server: configure-server-tests
	cmake --build build --target testserver
	./build/$(SERVER_TEST_BINARY) $(GTEST_FILTER_ARG)

clean:
	rm -rf build

else

$(ROO_BUILD_TARGETS):
	$(ROO_RUN_WITH_BUILD_LOCK)

endif
