# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -lm
LDFLAGS = -fsanitize=address -lm
LDLIBS =

SRC = main.c sigmoid.c matrix.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: matrix.o sigmoid.o main.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
