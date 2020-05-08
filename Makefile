CC=g++
CFLAGS= -std=c++11 -g -Wall

all: camera gate

camera: camera.cpp
	$(CC) $(CFLAGS) -o camera camera.cpp

gate: gate.cpp
	$(CC) $(CFLAGS) -o gate gate.cpp

clean:
	rm camera gate
