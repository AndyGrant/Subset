#include <stdint.h>

#ifndef SUBSET_H
#define SUBSET_H

#define CHUNKS   (1 + N/64)
#define STACK_SIZE  ((N * (N+1)) / 2)

typedef uint64_t Bitstring[CHUNKS];

void printBitstring(Bitstring bs);
void setBit(Bitstring bs, int bit);
int getBit(Bitstring bs, int bit);
void orBitstrings(Bitstring lhs, Bitstring rhs);

typedef struct Subset {
    
    Bitstring invalid;
    int length;
    int values[N];
    
} Subset;

void newSubset(Subset * ss, int value);
void extendSubset(Subset * ss, Subset * base, int value);
int relevantExcluded(Subset * ss);

int formsSquare(int a, int b, int c);
void setupInvalidTable();

int isPrime(int x);
void setupKnownBitstring();

void printSolution(Subset * ss);

#endif
