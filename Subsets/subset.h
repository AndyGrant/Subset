#pragma once

#include <stdint.h>

#define CHUNK_SIZE (64)
#define NUM_CHUNKS (1 + N / CHUNK_SIZE)
#define STACK_SIZE (N * (N+1) / 2)

typedef uint64_t Bitstring[NUM_CHUNKS];

typedef struct Subset {
    int length, values[N];
    Bitstring invalid;
} Subset;
