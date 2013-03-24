
all:
	cd src && qmake
	make -C src

clean:
	make -C src clean
