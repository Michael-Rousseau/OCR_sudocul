# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -ggdb3 -fsanitize=address -lm
LDFLAGS = -fsanitize=address -lm
LDLIBS =

SRC = xor.c helper.c network.c ocr.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

xor: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
