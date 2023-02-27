#include "FileText.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void initFileTxt(openedFile File){
    int row, col;
    linkedList *current;
    int curLine = 1;      
    initscr();
    werase(stdscr);
    getmaxyx(stdscr,row,col);
    

    
    //Text Editor header
    for(int i = 0; i<col; i++){
        mvaddch(0, i, '-');
    }
    mvprintw(0, (col-strlen(File.name))/2, " %s ", File.name);
    move(1, 0);
    
    current = File.content;    
    
    for(int i = 0; current->eof != true; i++){ // TODO: make me better, I dont look so good
        if(curLine < (row - 4)){
            mvaddch(curLine, i, current->letter);
        }else{
            break;
        }
        if(current->next == NULL){
            break;
        }    
        if(i >= col || current->letter == '\n'){
            i = -1;
            curLine++;
        }
        current = current->next;
    }
}


void updateFileTxt(openedFile File){
    int x, y;
    linkedList *current = File.content;
    int curLine = 1;
    getmaxyx(stdscr, y, x);
    werase(stdscr);
    
    for(int i = 0; i < File.line; ){
        if(current->letter == '\n'){
            i++;
        }
        current = current->next;
    }
    
    //Text Editor header
    for(int i = 0; i<x; i++){
        mvaddch(0, i, '-');
    }
    mvprintw(0, (x-strlen(File.name))/2, " %s ", File.name);
    wmove(stdscr, 1, 0);
    //Text Editor body
    for(int i = 0; current->eof == false; i++){
        if(curLine < (y - 4)){
            mvaddch(curLine, i, current->letter);
            if(current->letter == '\n'){
                i = -1;
                curLine++;
            }
            current = current->next;
        }else{
            break;
        }
    }
}