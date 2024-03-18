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
    /* IMPLEMENT HERE */


    long double x;
    long double y;
};

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingForward(struct linkedList *list){
    /* IMPLEMENT HERE */


}

/* Print out first the first item of the list, then print out each value in
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting
    the item. */
void traverseRingBackwards(struct linkedList *list){
    /* IMPLEMENT HERE */


}

/* Return a new empty list. */
struct linkedList *newList(){
    /* IMPLEMENT HERE */
    struct linkedList *list = NULL;


    return list;
}

/* Insert the given x, y pair at the head of the list */
void insertHead(struct linkedList *list, long double x, long double y){
    /* IMPLEMENT HERE */

}

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, long double x, long double y){
    /* IMPLEMENT HERE */

}

/* Free all items in the given list. */
void freeList(struct linkedList *list){
    /* IMPLEMENT HERE */

}