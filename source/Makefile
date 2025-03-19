include ../vars.mk

build: clean simfile/*.c window/*.c
	$(CC) -c simfile/*.c -o ../binary/simfile.o
	$(CC) -c window/*.c -o ../binary/window.o
clean: 
ifneq (, $(wildcard ../binary/*.o))
	rm ../binary/*.o
endif
