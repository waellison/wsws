CFLAGS=--std=c99 -Wall -Werror -pedantic -I../include -g -DDEBUG
LFLAGS=
OBJS=socketfun.o

all: wsws tester

wsws: main.o socketfun.o
	$(CC) $(LFLAGS) -o wsws main.o $(OBJS)

tester: test.o socketfun.o
	$(CC) $(LFLAGS) -o tester test.o $(OBJS)

.SUFFIXES: .c.o
.c.o: $*.c
	$(CC) $(CFLAGS) -c $*.c

.PHONY: clean
clean:
	-rm -rf *.o a.out core wsws tester

