CC=g++
CFLAGS=-g -Wall

all: camera bramka

camera: camera.cpp
	$(CC) $(CFLAGS) -o camera camera.cpp

bramka: bramka.cpp
	$(CC) $(CFLAGS) -o bramka bramka.cpp

clean:
	rm camera bramka
