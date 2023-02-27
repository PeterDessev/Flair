#ifndef CLANG_H_
#define CLANG_H_
#include "../linkedlist/LinkedList.h"
#include "../openedFile/openedFile.h"

typedef struct myScreen{
    int row;
    int col;
    bool enableColor;
    linkedList *current;
    int curLine;
    int i;
} myScreen;

int addChar(myScreen *Screen);

int isComment(myScreen *Screen);

int colorWord(char *input[], myScreen *Screen, int color[], int size);

void initCLang(openedFile File);

void updateCLang(openedFile File);

#endif