include ../vars.mk

build: simfile/*.c window/*.c
	$(MAKE) -C graphics build
	$(CC) -c simfile/simfile.c -o ../binary/build/simfile.o -I./
	$(CC) -c simfile/sm.c -o ../binary/build/simfile_sm.o -I./
	$(CC) -c simfile/ssc.c -o ../binary/build/simfile_ssc.o -I./
	$(CC) -c simfile/usm.c -o ../binary/build/simfile_usm.o -I./
	$(CC) -c window/*.c -o ../binary/build/window.o -I./
