# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -g
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: main

SRC = main.c image.c rotation.c detection.c square_detection.c
DEPS = ${SRC:.c=.d}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

main: image.o main.o rotation.o detection.o square_detection.o

clean:
	${RM} ${DEPS}
	${RM} ${OBJ}
	${RM} ${EXE}

# END

