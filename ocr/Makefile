# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -ggdb3 -std=c11 -fsanitize=address -lm
LDFLAGS = -fsanitize=address -lm
LDLIBS =

SRC = main.c helper.c network.c ocr.c sigmoid.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
