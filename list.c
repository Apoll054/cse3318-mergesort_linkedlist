#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static Node *new_node(int key, int seq) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    n->key = key;
    n->seq = seq;
    n->next = NULL;
    return n;
}

Node *list_from_array(const int *keys, int n) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *node = new_node(keys[i], i);
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

Node *list_random(int n, int max_key, unsigned seed) {
    srand(seed);
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *node = new_node(rand() % (max_key + 1), i);
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void list_free(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int list_length(const Node *head) {
    int len = 0;
    for (; head != NULL; head = head->next) len++;
    return len;
}

void list_print(const Node *head) {
    for (; head != NULL; head = head->next) printf("%d(%d)%s", head->key, head->seq, head->next ? " -> " : "");
    printf("\n");
}

int list_is_sorted(const Node *head) {
    for (; head != NULL && head->next != NULL; head = head->next)
        if (head->key > head->next->key) return 0;
    return 1;
}

int list_is_stable(const Node *head) {
    for (; head != NULL && head->next != NULL; head = head->next)
        if (head->key == head->next->key && head->seq > head->next->seq) return 0;
    return 1;
}

int list_has_all(const Node *head, int n) {
    if (n == 0) return head == NULL;
    char *seen = calloc((size_t)n, 1);
    if (seen == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    int count = 0, ok = 1;
    for (; head != NULL; head = head->next) {
        if (head->seq < 0 || head->seq >= n || seen[head->seq]) {
            ok = 0;
            break;
        }
        seen[head->seq] = 1;
        count++;
        if (count > n) {
            ok = 0;
            break;
        }
    }
    free(seen);
    return ok && count == n;
}
