/* test_part1.c -- provided tests for Part 1. Do not modify.
 * Build and run:  make test        (then:  make valgrind)
 * Tests run in stages: fix split first, then merge, then the full sort.      */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static int passed = 0, failed = 0;

static void report(const char *name, int ok) {
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

/* ---------- split ---------- */
static int check_split(int n) {
    int *keys = malloc((size_t)(n > 0 ? n : 1) * sizeof(int));
    for (int i = 0; i < n; i++) keys[i] = i * 10;
    Node *head = list_from_array(keys, n);
    free(keys);

    Node *second = split(head);
    int first_len = list_length(head);
    int second_len = list_length(second);
    int ok = 1;

    if (n < 2) {
        ok = (second == NULL) && (first_len == n);
    } else {
        int expect_first = (n + 1) / 2;                 /* ceil(n/2)  */
        ok = (first_len == expect_first) && (second_len == n - expect_first);
        /* order preserved: first list has seq 0.., second continues */
        int expect_seq = 0;
        for (Node *p = head; p && ok; p = p->next, expect_seq++) if (p->seq != expect_seq) ok = 0;
        for (Node *p = second; p && ok; p = p->next, expect_seq++) if (p->seq != expect_seq) ok = 0;
    }
    list_free(head);
    list_free(second);
    return ok;
}

static void test_split(void) {
    int sizes[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11};
    for (unsigned i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
        char name[64];
        snprintf(name, sizeof name, "split: list of %d node(s)", sizes[i]);
        report(name, check_split(sizes[i]));
    }
}

/* ---------- merge ---------- */
static int check_merge(const int *ka, int na, const int *kb, int nb) {
    Node *a = list_from_array(ka, na);
    Node *b = list_from_array(kb, nb);
    for (Node *p = b; p; p = p->next) p->seq += 1000;   /* mark nodes that came from b */
    Node *m = merge(a, b);
    int ok = list_is_sorted(m) && list_length(m) == na + nb;
    /* stability: for equal keys, nodes from a (seq < 1000) come before nodes from b */
    for (Node *p = m; p && p->next && ok; p = p->next)
        if (p->key == p->next->key && p->seq >= 1000 && p->next->seq < 1000) ok = 0;
    /* each list's own internal order must be kept */
    for (Node *p = m; p && ok; p = p->next)
        for (Node *q = p->next; q && ok; q = q->next)
            if (p->key == q->key && ((p->seq >= 1000) == (q->seq >= 1000)) && p->seq > q->seq) ok = 0;
    list_free(m);
    return ok;
}

static void test_merge(void) {
    int e[1] = {0};
    int a1[] = {1, 3, 5, 7}, b1[] = {2, 4, 6, 8};
    int a2[] = {1, 2, 3},    b2[] = {10, 20};
    int a3[] = {10, 20},     b3[] = {1, 2, 3};
    int a4[] = {5, 5, 5},    b4[] = {5, 5};
    int a5[] = {1, 4, 4, 9}, b5[] = {4, 4, 10};
    int a6[] = {7},          b6[] = {7};
    report("merge: both lists empty",          check_merge(e, 0, e, 0));
    report("merge: first list empty",          check_merge(e, 0, a2, 3));
    report("merge: second list empty",         check_merge(a2, 3, e, 0));
    report("merge: interleaved",               check_merge(a1, 4, b1, 4));
    report("merge: all of a before b",         check_merge(a2, 3, b2, 2));
    report("merge: all of b before a",         check_merge(a3, 2, b3, 3));
    report("merge: all keys equal (stability)",check_merge(a4, 3, b4, 2));
    report("merge: duplicates across lists",   check_merge(a5, 4, b5, 3));
    report("merge: one node each, equal keys", check_merge(a6, 1, b6, 1));
}

/* ---------- full sort ---------- */
static int check_sort(Node *head, int n) {
    Node *s = mergesort_list(head);
    int ok = list_is_sorted(s) && list_has_all(s, n) && list_is_stable(s);
    list_free(s);
    return ok;
}

static void test_sort(void) {
    char name[96];
    for (int n = 0; n <= 12; n++) {
        snprintf(name, sizeof name, "sort: random list, n = %d", n);
        report(name, check_sort(list_random(n, 20, 100u + (unsigned)n), n));
    }
    report("sort: 1,000 nodes, keys 0..1000",       check_sort(list_random(1000, 1000, 1u), 1000));
    report("sort: 1,000 nodes, only keys 0..3 (many duplicates, stability)",
           check_sort(list_random(1000, 3, 2u), 1000));
    report("sort: 1,000 nodes, all keys equal",     check_sort(list_random(1000, 0, 3u), 1000));

    int n = 2000;
    int *keys = malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) keys[i] = i;
    report("sort: already sorted (2,000 nodes)", check_sort(list_from_array(keys, n), n));
    for (int i = 0; i < n; i++) keys[i] = n - i;
    report("sort: reverse sorted (2,000 nodes)", check_sort(list_from_array(keys, n), n));
    free(keys);

    report("sort: 200,000 random nodes",            check_sort(list_random(200000, 1000000, 4u), 200000));
}

int main(void) {
    printf("=== split ===\n");
    test_split();
    printf("=== merge ===\n");
    test_merge();
    printf("=== full sort ===\n");
    test_sort();
    printf("\n%d passed, %d failed\n", passed, failed);
    return failed ? 1 : 0;
}
