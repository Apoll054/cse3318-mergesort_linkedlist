#define _POSIX_C_SOURCE 199309L   /* makes clock_gettime available under -std=c11 */
/* audit.c -- Part 2 harness.
 *
 *   ./audit <candidate 1-5> <n> <max_key> [seed]      (5 = your fixed_sort)
 *
 * It builds a random list of n nodes with keys in [0, max_key], runs the chosen
 * candidate, reports the CPU time, and runs the checks below.
 *
 * As provided, the ONLY check is "is the output sorted?". Part 2, Task 2: add
 * stronger checks in student_checks(). Do not edit anything else in this file. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

static double cpu_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

/* TODO (Part 2, Task 2): add your checks here. You have list_length(),
 * list_has_all(), list_is_stable(), list_is_sorted(), list_print().
 * Print one clear line per check (e.g. "stable: YES"). */
static void student_checks(Node *result, int n, int max_key) {
    int length = list_length(result);//store call values to prevent extra time complexity

    int has_all = list_has_all(result, n);

    int stable = list_is_stable(result);

    if (length==n){//used conditionals as per function designs
        printf("Result length: %d (expected: %d) \n YES the result length is the same\n", length, n);
    }
    else
    {
        printf("Result length: %d (expected: %d) \n NO the result length is not the same\n", length, n);
    }

    if (has_all==1){
        printf("YES! all nodes present exactly once\n");
    }
    else printf("NO! all nodes not present exactly once\n");

    if (stable==1){
        printf("YES! MergeSort implementation is Stable\n");
    }
    else printf("NO! MergeSort implementation not Stable\n");

    (void)max_key;//unused
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <candidate 1-5> <n> <max_key> [seed]\n", argv[0]);
        return 2;
    }
    int which   = atoi(argv[1]);
    int n       = atoi(argv[2]);
    int max_key = atoi(argv[3]);
    unsigned seed = (argc > 4) ? (unsigned)atoi(argv[4]) : 1u;

    Node *list = list_random(n, max_key, seed);

    double t0 = cpu_seconds();
    switch (which) {
        case 1:
            list = ai_sort_1(list);
            break;
        case 2:
            list = ai_sort_2(list);
            break;
        case 3:
            list = ai_sort_3(list);
            break;
        case 4:
            list = ai_sort_4(list);
            break;
        case 5:
            list = fixed_sort(list);
            break;
        default:
            fprintf(stderr, "candidate must be 1-5\n");
            return 2;
    }
    double t = cpu_seconds() - t0;

    printf("candidate %d  n=%d  max_key=%d  seed=%u\n", which, n, max_key, seed);
    printf("CPU time: %.4f s\n", t);
    printf("sorted: %s\n", list_is_sorted(list) ? "YES" : "NO");
    student_checks(list, n, max_key);

    list_free(list);
    return 0;
}
