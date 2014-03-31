topdir ?= $(shell pwd)

PRODUCT = blieng
REL ?= etp6

DEBUG ?= release
TRACK ?= dev
BITS ?= $(shell uname -m|sed -e 's/x86_64/64bit/' -e 's/x86/32bit/' -e 's/i.86/32bit/')
TARGET ?= linux-$(BITS)

ifeq ($(TARGET),win)
BUILDDIR ?= win
BITS = win32
DISTEXT ?= zip
DISTCMD ?= zip -qr
UNDISTCMD ?= unzip -q -o
UNDISTDIR ?= -d
UNDISTEXTRA =
else
BUILDDIR ?= linux-$(BITS)
DISTEXT ?= tar.gz
DISTCMD ?= tar czf
UNDISTCMD ?= tar xzf
UNDISTDIR ?= -C
UNDISTEXTRA ?= --strip-components=1
endif

OUT ?= $(PRODUCT)-$(REL)-$(TARGET)

all: $(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a
	@echo Done build for $(TARGET)

prepare:
	./tools/fetch_build.sh

.PHONY: all prepare dist build-$(TARGET) test doc

build-$(TARGET): prepare
	@mkdir -p "$(BUILDDIR)/$(PRODUCT)"
	cd "$(BUILDDIR)/$(PRODUCT)" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/$(PRODUCT)"
	"$(topdir)"/tools/build/make.sh $(TARGET) -C "$(BUILDDIR)/$(PRODUCT)"

$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a: build-$(TARGET)
	@if [ -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" ] ; then cp -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" "$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a" ; fi

test: $(BUILDDIR)/$(PRODUCT)/tests/tests | $(BUILDDIR)/$(PRODUCT)/tests
	"$(BUILDDIR)/$(PRODUCT)/tests/tests" 2> test_result.xml

$(BUILDDIR)/$(PRODUCT)/tests:
	@mkdir -p $(BUILDDIR)/$(PRODUCT)/tests

$(BUILDDIR)/$(PRODUCT)/tests/tests: $(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a | $(BUILDDIR)/$(PRODUCT)/tests
	cd "$(BUILDDIR)/$(PRODUCT)/tests" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/$(PRODUCT)/tests"
	make -C "$(BUILDDIR)/$(PRODUCT)/tests"

dist:
	rm -rf dist/$(OUT)
	mkdir -p dist/$(OUT)
	mkdir -p dist/$(OUT)/include
	cp "$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a" dist/$(OUT)
	cp $(PRODUCT)/*.h dist/$(OUT)/include
	cd dist && $(DISTCMD) $(OUT).$(DISTEXT) $(OUT)

clean:
	@echo Cleaning...
	if [ -d "$(BUILDDIR)/$(PRODUCT)" ] ; then make -C "$(BUILDDIR)/$(PRODUCT)" clean ; fi
	if [ -d "$(BUILDDIR)/test" ] ; then make -C "$(BUILDDIR)/test" clean ; fi

check:
	cppcheck --enable=all -I. -I$(PRODUCT) --inconclusive --inline-suppr --check-config --xml-version=2 $(PRODUCT) 2> cppcheck_report_$(PRODUCT).xml

doc: doc/html/index.html

doxygen:
	doxygen doc/config.doxy

doc/html/index.html: doxygen

doc/latex/refman.pdf: doxygen
	cd doc/latex && pdflatex -interaction=nonstopmode refman

upload: dist
	./tools/build/binrep_upload.sh dist/$(OUT).$(DISTEXT) $(PRODUCT) $(BITS) $(TRACK)
