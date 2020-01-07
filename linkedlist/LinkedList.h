#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* ^^ these are the include guards */

/* Prototypes for the functions */
/* Sums two ints */
int Sum(int a, int b);

typedef struct linkedList{
    char letter;
    struct linkedList *next;
    struct linkedList *prev;
} linkedList;

void appendNode(linkedList *current, char input);

linkedList *navigateNodes(linkedList *current, int input);

linkedList *navigateCNodes(linkedList *current, char input);

linkedList *removeNode(linkedList *current);

void addNode(linkedList *current, char input);

linkedList *nextNode(linkedList *current);

#endif