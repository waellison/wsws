CFLAGS=--std=c99 -Wall -Werror -pedantic -I./include -g -DDEBUG
LFLAGS=
OBJS=main.o
TARGET=wsws

all: $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS)

.SUFFIXES: .c.o
.c.o: $*.c
	$(CC) $(CFLAGS) -c $*.c

.PHONY: clean
clean:
	-rm -rf *.o a.out core wsws

