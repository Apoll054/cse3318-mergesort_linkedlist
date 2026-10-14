/* fixed.c -- Part 2, Task 4.
 *
 * Pick ONE defective candidate from candidates.c. Copy its code here, give the
 * copied functions new names (candidates.c already uses names like merge1 and
 * sort1, and those are static there, so reusing them in this file is fine, but
 * choosing clear new names helps you), FIX the defect, and make fixed_sort()
 * call your corrected version.
 *
 * Then test it with:   ./audit 5 <n> <max_key>
 * It must pass every check you wrote in audit.c's student_checks().         */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* ------------------------------------------------------------------ */
/* Candidate 2     fixed                                                    */
/* ------------------------------------------------------------------ */
static Node *split_list(Node *head) {
    if (head == NULL || head->next == NULL) return NULL;
    Node *slow = head;
    Node *fast = head->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *second = slow->next;
    slow->next = NULL;
    return second;
}
static Node *merge2(Node *a, Node *b) {
    Node dummy;
    Node *tail = &dummy;
    dummy.next = NULL;
    while (a != NULL && b != NULL) {
        if (a->key <= b->key) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}

static Node *sort2(Node *head) {
    if (head == NULL || head->next == NULL) return head;
    Node *second = split_list(head);
    head = sort2(head);
    second = sort2(second);
    return merge2(head, second);
}


Node *fixed_sort(Node *head) {
    /* done */
    return sort2(head);
}
