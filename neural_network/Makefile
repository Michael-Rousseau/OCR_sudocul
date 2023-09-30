# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -g -fsanitize=address -lm
LDFLAGS = -fsanitize=address -lm
LDLIBS =

SRC = main.c cost.c back_propagation.c sigmoid.c network.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: network.o sigmoid.o main.o cost.o back_propagation.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
