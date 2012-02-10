# Makefile for tjer
CC=gcc
OBJECTS=main.o
PREFIX=bin/

all: $(PREFIX)tjer

$(PREFIX)tjer: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PREFIX)tjer

main.o: main.c
	$(CC) -c main.c -o main.o

