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

enum orientationResult {
    COLLINEAR = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2
};

/* Finds the orientation between the three given points - calculates the angle between
    the Middle-First vector and Middle-Final vector - if the Middle-First vector is zero-length,
    the gradient of the Middle-Final vector is instead used. */
enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal);

enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal){
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

    if(crossProduct == 0){
        if(idxFirst == idxMiddle){
            /* Special case where we are only looking for positive slope of P1P2. */
            if(p2x == p1x){
                /* Special case: dx = 0, vertical */
                if(p2y < p1y){
                    /* Directly upwards */
                    return COUNTERCLOCKWISE;
                } else if(p2y == p1y){
                    /* Same point. */
                    return COLLINEAR;
                } else {
                    return CLOCKWISE;
                }
            }
            long double m = (p2y - p1y) / (p2x - p1x);
            if(m >= 0){
                return COUNTERCLOCKWISE;
            } else {
                return CLOCKWISE;
            }

        }
        return COLLINEAR;
    } else if(crossProduct > 0){
        return CLOCKWISE;
    } else {
        return COUNTERCLOCKWISE;
    }
}

struct solution *jarvisMarch(struct problem *p){
    /* Part A - perform Jarvis' March to construct a convex
    hull for the given problem. */
    /* IMPLEMENT HERE */
    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    /* ... */

    s->convexHull = hull;
    return s;
}

struct solution *grahamScan(struct problem *p){
    /* Part B - perform Graham's Scan to construct a convex
    hull for the given problem. */
    /* IMPLEMENT HERE */
    struct linkedList *hull = NULL;
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->operationCount = 0;

    /* ... */

    s->convexHull = hull;
    return s;
}

void freeSolution(struct solution *s){
    if(! s){
        return;
    }
    if(s->convexHull){
        freeList(s->convexHull);
    }
    free(s);
}
