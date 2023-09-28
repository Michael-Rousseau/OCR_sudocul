
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SOURCES = main.c linkedlist.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = test

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
