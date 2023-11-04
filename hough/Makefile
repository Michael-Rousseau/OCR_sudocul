# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` 
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: main

SRC = main.c image.c joanneh.c
DEPS = ${SRC:.c=.d}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

main: image.o main.o joanneh.o

clean:
	${RM} ${DEPS}
	${RM} ${OBJ}
	${RM} ${EXE}

# END
