# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -g
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: detection

SRC = detection.c hough.c
DEPS = ${SRC:.c=.d}
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

detection: detection.o hough.o 

clean:
	${RM} ${DEPS}
	${RM} ${OBJ}
	${RM} ${EXE}

# END
