# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -fsanitize=address -lm -g
LDFLAGS = -fsanitize=address -lm
LDLIBS =

SRC = xor.c helper.c network.c ocr.c main.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
