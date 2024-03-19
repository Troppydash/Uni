/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"
#include "list.h"

#define INITIALEDGES 32

struct edge;

/* Definition of a graph. */
struct graph {
    int numVertices;
    int numEdges;
    int allocedEdges;
    struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
    int start;
    int end;
    int cost;
};

struct graph *newGraph(int numVertices) {
    struct graph *g = (struct graph *) malloc(sizeof(struct graph));
    assert(g);
    /* Initialise edges. */
    g->numVertices = numVertices;
    g->numEdges = 0;
    g->allocedEdges = 0;
    g->edgeList = NULL;
    return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost) {
    assert(g);
    struct edge *newEdge = NULL;
    /* Check we have enough space for the new edge. */
    if ((g->numEdges + 1) > g->allocedEdges) {
        if (g->allocedEdges == 0) {
            g->allocedEdges = INITIALEDGES;
        } else {
            (g->allocedEdges) *= 2;
        }
        g->edgeList = (struct edge **) realloc(g->edgeList,
                                               sizeof(struct edge *) * g->allocedEdges);
        assert(g->edgeList);
    }

    /* Create the edge */
    newEdge = (struct edge *) malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = start;
    newEdge->end = end;
    newEdge->cost = cost;

    /* Add the edge to the list of edges. */
    g->edgeList[g->numEdges] = newEdge;
    (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be
  freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g) {
    struct graph *copyGraph = (struct graph *) malloc(sizeof(struct graph));
    assert(copyGraph);
    copyGraph->numVertices = g->numVertices;
    copyGraph->numEdges = g->numEdges;
    copyGraph->allocedEdges = g->allocedEdges;
    copyGraph->edgeList = (struct edge **) malloc(sizeof(struct edge *) * g->allocedEdges);
    assert(copyGraph->edgeList || copyGraph->numEdges == 0);
    int i;
    /* Copy edge list. */
    for (i = 0; i < g->numEdges; i++) {
        struct edge *newEdge = (struct edge *) malloc(sizeof(struct edge));
        assert(newEdge);
        newEdge->start = (g->edgeList)[i]->start;
        newEdge->end = (g->edgeList)[i]->end;
        newEdge->cost = (g->edgeList)[i]->cost;
        (copyGraph->edgeList)[i] = newEdge;
    }
    return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g) {
    int i;
    for (i = 0; i < g->numEdges; i++) {
        free((g->edgeList)[i]);
    }
    if (g->edgeList) {
        free(g->edgeList);
    }
    free(g);
}

struct pair {
    int first;
    int second;
};

// compute the shortest path in a graph from two points, returns the minimum distance
static int bfs(struct list **adjList, int vertices, int startingLocation, int finalLocation) {
    // allocate visited array
    bool *visited = malloc(sizeof(bool) * vertices);
    assert(visited);
    for (int i = 0; i < vertices; ++i) {
        visited[i] = false;
    }

    // a queue, maximize size is vertices for each vertex can be added to the queue exactly once
    // queue elements are of the form (vertex, depth)
    struct pair *queue = malloc(sizeof(struct pair) * vertices);
    assert(queue);
    int start = 0;
    int next = 0;

    // add starting vertex
    struct pair init = {startingLocation, 0};
    visited[next] = true;
    queue[next++] = init;

    int result = -1;
    // while the queue is not empty
    while (start < next) {
        // get front with sanity check
        assert(start < vertices);
        struct pair front = queue[start++];
        int node = front.first;
        int depth = front.second;

        // check if reached destination
        if (node == finalLocation) {
            result = depth;
            break;
        }

        // iterate through its neighbours
        struct list *head = adjList[node];
        while (head != NULL) {
            struct pair *neighbour = (struct pair *) peekHead(head);
            int other = neighbour->first;
            head = nextHead(head);

            // ignore if visited
            if (visited[other]) {
                continue;
            }
            // pre-visit the node and append to queue
            visited[other] = true;

            // sanity check to make sure no buffer overflow
            assert(next < vertices);
            struct pair data = {other, depth + 1};
            queue[next++] = data;

        }
    }

    free(queue);
    free(visited);

    // returns -1 if not found
    return result;
}

// computes the shortest weighted paths between two points, returns the minimum weighted path
static int dijkstras(struct list **adjList, int vertices, int startingLocation, int finalLocation) {
    // allocate visited array
    bool *visited = malloc(sizeof(bool) * vertices);
    assert(visited);
    for (int i = 0; i < vertices; ++i) {
        visited[i] = false;
    }

    struct pq *queue = newPQ();

    // add starting vertex
    struct pair *init = malloc(sizeof(struct pair));
    assert(init);
    init->first = startingLocation;
    init->second = 0;
    enqueue(queue, init, 0);

    int result = -1;
    while (!empty(queue)) {
        // get shortest
        struct pair *front = deletemin(queue);
        int node = front->first;
        int depth = front->second;
        free(front);

        // check if visited
        if (visited[node]) {
            continue;
        }
        visited[node] = true;

        // check if reached destination
        if (node == finalLocation) {
            result = depth;
            break;
        }

        // iterate through its neighbours
        struct list *head = adjList[node];
        while (head != NULL) {
            struct pair *neighbour = (struct pair *) peekHead(head);
            int other = neighbour->first;
            int cost = neighbour->second;
            head = nextHead(head);

            // ignore if visited
            if (visited[other]) {
                continue;
            }

            // add to queue
            struct pair *next = malloc(sizeof(struct pair));
            assert(next);
            next->first = other;
            next->second = depth + cost;
            enqueue(queue, next, depth + cost);
        }
    }

    // free queue
    while (!empty(queue)) {
        struct pair *front = deletemin(queue);
        free(front);
    }

    freePQ(queue);
    free(visited);

    // returns -1 if not found
    return result;
}

// computes the minimum spanning tree, returns the minimum tree weight
static int primsMethod(struct list **adjList, int vertices, int startingLocation) {
    // allocate visited array
    bool *visited = malloc(sizeof(bool) * vertices);
    assert(visited);
    for (int i = 0; i < vertices; ++i) {
        visited[i] = false;
    }

    struct pq *queue = newPQ();

    // add starting vertex
    struct pair *init = malloc(sizeof(struct pair));
    assert(init);
    init->first = startingLocation;
    init->second = 0;
    enqueue(queue, init, 0);

    int result = 0;
    while (!empty(queue)) {
        // get shortest
        struct pair *front = deletemin(queue);
        int node = front->first;
        int weight = front->second;
        free(front);

        // check if visited
        if (visited[node]) {
            continue;
        }
        visited[node] = true;

        // add this edge weight to total
        result += weight;

        // iterate through its neighbours
        struct list *head = adjList[node];
        while (head != NULL) {
            struct pair *neighbour = (struct pair *) peekHead(head);
            int other = neighbour->first;
            int cost = neighbour->second;
            head = nextHead(head);

            // ignore if visited
            if (visited[other]) {
                continue;
            }

            // add to queue
            struct pair *next = malloc(sizeof(struct pair));
            assert(next);
            next->first = other;
            next->second = cost;
            enqueue(queue, next, cost);
        }
    }

    freePQ(queue);
    free(visited);

    // returns -1 if not found
    return result;
}

// computes the shortest percent paths between two points, returns the minimum percent path
static int dijkstrasPercent(struct list **adjList, int vertices, int startingLocation, int finalLocation) {
    // allocate visited array
    bool *visited = malloc(sizeof(bool) * vertices);
    assert(visited);
    for (int i = 0; i < vertices; ++i) {
        visited[i] = false;
    }

    // create queue and add starting vertex
    struct heapq *queue = newHeapq();
    heapqPush(queue, startingLocation, 1.0);

    int result = -1;
    while (!heapqEmpty(queue)) {
        // get smallest
        int node;
        long double multiplier;
        heapqTop(queue, &node, &multiplier);
        heapqPop(queue);

        // check if visited
        if (visited[node]) {
            continue;
        }
        visited[node] = true;

        // check if reached destination
        if (node == finalLocation) {
            // bring multiplier to %, floor it, then subtract the base 100
            result = (int) floorl(multiplier * 100.0) - 100;
            break;
        }

        // iterate through its neighbours
        struct list *head = adjList[node];
        while (head != NULL) {
            struct pair *neighbour = (struct pair *) peekHead(head);
            int other = neighbour->first;
            int cost = neighbour->second;
            head = nextHead(head);

            // ignore if visited
            if (visited[other]) {
                continue;
            }

            // the percent cost adds the base 100, converts to double, then divide by 100 into decimal
            long double percentCost = (long double) (cost + 100) / 100.0;
            // add to queue
            heapqPush(queue, other, multiplier * percentCost);
        }
    }

    freeHeapq(queue);
    free(visited);

    // returns -1 if not found
    return result;
}


struct solution *graphSolve(struct graph *g, enum problemPart part,
                            int numLocations, int startingLocation, int finalLocation) {
    struct solution *solution = (struct solution *)
            malloc(sizeof(struct solution));
    assert(solution);

    int vertices = g->numVertices;
    int edges = g->numEdges;

    // create adjacency list
    struct list **adjList = malloc(vertices * sizeof(struct list *));
    assert(adjList);
    for (int i = 0; i < vertices; ++i) {
        adjList[i] = NULL;
    }

    // add all edges into the list, undirected
    for (int i = 0; i < edges; ++i) {
        struct edge *e = g->edgeList[i];

        struct pair *forward = malloc(sizeof(struct pair));
        assert(forward);
        forward->first = e->end;
        forward->second = e->cost;

        struct pair *reverse = malloc(sizeof(struct pair));
        assert(reverse);
        reverse->first = e->start;
        reverse->second = e->cost;

        adjList[e->start] = prependList(adjList[e->start], forward);
        adjList[e->end] = prependList(adjList[e->end], reverse);
    }

    if (part == PART_A) {
        solution->damageTaken = bfs(adjList, vertices, startingLocation, finalLocation);
    } else if (part == PART_B) {
        solution->totalCost = dijkstras(adjList, vertices, startingLocation, finalLocation);
    } else if (part == PART_C) {
        solution->artisanCost = primsMethod(adjList, vertices, startingLocation);
    } else {
        solution->totalPercentage = dijkstrasPercent(adjList, vertices, startingLocation, finalLocation);
    }

    // free adjacency list
    for (int i = 0; i < vertices; ++i) {
        freeList(adjList[i]);
    }
    free(adjList);

    return solution;
}
