topdir ?= $(shell pwd)

PRODUCT = blieng
REL ?= etp5

DEBUG ?= release
TRACK ?= dev
BITS ?= $(shell uname -m|sed -e 's/x86_64/64bit/' -e 's/x86/32bit/')
TARGET ?= linux-$(BITS)

ifeq ($(TARGET),win)
BUILDDIR ?= win
DISTEXT ?= zip
DISTCMD ?= zip -qr
else
BUILDDIR ?= linux-$(BITS)
DISTEXT ?= tar.gz
DISTCMD ?= tar czf
endif

OUT ?= $(PRODUCT)-$(REL)-$(TARGET)

all: prepare | $(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a
	@echo Done build for $(TARGET)

prepare:
	./tools/fetch_build.sh

.PHONY: all prepare dist build-$(TARGET) test

build-$(TARGET):
	@mkdir -p "$(BUILDDIR)/$(PRODUCT)"
	cd "$(BUILDDIR)/$(PRODUCT)" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/$(PRODUCT)"
	"$(topdir)"/tools/build/make.sh $(TARGET) -C "$(BUILDDIR)/$(PRODUCT)"

$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a: build-$(TARGET)
	@if [ -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" ] ; then cp -f "$(BUILDDIR)/$(PRODUCT)/$(DEBUG)/lib$(PRODUCT).a" "$(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a" ; fi

test: $(BUILDDIR)/$(PRODUCT)/lib$(PRODUCT).a
	@mkdir -p "$(BUILDDIR)/test"
	cd "$(BUILDDIR)/test" && "$(topdir)"/tools/build/qmake.sh $(TARGET) "$(topdir)/test"
	make -C "$(BUILDDIR)/test"

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

cppcheck:
	cppcheck --enable=all -I. -I$(PRODUCT) --inconclusive --inline-suppr --check-config --xml-version=2 $(PRODUCT) 2> cppcheck_report_$(PRODUCT).xml
