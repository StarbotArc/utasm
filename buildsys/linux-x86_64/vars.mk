MAKE=make

CC=gcc
LIBRARIES=-lGL -lglfw
INCLUDES=-Isource

FLAGS=-g -O0 -std=c99 -pedantic -Wall -Wextra -fsanitize=address
RELEASE_FLAGS=-std=c99 -pedantic -Wall -DNDEBUG
