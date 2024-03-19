/*
    Skeleton written by Grady Fitzpatrick for
    COMP20007 Assignment 1 2024 Semester 1

    Implementation details for module which contains doubly-linked list
        specification data structures and functions.

    Implemented by <YOU>
*/

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct linkedListNode;

struct linkedList {
    struct linkedListNode *head;
    struct linkedListNode *tail;
};

struct linkedListNode {
    struct linkedListNode *next;
    struct linkedListNode *prev;

    long double x;
    long double y;
};

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingForward(struct linkedList *list) {
    struct linkedListNode *node = list->head;
    while (node != NULL) {
        printf("(%Lf, %Lf) ", node->x, node->y);
        node = node->next;
    }
}

/* Print out first the first item of the list, then print out each value in
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingBackwards(struct linkedList *list) {
    struct linkedListNode *node = list->head;
    if (node == NULL) {
        return;
    }

    do {
        printf("(%Lf, %Lf) ", node->x, node->y);

        // previous node with looping from the head to tail
        node = node->prev;
        if (node == NULL) {
            node = list->tail;
        }
    } while (node != list->head);
}

/* Return a new empty list. */
struct linkedList *newList() {
    struct linkedList *list = malloc(sizeof(struct linkedList));
    assert(list);
    list->head = list->tail = NULL;

    return list;
}

/* Insert the given x, y pair at the head of the list */
void insertHead(struct linkedList *list, long double x, long double y) {
    // allocate node
    struct linkedListNode *node = malloc(sizeof(struct linkedListNode));
    assert(node);
    node->next = node->prev = NULL;
    node->x = x;
    node->y = y;

    if (list->head == NULL) {
        // when the list is empty
        list->head = list->tail = node;
    } else {
        // when the list has one or more elements
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
}

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, long double x, long double y) {
    struct linkedListNode *node = malloc(sizeof(struct linkedListNode));
    assert(node);
    node->next = node->prev = NULL;
    node->x = x;
    node->y = y;

    if (list->tail == NULL) {
        // when the list is empty
        list->head = list->tail = node;
    } else {
        // when the list has one or more elements
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
}

/* Free all items in the given list. */
void freeList(struct linkedList *list) {
    // first free the nodes
    struct linkedListNode *node = list->head;
    while (node != NULL) {
        struct linkedListNode *next = node->next;
        free(node);
        node = next;
    }

    // then free the list
    free(list);
}