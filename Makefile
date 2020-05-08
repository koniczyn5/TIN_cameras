CC=g++
CFLAGS=-g -Wall

all: camera gate

camera: camera.cpp
	$(CC) $(CFLAGS) -o camera camera.cpp

gate: gate.cpp
	$(CC) $(CFLAGS) -o bramka bramka.cpp

clean:
	rm camera gate
