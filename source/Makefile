include ../vars.mk

build: simfile/*.c window/*.c
	$(MAKE) -C graphics build
	$(CC) -c simfile/*.c -o ../binary/build/simfile.o
	$(CC) -c window/*.c -o ../binary/build/window.o
