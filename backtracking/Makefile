# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror 
LDFLAGS =
LDLIBS =

SRC = allocator.c filestream.c main.c solver.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main

# END
