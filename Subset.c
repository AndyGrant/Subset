#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Subset.h"

// Given a the first two items in a triple (A, B, C),
// we can index by A and by B, and get a Bitstring with
// a bit sent for each value of C which creates a triplet
// which cannot be in the subset, due to forming a square
Bitstring InvalidTable[N+1][N+1];

// Bits to represent the values we know will be in the
// subset. These values will be primes greater than N/2
Bitstring Knowns;
int KnownLength = 0;

int main(){
    
    setupInvalidTable();
    setupKnownBitstring();
    
    // Stack Size is, at a max, the sum of [1..N]
    Subset * stack = malloc(sizeof(Subset) * STACK_SIZE);
    int stackindex = 0;
    
    printf("Known Values: ");
    printBitstring(Knowns);
    printf("\n");

    // Search for a subset of the biggest size possible, starting with
    // size N. We skip over the values we already know will be in the set
    for (int target = N - KnownLength; target > 0; target--){
        
        // Add a Subset starting with each value [1..N] which
        // is not known to be in the final largest subset
        for (int i = N; i > 0; i--)
            if (!getBit(Knowns, i))
                newSubset(&(stack[stackindex++]), i);
            
        // Exhaust the stack
        while (stackindex != 0){
            
            Subset top = stack[--stackindex];
            
            if (top.length == 1)
                printf("\rSearching |S| = %d, S[0] = %d", target + KnownLength, top.values[0]);
            
            // Solution found with proper size
            if (top.length == target){
                printSolution(&top);
                exit(0);
            }
            
            // Calculate the best case. ReleventExcluded gives the number
            // of numbrs between the current max of the subset, and N, that
            // are not currently known to be unable to be added to the set
            int bestcase = top.length + N
                         - top.values[top.length-1]
                         - relevantExcluded(&top);
                         
            if (bestcase < target)
                continue;
                         
            // For each value from [maxOfTop+1..N], make a new Subset
            // as an extension of top. Skip over values which are known
            // to be invalid for a final subset.
            for (int i = N; i > top.values[top.length-1]; i--)
                if (!getBit(top.invalid, i))
                    extendSubset(&(stack[stackindex++]), &top, i);
                
        }
        
        printf("\n");
    }
}


 /* Functions for Bitstrings */
 
void printBitstring(Bitstring bs){
    for (int i = 1; i < (CHUNKS * 64); i++)
        if (getBit(bs, i)) printf("%d ", i);
    printf("\n");
}

void setBit(Bitstring bs, int bit){
    int index = bit / 64;
    int offset = bit % 64;
    uint64_t mask = 1ull << offset;
    bs[index] |= mask;
}

int getBit(Bitstring bs, int bit){
    int index = bit / 64;
    int offset = bit % 64;
    uint64_t mask = 1ull << offset;
    return (bs[index] & mask) >> offset;
}

void orBitstrings(Bitstring lhs, Bitstring rhs){
    for (int i = 0; i < CHUNKS; i++)
        lhs[i] |= rhs[i];
}


/* Functions for Subset structs */

void newSubset(Subset * ss, int value){
    memcpy(ss->invalid, Knowns, CHUNKS * sizeof(uint64_t));
    ss->length = 1;
    ss->values[0] = value;
}

void extendSubset(Subset * ss, Subset * base, int value){
    memcpy(ss->invalid, base->invalid, CHUNKS * sizeof(uint64_t));
    ss->length = base->length + 1;
    memcpy(ss->values, base->values, N * sizeof(int));
    ss->values[base->length] = value;
    // Update the invalid bitstring to account for the new value
    for (int i = 0; i < base->length; i++)
        orBitstrings(ss->invalid, InvalidTable[ss->values[i]][value]);
}

int relevantExcluded(Subset * ss){
    
    int start = 1 + ss->values[ss->length-1];
    
    int partialindex = start / 64;
    int partialoffset = start % 64;
    
    // Only run popcount on bits [start..N]
    int count = __builtin_popcountll(ss->invalid[partialindex] >> partialoffset);
    for (int i = partialindex + 1; i < CHUNKS; i++)
        count += __builtin_popcountll(ss->invalid[i]);
    
    return count;
}

/* Functions for identifiying invalid triplets */

int formsSquare(int a, int b, int c){
    int product = a * b * c;
    int root = (int)sqrt(product);
    return root * root == product;
}

void setupInvalidTable(){
    for (int a = 1; a <= N; a++)
        for (int b = 1; b <= N; b++)
            for (int c = 1; c <= N; c++)
                if (formsSquare(a, b, c))
                    setBit(InvalidTable[a][b], c);
}


/* Functions for indentifying known values */

int isPrime(int x){
    if (x == 1) return 0;
    if (x == 2) return 1;
    for (int i = 2; i < x; i++)
        if (x % i == 0) return 0;
    return 1;
}

void setupKnownBitstring(){
    for (int i = 1 + N/2; i <= N; i++){
        if (isPrime(i)){
            setBit(Knowns, i);
            KnownLength += 1;
        }
    }
}

/* Printing the Knowns with the found subset */

void printSolution(Subset * ss){
    
    
    int magnitude = ss->length + KnownLength;
    int i, j, temp, idx = 0, answer[magnitude];
    
    // Concan the known values with the found subset
    for (i = 1; i <= N; i++)
        if (getBit(Knowns, i))
            answer[idx++] = i;
    memcpy(answer + idx, ss->values, sizeof(int) * ss->length);
    
    // Sort the subset we have greated
    for (i = 0; i < magnitude; i++){
        for (j = i+1; j < magnitude; j++){
            if (answer[j] < answer[i]){
                temp = answer[i];
                answer[i] = answer[j];
                answer[j] = temp;
            }
        }
    }
    
    printf("|S| = %d; S = {", magnitude);
    for (i = 0; i < magnitude; i++)
        printf(" %d ", answer[i]);
    printf("}\n");

}