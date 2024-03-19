#include <stdlib.h>
#include <math.h>
#include "problem.h"
#include "convexHull.h"

#define UNIFORM_RADIUS (50.0)
#define PI (3.141592)

// returns a random real value between [0, UNIFORM_RADIUS]
static long double realRandom() {
    return (long double) rand() / (long double) RAND_MAX;
}

enum SampleType {
    SAMPLE_RANDOM = 0,
    SAMPLE_CIRCLE,
    SAMPLE_INNER,
};

// create a pair of equal sample points, depending on the generation type and length
static void makeSamples(enum SampleType type, int n, struct problem **A, struct problem **B) {
    struct problem *pA = malloc(sizeof(struct problem));
    struct problem *pB = malloc(sizeof(struct problem));

    pA->part = PART_A;
    pA->pointsX = malloc(sizeof(long double) * n);
    pA->pointsY = malloc(sizeof(long double) * n);
    pA->numPoints = n;
    pB->part = PART_B;
    pB->pointsX = malloc(sizeof(long double) * n);
    pB->pointsY = malloc(sizeof(long double) * n);
    pB->numPoints = n;

    switch (type) {
        case SAMPLE_RANDOM: {
            // random points within a square
            for (int i = 0; i < n; ++i) {
                long double x = (realRandom() * 2.0 * UNIFORM_RADIUS) - UNIFORM_RADIUS;
                long double y = (realRandom() * 2.0 * UNIFORM_RADIUS) - UNIFORM_RADIUS;
                pA->pointsX[i] = pB->pointsX[i] = x;
                pA->pointsY[i] = pB->pointsY[i] = y;
            }
            break;
        }
        case SAMPLE_CIRCLE: {
            // random points on a circle
            for (int i = 0; i < n; ++i) {
                long double angle = realRandom() * 2.0 * PI;
                long double x = UNIFORM_RADIUS * coshl(angle);
                long double y = UNIFORM_RADIUS * sinhl(angle);
                pA->pointsX[i] = pB->pointsX[i] = x;
                pA->pointsY[i] = pB->pointsY[i] = y;
            }
            break;
        }
        case SAMPLE_INNER: {
            // random points within a square, including the 4 corners

            // top left
            pA->pointsX[0] = pB->pointsX[0] = -UNIFORM_RADIUS;
            pA->pointsY[0] = pB->pointsY[0] = UNIFORM_RADIUS;
            // top right
            pA->pointsX[1] = pB->pointsX[1] = UNIFORM_RADIUS;
            pA->pointsY[1] = pB->pointsY[1] = UNIFORM_RADIUS;
            // bottom left
            pA->pointsX[2] = pB->pointsX[2] = -UNIFORM_RADIUS;
            pA->pointsY[2] = pB->pointsY[2] = -UNIFORM_RADIUS;
            // bottom right
            pA->pointsX[3] = pB->pointsX[3] = UNIFORM_RADIUS;
            pA->pointsY[3] = pB->pointsY[3] = -UNIFORM_RADIUS;

            for (int i = 4; i < n; ++i) {
                long double x = (realRandom() * 2.0 * UNIFORM_RADIUS) - UNIFORM_RADIUS;
                long double y = (realRandom() * 2.0 * UNIFORM_RADIUS) - UNIFORM_RADIUS;
                pA->pointsX[i] = pB->pointsX[i] = x;
                pA->pointsY[i] = pB->pointsY[i] = y;
            }
        }
    }

    *A = pA;
    *B = pB;
}

// test a particular sample strategy of size n
static void test(const char *tag, int n, enum SampleType type)
{
    struct problem *A;
    struct problem *B;
    makeSamples(type, n, &A, &B);

    struct solution *s1 = jarvisMarch(A);
    struct solution *s2 = grahamScan(B);
    printf("%s, n=%10d, jarvisMarch=%10llu, grahamScan=%10llu\n", tag, n, s1->operationCount, s2->operationCount);
    fflush(stdout);

    freeSolution(s1);
    freeSolution(s2);

    freeProblem(A);
    freeProblem(B);
}

int main() {
    test("RANDOM", 1000, SAMPLE_RANDOM);
    test("RANDOM", 10000, SAMPLE_RANDOM);
    test("RANDOM", 100000, SAMPLE_RANDOM);

    test("CIRCLE", 1000, SAMPLE_CIRCLE);
    test("CIRCLE", 10000, SAMPLE_CIRCLE);
    test("CIRCLE", 100000, SAMPLE_CIRCLE);

    test("INNER ", 1000, SAMPLE_INNER);
    test("INNER ", 10000, SAMPLE_INNER);
    test("INNER ", 100000, SAMPLE_INNER);

    return 0;
}
