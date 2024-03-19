/*
    Skeleton written by Grady Fitzpatrick for
    COMP20007 Assignment 1 2024 Semester 1

    Header for module which contains convex hull
        specification data structures and functions.

    Implemented by <YOU>
*/
#include "linkedList.h"
#include "problem.h"
#include "convexHull.h"
#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <string.h>
#include <stdbool.h>

#define EPS (0.000001)

enum orientationResult {
    COLLINEAR = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2
};

/* Finds the orientation between the three given points - calculates the angle between
    the Middle-First vector and Middle-Final vector - if the Middle-First vector is zero-length,
    the gradient of the Middle-Final vector is instead used. */
enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal);

enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal) {
    assert(idxFirst >= 0 && idxFirst < p->numPoints);
    assert(idxMiddle >= 0 && idxMiddle < p->numPoints);
    assert(idxFinal >= 0 && idxFinal < p->numPoints);

    /* Use cross-product to calculate turn direction. */
    long double p0x = p->pointsX[idxFirst];
    long double p0y = p->pointsY[idxFirst];

    long double p1x = p->pointsX[idxMiddle];
    long double p1y = p->pointsY[idxMiddle];

    long double p2x = p->pointsX[idxFinal];
    long double p2y = p->pointsY[idxFinal];

    /* Cross product of vectors P1P0 & P1P2 */
    long double crossProduct = (p0x - p1x) * (p2y - p1y) - (p0y - p1y) * (p2x - p1x);

    if (crossProduct == 0) {
        if (idxFirst == idxMiddle) {
            /* Special case where we are only looking for positive slope of P1P2. */
            if (p2x == p1x) {
                /* Special case: dx = 0, vertical */
                if (p2y < p1y) {
                    /* Directly upwards */
                    return COUNTERCLOCKWISE;
                } else if (p2y == p1y) {
                    /* Same point. */
                    return COLLINEAR;
                } else {
                    return CLOCKWISE;
                }
            }
            long double m = (p2y - p1y) / (p2x - p1x);
            if (m >= 0) {
                return COUNTERCLOCKWISE;
            } else {
                return CLOCKWISE;
            }

        }
        return COLLINEAR;
    } else if (crossProduct > 0) {
        return CLOCKWISE;
    } else {
        return COUNTERCLOCKWISE;
    }
}

// returns -1 if a < b, 0 if approximately equals, and 1 if b > a
static int ldoubleCompare(long double a, long double b) {
    if (fabsl(a - b) < EPS) {
        return 0;
    }

    return (a < b) ? -1 : 1;
}

// sorts the indices based on angles and distances,
// returns the number of angle comparisons
static unsigned long long mergesort(
        const long double *angles, const long double *distances,
        int *indices, int n
) {
    // base case
    if (n <= 1) {
        return 0;
    }

    unsigned long long comparisons = 0;

    // recursion
    int mid = n / 2;
    comparisons += mergesort(angles, distances, indices, mid);
    comparisons += mergesort(angles, distances, indices + mid, n - mid);

    // merging steps ahead

    // we copy the left result into the array left
    int *left = malloc(sizeof(int) * mid);
    memcpy(left, indices, mid * sizeof(int));
    int *right = indices + mid;

    // merging left and right directly into indices
    int i = 0, j = 0, k = 0;
    while (i < mid && j < n - mid) {
        comparisons += 1;
        int angleCompare = ldoubleCompare(angles[left[i]], angles[right[j]]);
        int distCompare = ldoubleCompare(distances[left[i]], distances[right[j]]);
        if (angleCompare < 0 || (angleCompare == 0 && distCompare < 0)) {
            indices[k++] = left[i++];
        } else {
            indices[k++] = right[j++];
        }
    }

    // copy the unmerged elements in array left into indices
    memcpy(indices + k, left + i, (mid - i) * sizeof(int));
    free(left);

    return comparisons;
}


// returns the minimum point in x then y in the problem,
// errors if numPoints is zero
static int minimumPointXY(const struct problem *p) {
    long double x = LDBL_MAX;
    long double y = LDBL_MAX;
    int minimum = -1;

    // iteratively finds the smallest point
    for (int i = 0; i < p->numPoints; ++i) {
        if (ldoubleCompare(p->pointsX[i], x) < 0
            || (ldoubleCompare(p->pointsX[i], x) == 0
                    && p->pointsY[i] < y)) {
            minimum = i;
            x = p->pointsX[i];
            y = p->pointsY[i];
        }
    }

    assert(minimum != -1);
    return minimum;
}

