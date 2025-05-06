MAKE=make

CC=gcc
LIBRARIES=-lGL -lglfw
INCLUDES=-Isource

FLAGS=-g -std=c99 -pedantic -Wall -Wextra -fsanitize=address
RELEASE_FLAGS=-std=c99 -pedantic -Wall
