
all:
	cd src && qmake
	make -C src
	make -C tools

clean:
	make -C src clean
	make -C tools clean
