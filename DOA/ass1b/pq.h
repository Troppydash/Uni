/*
pq.h

Visible structs and functions for priority queues.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/

#include <stdbool.h>

/* The priority queue. */
struct pq;

/* Get a new empty priority queue. */
struct pq *newPQ();

/* Add an item to the priority queue - cast pointer to (void *). */
void enqueue(struct pq *pq, void *item, int priority);

/* Take the smallest item from the priority queue - cast pointer back to
  original type. */
void *deletemin(struct pq *pq);

/* Returns 1 if empty, 0 otherwise. */
int empty(struct pq *pq);

/* Remove all items from priority queue (doesn't free) and free the queue. */
void freePQ(struct pq *pq);

/* a binary heap queue */
struct heapq;

/* create a new heap */
struct heapq *newHeapq();

/* push a node with priority onto the queue */
void heapqPush(struct heapq *heap, int node, long double priority);

/* pop the min node off the queue */
void heapqPop(struct heapq *heap);

/* returns the minimum node on the queue */
void heapqTop(struct heapq *heap, int *node, long double *priority);

/* returns whether the queue is empty */
bool heapqEmpty(struct heapq *heap);

/* frees the queue */
void freeHeapq(struct heapq *heap);