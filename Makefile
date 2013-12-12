cores := $(shell cat /proc/cpuinfo | grep -i 'processor' | wc -l)
coreflags = -j$(cores)
#coreflags = -j2
topdir := $(shell pwd)
VERS=etp4
PRODUCT="blieng"

native: linux

all: linux win translations

prepare:
	./tools/fetch_build.sh

linux: prepare linux-blieng

linux-prep:
	@./tools/build/clean.sh linux
	@echo "linux" > build.last

linux-blieng: linux-prep
	cd blieng && "$(topdir)"/tools/build/qmake.sh linux
	 "$(topdir)"/tools/build/make.sh linux -C blieng $(coreflags)

linux-dist:
	mkdir -p dist/$(PRODUCT)-$(VERS)-linux
	mkdir -p dist/$(PRODUCT)-$(VERS)-linux/include
	cp README dist/$(PRODUCT)-$(VERS)-linux/README
	cp -f blieng/libblieng.a dist/$(PRODUCT)-$(VERS)-linux/
	cp -f blieng/*.h dist/$(PRODUCT)-$(VERS)-linux/include/
	cd dist && tar czf $(PRODUCT)-$(VERS)-linux.tar.gz $(PRODUCT)-$(VERS)-linux

win: prepare
	"$(topdir)"/tools/build/build.sh win-build

win-prep:
	@./tools/build/clean.sh win
	@echo "win" > build.last

win-build: win-blieng

win-blieng: win-prep
	cd blieng && "$(topdir)"/tools/build/qmake.sh win
	"$(topdir)"/tools/build/make.sh win -C blieng $(coreflags)

win-dist:
	mkdir -p dist/$(PRODUCT)-$(VERS)-win
	#mkdir -p dist/$(PRODUCT)-$(VERS)-win/debug
	#mkdir -p dist/$(PRODUCT)-$(VERS)-win/release
	mkdir -p dist/$(PRODUCT)-$(VERS)-win/include
	cp README dist/$(PRODUCT)-$(VERS)-win/README
	#cp -f blieng/debug/libblieng.a dist/$(PRODUCT)-$(VERS)-win/debug/
	#cp -f blieng/release/libblieng.a dist/$(PRODUCT)-$(VERS)-win/release/
	cp -f blieng/release/libblieng.a dist/$(PRODUCT)-$(VERS)-win/
	cp -f blieng/*.h dist/$(PRODUCT)-$(VERS)-win/include/
	cd dist && zip -r $(PRODUCT)-$(VERS)-win.zip $(PRODUCT)-$(VERS)-win

cppcheck:
	cppcheck --enable=all -I. -Iblieng --inconclusive --inline-suppr --check-config --xml-version=2 blieng 2> cppcheck_report_blieng.xml

test:
	cd blieng/tests && "$(topdir)"/tools/build/qmake.sh linux
	"$(topdir)"/tools/build/make.sh linux -C blieng/tests
	blieng/tests/tests 2> test_result.xml

clean:
	make -C blieng clean

dist-clean:
	rm -rf dist
