include vars.mk

setup:
ifeq (, $(wildcard ./binary/build))
	mkdir -p binary/build
endif
ifeq (, $(wildcard ./binary/release))
	mkdir -p binary/release
endif

build: setup clean
	$(MAKE) -C source build
	$(CC) $(FLAGS) source/*.c -o binary/build/utasm binary/build/*.o $(INCLUDES) $(LIBRARIES)
build-release: setup clean
	$(MAKE) -C source build-release
	$(CC) $(RELEASE_FLAGS) source/*.c -o binary/build/utasm-release binary/build/*.o $(INCLUDES) $(LIBRARIES)
	cp binary/build/utasm-release binary/release/utasm

package:
	cd ./binary && tar -cf utasm.tar.xz assets/*
	cd ./binary/release && tar -rf ../utasm.tar.xz utasm

clean:
ifneq (, $(wildcard ./binary/build/utasm))
	rm binary/build/utasm
endif
ifneq (, $(wildcard ./binary/build/utasm-release))
	rm binary/build/utasm-release
endif
ifneq (, $(wildcard ./binary/build/*.o))
	rm binary/build/*.o
endif

stop-use:
	rm ./vars.mk ./Makefile
