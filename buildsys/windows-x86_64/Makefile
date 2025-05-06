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
	$(MAKE) -C source build
	$(CC) $(RELEASE_FLAGS) source/*.c -o binary/build/utasm-release binary/build/*.o $(INCLUDES) $(LIBRARIES)
	cp binary/build/utasm-release.exe binary/release/utasm.exe

package:
	cd ./binary && zip utasm.zip assets/*
	cd ./binary/release && zip ../utasm.zip utasm.exe *.dll

clean:
ifneq (, $(wildcard ./binary/build/utasm.exe))
	rm binary/build/utasm.exe
endif
ifneq (, $(wildcard ./binary/build/utasm-release.exe))
	rm binary/build/utasm-release.exe
endif
ifneq (, $(wildcard ./binary/build/*.o))
	rm binary/build/*.o
endif

stop-use:
	rm ./vars.mk ./Makefile
