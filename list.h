#ifndef LIST_H
#define LIST_H

/* Singly linked list node. 'seq' is the node's ORIGINAL position (0, 1, 2, ...)
 * assigned when the list is built. It never changes. We use it to check
 * (a) that no nodes were lost or duplicated, and (b) that your sort is stable. */
typedef struct Node {
    int key;
    int seq;
    struct Node *next;
} Node;

/* ---- Provided helpers (list.c). Do not modify. ---- */
Node *list_from_array(const int *keys, int n);          /* seq = 0..n-1            */
Node *list_random(int n, int max_key, unsigned seed);   /* keys in [0, max_key]    */
void  list_free(Node *head);
int   list_length(const Node *head);
void  list_print(const Node *head);                     /* prints key(seq) ...     */

/* ---- Checkers (list.c) ---- */
int list_is_sorted(const Node *head);   /* 1 if keys are non-decreasing            */
int list_is_stable(const Node *head);   /* 1 if equal keys appear in increasing seq */
int list_has_all(const Node *head, int n); /* 1 if seq values 0..n-1 each appear
                                              exactly once (no lost/duplicate nodes) */

/* ---- YOUR work, Part 1 (msort.c) ---- */
Node *split(Node *head);
Node *merge(Node *a, Node *b);
Node *mergesort_list(Node *head);

/* ---- Part 2: four candidate sorts (candidates.c) ---- */
Node *ai_sort_1(Node *head);
Node *ai_sort_2(Node *head);
Node *ai_sort_3(Node *head);
Node *ai_sort_4(Node *head);

/* ---- Part 2, Task 4: YOUR fixed version of one candidate (fixed.c) ---- */
Node *fixed_sort(Node *head);

#endif