// returns the minimum point in y then x in the problem,
// errors if numPoints is zero
static int minimumPointYX(const struct problem *p) {
    long double x = LDBL_MAX;
    long double y = LDBL_MAX;
    int minimum = -1;

    // iteratively finds the smallest point
    for (int i = 0; i < p->numPoints; ++i) {
        if (ldoubleCompare(p->pointsY[i], y) < 0
            || (ldoubleCompare(p->pointsY[i], y) == 0
                    && p->pointsX[i] < x)) {
            minimum = i;
            x = p->pointsX[i];
            y = p->pointsY[i];
        }
    }

    assert(minimum != -1);
    return minimum;
}


struct solution *jarvisMarch(struct problem *p) {
    /* Part A - perform Jarvis' March to construct a convex
    hull for the given problem. */
    struct linkedList *hull = newList();
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;
    s->convexHull = hull;

    // early return on impossible conditions
    if (p->numPoints < 3) {
        return s;
    }

    // search for bottom left point
    int leftMost = minimumPointXY(p);

    // loop and finding the next convex hull point til returning to start point
    int current = leftMost;
    do {
        insertTail(s->convexHull, p->pointsX[current], p->pointsY[current]);

        // randomly selected an element not current
        int next = 0;
        if (next == current) {
            next = 1;
        }

        for (int i = 0; i < p->numPoints; ++i) {
            // skip identical points
            if (i == current || i == next) {
                continue;
            }

            s->operationCount += 1;
            if (orientation(p, next, current, i) == COUNTERCLOCKWISE) {
                next = i;
            }
        }

        current = next;
    } while (current != leftMost);

    return s;
}

struct solution *grahamScan(struct problem *p) {
    /* Part B - perform Graham's Scan to construct a convex
    hull for the given problem. */
    /* IMPLEMENT HERE */
    struct linkedList *hull = newList();
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;
    s->convexHull = hull;

    // early return when impossible
    if (p->numPoints < 3) {
        return s;
    }

    int lowest = minimumPointYX(p);

    // create indices, angle, and distance arrays for sorting
    int n = p->numPoints;
    int *indices = malloc(sizeof(int) * n);
    long double *angles = malloc(sizeof(long double) * n);
    long double *distances = malloc(sizeof(long double) * n);
    assert(indices);
    assert(angles);
    assert(distances);

    for (int i = 0; i < n; ++i) {
        indices[i] = i;

        if (i == lowest) {
            // for the lowest point, we want it to be first in the sort
            // so assign zeros for both angle and distances
            angles[i] = 0.0;
            distances[i] = 0.0;
        } else {
            long double dy = p->pointsY[i] - p->pointsY[lowest];
            long double dx = p->pointsX[i] - p->pointsX[lowest];
            angles[i] = atan2l(dy, dx);
            // don't need to sqrt for monotonic property
            distances[i] = dy * dy + dx * dx;
        }
    }

    // merge sort on the indices
    s->operationCount = mergesort(angles, distances, indices, n);

    // create a stack, max size is the whole points array
    // this is a stack of point indices
    int *stack = malloc(sizeof(int) * n);
    assert(stack);
    int stackSize = 0;
    stack[stackSize++] = indices[0];
    stack[stackSize++] = indices[1];

    // looping through the sorted indices, i is the index of indices
    for (int i = 2; i < n; ++i) {
        // keep popping when we have more than 2 elements
        // and the angle for the last three forms a clockwise orientation
        while (stackSize > 2 &&
               orientation(
                       p,
                       stack[stackSize - 2], stack[stackSize - 1], indices[i]
               ) != COUNTERCLOCKWISE) {
            stackSize -= 1;
        }

        stack[stackSize++] = indices[i];
    }

    // copy the stack result to the linked list
    for (int i = 0; i < stackSize; ++i) {
        insertTail(s->convexHull, p->pointsX[stack[i]], p->pointsY[stack[i]]);
    }

    free(stack);
    free(indices);
    free(angles);
    free(distances);

    return s;
}

void freeSolution(struct solution *s) {
    if (!s) {
        return;
    }
    if (s->convexHull) {
        freeList(s->convexHull);
    }
    free(s);
}
