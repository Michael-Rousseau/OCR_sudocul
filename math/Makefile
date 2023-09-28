# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS =

SRC = main.c matrix.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: matrix.o main.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
