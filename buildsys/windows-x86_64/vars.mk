MAKE=make

CC=x86_64-w64-mingw32-gcc
LIBRARIES=-lopengl32 -lglfw3dll
INCLUDES=-Isource

FLAGS=-g -std=c99 -pedantic -Wall -Wextra
RELEASE_FLAGS=-std=c99 -pedantic -Wall
