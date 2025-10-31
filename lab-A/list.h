#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* data;
    struct node* link;
} node;

node* xor_operation(node* a, node* b) {
    return (node*)((unsigned long long)a ^ (unsigned long long)b);
}

node* creat_node(const char* data) {
    if (data == NULL) return NULL;
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL) return NULL;
    n->data = _strdup(data);
    n->link = NULL;
    return n;
}

node* add_node(node* current, const char* data) {
    if (data == NULL) return NULL;
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL) return NULL;
    current->link = xor_operation(n, current->link);
    n->data = _strdup(data);
    n->link = current;
    return n;
}

node* injection(node* prev, node* next, const char* data) {
    if ((prev == NULL) && (next == NULL)) return NULL;
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL) return NULL;
    n->data = _strdup(data);
    if (prev != NULL) prev->link = xor_operation(xor_operation(prev->link, next), n);
    n->link = xor_operation(prev, next);
    if (next != NULL) next->link = xor_operation(xor_operation(next->link, prev), n);
    return n;
}

node* found_node(node* head, const char* data) {
    if (head == NULL) return NULL;
    if (strcmp(head->data, data) == 0) return(head);
    node* prev = head;
    node* curr = head->link;
    node* save;
    while (curr != NULL) {
        if (strcmp(curr->data, data) == 0) return curr;
        save = curr;
        curr = xor_operation(curr->link, prev);
        prev = save;
    }
    return NULL;
}

node* remove_node(node* head, node* target) {
    if ((head == NULL) || (target == NULL)) return NULL;
    if (head == target) {
        node* new_head = head->link;
        if (new_head != NULL) new_head->link = xor_operation(new_head->link, head);
        free(head->data);
        free(head);
        return new_head;
    }
    node* prev = head;
    node* curr = head->link;
    node* next = xor_operation(curr->link, prev);
    node* save;
    while (curr != NULL) {
        if (curr == target) {
            if (prev != NULL) prev->link = xor_operation(xor_operation(prev->link, curr), next);
            if (next != NULL) next->link = xor_operation(xor_operation(next->link, curr), prev);
            free(curr->data);
            free(curr);
            return head;
        }
        save = curr;
        curr = xor_operation(curr->link, prev);
        prev = save;
        if (curr != NULL) next = xor_operation(curr->link, prev);
    }
    return head;
}

node* remove_by_key(node* head, const char* data) {
    if (head == NULL) return NULL;
    node* target = found_node(head, data);
    if (target == NULL) return head;
    return remove_node(head, target);
}

void print_list(node* head) {
    if (head == NULL) {
        printf("Список пуст\n");
        return;
    }
    node* prev = NULL;
    node* curr = head;
    node* save;
    int count = 0;
    printf("Содержимое списка:\n");
    while (curr != NULL) {
        printf("  [%d] %s %p %p\n", count, curr->data, curr, curr->link);
        save = curr;
        curr = xor_operation(prev, curr->link);
        prev = save;
        count++;
    }
    printf("Всего элементов: %d\n", count);
}

node* next_node(node* prev, node* curr) {
    if (curr == NULL) return NULL;
    return xor_operation(prev, curr->link);
}

#endif