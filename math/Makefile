# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -Werror 
LDFLAGS =
LDLIBS =

SRC = matrices.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.d}

matrices: matrices.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} matrices

# END
