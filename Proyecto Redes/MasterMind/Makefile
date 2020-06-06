CC=g++
CFLAGS=-g -I. 
DEPS = Socket.h XLDisplay.h Game.h 
OBJ = Socket.o XLDisplay.o Game.o
LIBS=-lpthread -lm -lX11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: server client

server: $(OBJ) MMServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ) MMClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o server client