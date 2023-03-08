#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct linkedList{
    char letter;
    bool eof;
    struct linkedList *next;
    struct linkedList *prev;
} linkedList;

void appendNode(linkedList *current, char input, bool eof);

linkedList *navigateNodes(linkedList *current, int input);

//linkedList *navigateCNodes(linkedList *current, char input);

linkedList *removeNode(linkedList *current);

void addNode(linkedList *current, char input);

//linkedList *nextNode(linkedList *current);

//linkedList *prevNode(linkedList *current);

int getLineLength(linkedList *current);

linkedList *getEOL(linkedList *current);

linkedList *getPrevEOL(linkedList *current);

void printAll(linkedList *current);

const char *stringFrom(linkedList *current, int length);

#endif