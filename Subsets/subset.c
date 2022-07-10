#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "subset.h"

Bitstring InvalidTable[N+1][N+1]; // Lookup for perfect squares
Bitstring Knowns;                 // Values guaranteed to be in the Subset

static void set_bit(Bitstring bs, int bit) {
    bs[bit / CHUNK_SIZE] |= (1ull << (bit % CHUNK_SIZE));
}

static bool get_bit(Bitstring bs, int bit) {
    return 1 & (bs[bit / CHUNK_SIZE] >> (bit % CHUNK_SIZE));
}

static void or_equal_bitstrings(Bitstring lhs, Bitstring rhs) {
    for (int i = 0; i < NUM_CHUNKS; i++)
        lhs[i] |= rhs[i];
}

static void print_bitstring(Bitstring bs) {
    for (int i = 0; i <= N; i++)
        if (get_bit(bs, i)) printf("%d ", i);
    printf("\n\n");
}

static int popcount_bitstring(Bitstring bs) {
    int count = __builtin_popcountll(bs[0]);
    for (int i = 1; i < NUM_CHUNKS; i++)
        count += __builtin_popcountll(bs[i]);
    return count;
}


static bool forms_square(int A, int B, int C) {
    const int X = A * B * C;
    const int R = (int) sqrt(X);
    return R * R == X;
}

static void build_invalid_table() {
    for (int A = 1; A <= N; A++)
        for (int B = 1; B <= N; B++)
            for (int C = 1; C <= N; C++)
                if (forms_square(A, B, C))
                    set_bit(InvalidTable[A][B], C);
}


static bool is_prime(int x) {

    if (x == 1) return false;
    if (x == 2) return true;

    for (int i = 2; i < x; i++)
        if (x % i == 0) return false;

    return true;
}

static void build_known_bitstring() {
    for (int i = 1 + N / 2; i <= N; i++)
        if (is_prime(i)) set_bit(Knowns, i);
}


static void new_subset(Subset *set, int X) {
    memcpy(set->invalid, Knowns, sizeof(Bitstring));
    set->length = 1;
    set->values[0] = X;
}

static void extend_subset(Subset *set, const Subset *base, int X) {

    memcpy(set->invalid, base->invalid, sizeof(Bitstring));
    memcpy(set->values, base->values, sizeof(int) * base->length);

    set->length = base->length + 1;
    set->values[base->length] = X;

    for (int i = 0; i < base->length; i++)
        or_equal_bitstrings(set->invalid, InvalidTable[set->values[i]][X]);
}

static int popcount_excluded(const Subset *set) {

    const int start = 1 + set->values[set->length-1];

    int count = __builtin_popcountll(set->invalid[start / CHUNK_SIZE] >> (start % CHUNK_SIZE));
    for (int i = (start / CHUNK_SIZE) + 1; i < NUM_CHUNKS; i++)
        count += __builtin_popcountll(set->invalid[i]);

    return count;
}


static void swap_elements(int *A, int *B) {
    *A ^= *B;
    *B ^= *A;
    *A ^= *B;
}

static void sort_array(int *array, int length) {

    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
            if (array[j] < array[i])
                swap_elements(array + i, array + j);
}

static void print_solution(const Subset *solution) {

    const int length = solution->length + popcount_bitstring(Knowns);

    int answer[N], idx = 0;

    for (int i = 1; i <= N; i++)
        if (get_bit(Knowns, i))
            answer[idx++] = i;

    memcpy(answer + idx, solution->values, sizeof(int) * solution->length);
    sort_array(answer, length);

    printf("\n |S| = %d; S = {", length);
    for (int i = 0; i < length; i++)
        printf(" %d", answer[i]);
    printf(" }\n");
}


int main(int argc, char **argv) {

    build_invalid_table();
    build_known_bitstring();

    printf("Known Values: ");
    print_bitstring(Knowns);

    const int known_length = popcount_bitstring(Knowns);

    int si = 0;
    bool has_solution = false;
    Subset *stack = malloc(sizeof(Subset) * STACK_SIZE);

    for (int target = N - known_length; !has_solution; target--) {

        printf("\rSearching for |S| = %3d... ", target + known_length);
        fflush(stdout);

        for (int i = N; i > 0; i--)
            if (!get_bit(Knowns, i))
                new_subset(&(stack[si++]), i);

        while (si) {

            Subset top = stack[--si];

            if (top.length == target) {
                print_solution(&top);
                has_solution = true;
                continue;
            }

            int best_case = top.length + N
                          - top.values[top.length-1]
                          - popcount_excluded(&top);
            if (best_case < target) continue;

            for (int i = N; i > top.values[top.length-1]; i--)
                if (!get_bit(top.invalid, i))
                    extend_subset(&(stack[si++]), &top, i);
        }

        printf("\n");
    }
}

/********************************** Observations /**********************************

Let F(N) = |S|, where S = the greatest subset s.t. criteria are met.
Let X(N) denote the set of all subsets s.t. F(N) = |S|

1. Observe that 0 <= F(N+1) - F(N) <= 1

2. Observe that all primes > N/2 are included in all X(N)

3. Observe that for N>=4, No x in X(n) may have just one square

4. It seems most common for non-trivial N, that there are 0 squares.
   3 or more squares is illegal as it breaks the basic criteria. With
   only one square, one could add another IF IT EXISTS (N>=4) without
   creating any invalid triplets. There, all sets have 0 or 2 squares

*********************************** Observations /**********************************/