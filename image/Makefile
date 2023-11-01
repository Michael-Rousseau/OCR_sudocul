# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = -lm
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: main

SRC = main.c image.c hough1.c
DEPS = ${SRC:.c=.d}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

main: image.o main.o hough1.o

clean:
	${RM} ${DEPS}
	${RM} ${OBJ}
	${RM} ${EXE}

# END
