
N = 10

CC = gcc

CFLAGS = -Wall -Wextra -Wshadow -pedantic

OPTFLAGS = -O3 -funroll-loops

SRC = Subset.c

EXE = Subset.exe

all:
	$(CC) $(CFLAGS) $(OPTFLAGS) $(SRC) -DN=$(N) -o $(EXE)