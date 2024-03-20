/*
pq.c

Unsorted Array Implementation

Implementations for helper functions for priority queue construction and
manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdlib.h>
#include <assert.h>
#include "pq.h"


#define INITIALITEMS 32

struct pq {
    int count;
    int allocated;
    void **queue;
    int *priorities;
};


struct pq *newPQ() {
    struct pq *pq = (struct pq *) malloc(sizeof(struct pq));
    assert(pq);
    pq->count = 0;
    pq->allocated = 0;
    pq->queue = NULL;
    pq->priorities = NULL;
    return pq;
}

void enqueue(struct pq *pq, void *item, int priority) {
    assert(pq);
    if ((pq->count + 1) > pq->allocated) {
        if (pq->allocated == 0) {
            pq->allocated = INITIALITEMS;
        } else {
            pq->allocated *= 2;
        }
        pq->queue = (void **) realloc(pq->queue,
                                      pq->allocated * sizeof(void *));
        assert(pq->queue);
        pq->priorities = (int *) realloc(pq->priorities,
                                         pq->allocated * sizeof(int));
        assert(pq->priorities);
    }
    (pq->queue)[pq->count] = item;
    (pq->priorities)[pq->count] = priority;
    (pq->count)++;
}

/* Scan through all the priorities linearly and find lowest. */
void *deletemin(struct pq *pq) {
    int i;
    int lowestElement = 0;
    void *returnVal;
    if (pq->count <= 0) {
        return NULL;
    }
    for (i = 0; i < pq->count; i++) {
        if ((pq->priorities)[i] < (pq->priorities)[lowestElement]) {
            lowestElement = i;
        }
    }
    returnVal = (pq->queue)[lowestElement];
    /* Delete item from queue by swapping final item into place of deleted
      element. */
    if (pq->count > 0) {
        (pq->priorities)[lowestElement] = (pq->priorities)[pq->count - 1];
        (pq->queue)[lowestElement] = (pq->queue)[pq->count - 1];
        (pq->count)--;
    }
    return returnVal;
}

int empty(struct pq *pq) {
    return pq->count == 0;
}

void freePQ(struct pq *pq) {
    if (!pq) {
        return;
    }
    if (pq->allocated > 0) {
        free(pq->queue);
        free(pq->priorities);
    }
    free(pq);
}


struct pairDouble {
    int node;
    long double priority;
};

struct heapq {
    struct pairDouble *pairs;
    int size;
    int capacity;
};

/* allocate new heap */
struct heapq *newHeapq() {
    struct heapq *heap = malloc(sizeof(struct heapq));
    assert(heap);

    heap->capacity = INITIALITEMS;
    heap->size = 0;
    heap->pairs = malloc(sizeof(struct pairDouble) * INITIALITEMS);
    assert(heap->pairs);

    return heap;
}

/* frees the entire heap */
void freeHeapq(struct heapq *heap) {
    free(heap->pairs);
    free(heap);
}

/* returns the top of the heap, minimum priority */
void heapqTop(struct heapq *heap, int *node, long double *priority) {
    assert(heap->size > 0);
    *node = heap->pairs[0].node;
    *priority = heap->pairs[0].priority;
}

/* returns whether the heap is empty */
bool heapqEmpty(struct heapq *heap) {
    return heap->size == 0;
}

/* push an element into the heap */
void heapqPush(struct heapq *heap, int node, long double priority) {
    // resize if out of storage
    if (heap->capacity == heap->size) {
        heap->capacity *= 2;
        heap->pairs = realloc(
                heap->pairs,
                sizeof(struct pairDouble) * heap->capacity
        );
        assert(heap->pairs);
    }

    // bring the element up from the last position
    int i = heap->size;
    while (i != 0) {
        // check if smaller than parent
        int parent = (i - 1) / 2;
        if (heap->pairs[parent].priority <= priority) {
            break;
        }

        // else swap
        heap->pairs[i] = heap->pairs[parent];
        i = parent;
    }

    // write to the last position
    heap->pairs[i].node = node;
    heap->pairs[i].priority = priority;
    heap->size += 1;
}

/* pop the minimum element off the heap */
void heapqPop(struct heapq *heap) {
    assert(heap->size > 0);

    // swap the last element with the first, then remove the last position
    struct pairDouble last = heap->pairs[heap->size - 1];
    heap->size -= 1;

    // bring the element down
    int i = 0;
    while (true) {
        int lchild = 2 * i + 1;
        int rchild = lchild + 1;

        if (lchild < heap->size && rchild < heap->size) {
            // if it has two children
            long double left = heap->pairs[lchild].priority;
            long double right = heap->pairs[rchild].priority;

            // check if larger than the smallest,
            // bring it down if so, stop otherwise
            if (left <= right && last.priority > left) {
                heap->pairs[i] = heap->pairs[lchild];
                i = lchild;
            } else if (right <= left && last.priority > right) {
                heap->pairs[i] = heap->pairs[rchild];
                i = rchild;
            } else {
                break;
            }
        } else if (lchild < heap->size) {
            // if it only has the left child
            long double left = heap->pairs[lchild].priority;

            if (last.priority > left) {
                heap->pairs[i] = heap->pairs[lchild];
                i = lchild;
            } else {
                break;
            }
        } else {
            // if no child, we stop
            break;
        }
    }

    // place the last node into the correct position
    heap->pairs[i] = last;
}





