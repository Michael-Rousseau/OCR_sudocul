# Makefile

CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

OBJ = image.o renderer.o
DEP = ${OBJ:.o=.d}
EXE = ${OBJ:.o=}

all: renderer

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} ${EXE} main

-include ${DEP}

# END
