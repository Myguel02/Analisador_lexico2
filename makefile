CC=gcc
CFLAGS=-Wall -g -std=c99
OBJS=token.o lexer.o ast.o parser.o main.o

all: analisador.exe

analisador.exe: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o analisador.exe
