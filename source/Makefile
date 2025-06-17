include ../vars.mk

build: editor/*.c graphics/*.c simfile/*.c window/*.c
	$(MAKE) -C graphics build

	$(CC) -g -c editor/editor.c -o ../binary/build/editor.o -I./

	$(CC) -g -c graphics/graphics.c -o ../binary/build/graphics.o -I./

	$(CC) -g -c simfile/simfile.c -o ../binary/build/simfile.o -I./
	$(CC) -g -c simfile/sm.c -o ../binary/build/simfile_sm.o -I./
	$(CC) -g -c simfile/ssc.c -o ../binary/build/simfile_ssc.o -I./
	$(CC) -g -c simfile/usm.c -o ../binary/build/simfile_usm.o -I./

	$(CC) -g -c window/*.c -o ../binary/build/window.o -I./

build-release: editor/*.c graphics/*.c simfile/*.c window/*.c
	$(MAKE) -C graphics build

	$(CC) -c editor/editor.c -o ../binary/build/editor.o -I./

	$(CC) -c graphics/graphics.c -o ../binary/build/graphics.o -I./

	$(CC) -c simfile/simfile.c -o ../binary/build/simfile.o -I./
	$(CC) -c simfile/sm.c -o ../binary/build/simfile_sm.o -I./
	$(CC) -c simfile/ssc.c -o ../binary/build/simfile_ssc.o -I./
	$(CC) -c simfile/usm.c -o ../binary/build/simfile_usm.o -I./

	$(CC) -c window/*.c -o ../binary/build/window.o -I./
