include vars.mk

setup:
ifeq (, $(wildcard ./binary))
	mkdir binary
endif
build: setup clean
	$(MAKE) -C source build
	$(CC) -fsanitize=address source/*.c -o binary/utasm binary/*.o $(LIBRARIES)  
clean:
	$(MAKE) -C source clean
ifneq (, $(wildcard ./binary/utasm))
	rm binary/utasm
endif
