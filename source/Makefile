include ../vars.mk

build: simfile/*.c window/*.c
	$(MAKE) -C graphics build
	$(CC) -c simfile/simfile.c -o ../binary/build/simfile.o
	$(CC) -c simfile/sm.c -o ../binary/build/simfile_sm.o
	$(CC) -c simfile/ssc.c -o ../binary/build/simfile_ssc.o
	$(CC) -c simfile/usm.c -o ../binary/build/simfile_usm.o
	$(CC) -c window/*.c -o ../binary/build/window.o
