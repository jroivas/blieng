topdir ?= $(shell pwd)

PRODUCT = blieng
REL ?= etp5

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

.PHONY: all prepare dist build-$(TARGET) test

build-$(TARGET): prepare
	@mkdir -p "$(BUILDDIR)/$(PRODUCT)"
	cd "$(BUILDDIR)/$(PRODUCT)" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/$(PRODUCT)"
	"$(topdir)"/tools/build/make.sh $(TARGET) -C "$(BUILDDIR)/$(PRODUCT)"

$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a: build-$(TARGET)
	@if [ -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" ] ; then cp -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" "$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a" ; fi

test: $(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a
	@mkdir -p "$(BUILDDIR)/blieng/tests"
	cd "$(BUILDDIR)/blieng/tests" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/blieng/tests"
	make -C "$(BUILDDIR)/blieng/tests"
	"$(BUILDDIR)/blieng/tests/tests" 2> test_result.xml

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

upload: dist
	./tools/build/binrep_upload.sh dist/$(OUT).$(DISTEXT) $(PRODUCT) $(BITS) $(TRACK)
