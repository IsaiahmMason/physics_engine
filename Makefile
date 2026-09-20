CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lSDL2

physics_engine: main.c
	$(CC) main.c -o physics_engine $(LIBS)

clean:
	rm -f physics_engine

run:
	./physics_engine