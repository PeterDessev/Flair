#ifndef OPENEDFILE_H_
#define OPENEDFILE_H_

#include <stdio.h>
#include "LinkedList.h"

#define PATH_MAX 255

typedef struct openedFiles{
    int line;
    char name[FILENAME_MAX];
    char *extension;
    char fileDir[PATH_MAX + 1];  
    char fullPath[PATH_MAX + 1];
    linkedList *content;
} openedFile;

#endif