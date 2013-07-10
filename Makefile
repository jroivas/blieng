cores := $(shell cat /proc/cpuinfo | grep -i 'processor' | wc -l)
coreflags = -j$(cores)
#coreflags = -j2
topdir := $(shell pwd)
VERS=etp3

all: linux win

linux: linux-prep linux-blieng linux-src linux-tools

linux-prep:
	./tools/winbuild/clean.sh linux
	echo "linux" > build.last

linux-blieng:
	cd blieng && qmake
	make -C blieng $(coreflags)

linux-src:
	cd src && qmake
	make -C src $(coreflags)

linux-tools: linux-map-editor

linux-map-editor:
	cd tools/map-editor && qmake
	make -C tools/map-editor $(coreflags)

linux-item-editor:
	cd tools/item-editor && qmake
	make -C tools/item-editor $(coreflags)

linux-dist:
	mkdir -p dist/zombiebli-$(VERS)-linux
	cp -f src/zombiebli dist/zombiebli-$(VERS)-linux/
	cp -f tools/map-editor/map-editor dist/zombiebli-$(VERS)-linux
	cp -rf data dist/zombiebli-$(VERS)-linux/
	cd dist && tar czf zombiebli-$(VERS)-linux.tar.gz zombiebli-$(VERS)-linux

win: win-prep
	$(topdir)/tools/winbuild/winbuild.sh win-build

win-prep:
	./tools/winbuild/clean.sh win
	echo "win" > build.last

win-build: win-blieng win-src win-tools

win-blieng:
	cd blieng && $(topdir)/tools/winbuild/winqmake.sh
	$(topdir)/tools/winbuild/winmake.sh -C blieng $(coreflags)

win-src:
	cd src && $(topdir)/tools/winbuild/winqmake.sh
	$(topdir)/tools/winbuild/winmake.sh -C src $(coreflags)

win-tools: win-map-editor

win-map-editor:
	cd tools/map-editor && $(topdir)/tools/winbuild/winqmake.sh
	$(topdir)/tools/winbuild/winmake.sh -C tools/map-editor $(coreflags)

win-item-editor:
	cd tools/item-editor && $(topdir)/tools/winbuild/winqmake.sh
	$(topdir)/tools/winbuild/winmake.sh -C tools/item-editor $(coreflags)

win-dist:
	mkdir -p dist/zombiebli-$(VERS)-win
	cp -f src/release/zombiebli.exe dist/zombiebli-$(VERS)-win/
	cp -f tools/map-editor/release/map-editor.exe dist/zombiebli-$(VERS)-win
	cp -rf data dist/zombiebli-$(VERS)-win/
	cd dist && zip -r -9 -q zombiebli-$(VERS)-win.zip zombiebli-$(VERS)-win

clean:
	make -C blieng clean
	make -C src clean
	make -C tools/map-editor clean
	make -C tools/item-editor clean

dist-clean:
	rm -rf dist
