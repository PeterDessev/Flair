#include "languageC.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int addChar(myScreen *Screen){
    if(Screen->curLine < (Screen->row - 4)){
        mvaddch(Screen->curLine, Screen->i, Screen->current->letter);
    }else{
        return 1;
    }
    if(Screen->current->next->eof == true){
        return 1;
    }    
    if(Screen->i >= Screen->col || Screen->current->letter == '\n'){
        Screen->i = -1;
        Screen->curLine++;
    }
    Screen->i++;
    Screen->current = Screen->current->next;
    return 0;
}

int isComment(myScreen *Screen){
    if(Screen->current->letter == '/'){
        if(Screen->current->next->letter == '/' || Screen->current->next->letter == '*'){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int colorWord(char *input[], myScreen *Screen, int color[], int size){
    int check = 0;
    int notFound = 0;
    for(int i = 0; i < size; i++){
        notFound = 0;
        int length = strlen(input[i]);
        char word[length + 1];
        char wordp[length + 2];
        strcpy(wordp, stringFrom(Screen->current, length + 1));
        strcpy(word, stringFrom(Screen->current, length));
        
        if(wordp[length] < 65 || wordp[length] > 122){
            
            if(strcmp(input[i], word) == 0){
                attron(COLOR_PAIR(color[i]));
                for(int n = length; n > 0; n--){  
                    check = addChar(Screen);
                      
                    if(check == 1){
                        return 1;
                    }
                }
                attroff(COLOR_PAIR(color[i]));
                break;
            }
        }
        notFound = 1;
    }
    if(notFound == 1){
        attron(COLOR_PAIR(1));
        check = addChar(Screen);
        attroff(COLOR_PAIR(1));
    }
    return 0;
}

void initCLang(openedFile File){
    myScreen *Screen = malloc(sizeof(myScreen));
    Screen->enableColor = true;
    Screen->current = File.content;
    Screen->curLine = 1;
    Screen->i = 0;
    int check = 0;
    initscr();
    getmaxyx(stdscr, Screen->row, Screen->col);
    start_color();    
    
    if(has_colors() == false){	
        Screen->enableColor = false;
		fprintf(stderr, "Your terminal does not support color\n");
	}
    
    //Text Editor header    
    for(int i = 0; i<Screen->col; i++){
        mvaddch(0, i, '-');
    }
    mvprintw(0, (Screen->col-strlen(File.name))/2, " %s ", File.name);
    move(1, 0);
    
    if(Screen->enableColor == true){
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_WHITE, COLOR_BLACK);
        init_pair(6, COLOR_BLUE, COLOR_BLACK);
    }
    
    while(check != 1){
        if(Screen->current->letter > 47 && Screen->current->letter < 58){
            attron(COLOR_PAIR(6));
            while(check != 1){
                check = addChar(Screen);
                if(Screen->current->letter == 47 || Screen->current->letter <= 45 || Screen->current->letter >= 58){
                    attroff(COLOR_PAIR(6));
                    break;
                }
            }
        }
        switch(Screen->current->letter){    
            case '"':
                attron(COLOR_PAIR(2));
                check = addChar(Screen);
                while(check != 1){
                    check = addChar(Screen);
                    if(Screen->current->letter == '"'){
                        check = addChar(Screen);
                        break;
                    }
                }
                attroff(COLOR_PAIR(2));
                break;
            case '/': //comments
                if(Screen->current->next->letter == '/'){
                    attron(COLOR_PAIR(3));
                    check = addChar(Screen);
                    while(check != 1){
                        check = addChar(Screen);
                        if(Screen->current->letter == '\n'){
                            check = addChar(Screen);
                            break;
                        }
                    }
                    attroff(COLOR_PAIR(3));
                    break;
                }else if(Screen->current->next->letter == '*'){
                    attron(COLOR_PAIR(3));
                    check = addChar(Screen);
                    while(check != 1){
                        check = addChar(Screen);
                        if(Screen->current->letter == '*' && Screen->current->next && Screen->current->next->letter == '/'){
                            check = addChar(Screen);
                            check = addChar(Screen);
                            break;
                        }
                    }
                    attroff(COLOR_PAIR(3));
                break;
                }
            case '#':
                attron(COLOR_PAIR(1));
                while(check != 1){
                    check = addChar(Screen);
                    if(Screen->current->letter == '\n' || isComment(Screen)){
                        break;
                    }
                }
                attroff(COLOR_PAIR(1));
                break;
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ':':
            case ';':
            case ',':
            case '.':
                attron(COLOR_PAIR(5));
                check = addChar(Screen);
                attroff(COLOR_PAIR(5));
                break;
            case '=':
            case '+':
            case '-':
            case '*':
            case '!':
            case '%':
            case '&':
            case '<':
            case '>':
            case '?':
            case '|':
                attron(COLOR_PAIR(4));
                check = addChar(Screen);
                attroff(COLOR_PAIR(4));
                break;
            case 'a':{
                char *words[1] = {"auto"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'b':{
                char *words[1] = {"break"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }   
           case 'c':{
                char *words[4] = {"case", "char", "const", "continue"};
                int colors[4] = {4, 5, 4, 4};
                check = colorWord(words, Screen, colors, 4);
                break;
            }
            case 'd':{
                char *words[3] = {"default", "do", "double"};
                int colors[3] = {4, 4, 5};
                check = colorWord(words, Screen, colors, 3);
                break;
            }
            case 'e':{
                char *words[3] = {"else", "enum", "extern"};
                int colors[3] = {4, 4, 4};
                check = colorWord(words, Screen, colors, 3);
                break;
            }
            case 'f':{
                char *words[2] = {"float", "for"};
                int colors[2] = {5, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'g':{
                char *words[1] = {"goto"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'l':{
                char *words[1] = {"long"};
                int colors[1] = {5};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'r':{
                char *words[2] = {"register", "return"};
                int colors[2] = {4, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 't':{
                char *words[1] = {"typedef"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'u':{
                char *words[2] = {"union", "unsigned"};
                int colors[2] = {4, 5};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'v':{
                char *words[2] = {"void", "volatile"};
                int colors[2] = {5, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'w':{
                char *words[1] = {"while"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'i':{
                char *words[2] = {"if", "int"};
                int colors[2] = {4, 5};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 's':{
                char *words[7] = {"short", "signed", "short", "sizeof", "static", "struct", "switch"};
                int colors[7] = {5, 5, 5, 4, 4, 4, 4};
                check = colorWord(words, Screen, colors, 7);
                break;
            }
            default:
                attron(COLOR_PAIR(1));
                check = addChar(Screen);
                attroff(COLOR_PAIR(1));
                break;
        }
    }
    free(Screen);
}

void updateCLang(openedFile File){   
    myScreen *Screen = malloc(sizeof(myScreen));
    Screen->enableColor = true;
    Screen->current = File.content;
    Screen->curLine = 1;
    Screen->i = 0;
    int check = 0;
    werase(stdscr);
    getmaxyx(stdscr, Screen->row, Screen->col);
    
    if(has_colors() == false){	
        Screen->enableColor = false;
	}
    
    for(int i = 0; i < File.line; ){
        if(Screen->current->letter == '\n'){
            i++;
        }
        Screen->current = Screen->current->next;
    }
    
    //Text Editor header    
    for(int i = 0; i<Screen->col; i++){
        mvaddch(0, i, '-');
    }
    mvprintw(0, (Screen->col-strlen(File.name))/2, " %s ", File.name);
    move(1, 0);
    
    while(check != 1){
        if(Screen->current->letter > 47 && Screen->current->letter < 58){
            attron(COLOR_PAIR(6));
            while(check != 1){
                check = addChar(Screen);
                if(Screen->current->letter == 47 || Screen->current->letter <= 45 || Screen->current->letter >= 58){
                    attroff(COLOR_PAIR(6));
                    break;
                }
            }
        }
        switch(Screen->current->letter){    
            case '"':
                attron(COLOR_PAIR(2));
                check = addChar(Screen);
                while(check != 1){
                    check = addChar(Screen);
                    if(Screen->current->letter == '"'){
                        check = addChar(Screen);
                        break;
                    }
                }
                attroff(COLOR_PAIR(2));
                break;
            case '/': //comments
                if(Screen->current->next->letter == '/'){
                    attron(COLOR_PAIR(3));
                    check = addChar(Screen);
                    while(check != 1){
                        check = addChar(Screen);
                        if(Screen->current->letter == '\n'){
                            check = addChar(Screen);
                            break;
                        }
                    }
                    attroff(COLOR_PAIR(3));
                    break;
                }else if(Screen->current->next->letter == '*'){
                    attron(COLOR_PAIR(3));
                    check = addChar(Screen);
                    while(check != 1){
                        check = addChar(Screen);
                        if(Screen->current->letter == '*' && Screen->current->next && Screen->current->next->letter == '/'){
                            check = addChar(Screen);
                            check = addChar(Screen);
                            break;
                        }
                    }
                    attroff(COLOR_PAIR(3));
                break;
                }
            case '#':
                attron(COLOR_PAIR(1));
                while(check != 1){
                    check = addChar(Screen);
                    if(Screen->current->letter == '\n' || isComment(Screen)){
                        break;
                    }
                }
                attroff(COLOR_PAIR(1));
                break;
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ':':
            case ';':
            case ',':
            case '.':
                attron(COLOR_PAIR(5));
                check = addChar(Screen);
                attroff(COLOR_PAIR(5));
                break;
            case '=':
            case '+':
            case '-':
            case '*':
            case '!':
            case '%':
            case '&':
            case '<':
            case '>':
            case '?':
            case '|':
                attron(COLOR_PAIR(4));
                check = addChar(Screen);
                attroff(COLOR_PAIR(4));
                break;
            case 'a':{
                char *words[1] = {"auto"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'b':{
                char *words[1] = {"break"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }   
           case 'c':{
                char *words[4] = {"case", "char", "const", "continue"};
                int colors[4] = {4, 5, 4, 4};
                check = colorWord(words, Screen, colors, 4);
                break;
            }
            case 'd':{
                char *words[3] = {"default", "do", "double"};
                int colors[3] = {4, 4, 5};
                check = colorWord(words, Screen, colors, 3);
                break;
            }
            case 'e':{
                char *words[3] = {"else", "enum", "extern"};
                int colors[3] = {4, 4, 4};
                check = colorWord(words, Screen, colors, 3);
                break;
            }
            case 'f':{
                char *words[2] = {"float", "for"};
                int colors[2] = {5, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'g':{
                char *words[1] = {"goto"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'l':{
                char *words[1] = {"long"};
                int colors[1] = {5};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'r':{
                char *words[2] = {"register", "return"};
                int colors[2] = {4, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 't':{
                char *words[1] = {"typedef"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'u':{
                char *words[2] = {"union", "unsigned"};
                int colors[2] = {4, 5};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'v':{
                char *words[2] = {"void", "volatile"};
                int colors[2] = {5, 4};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 'w':{
                char *words[1] = {"while"};
                int colors[1] = {4};
                check = colorWord(words, Screen, colors, 1);
                break;
            }
            case 'i':{
                char *words[2] = {"if", "int"};
                int colors[2] = {4, 5};
                check = colorWord(words, Screen, colors, 2);
                break;
            }
            case 's':{
                char *words[7] = {"short", "signed", "short", "sizeof", "static", "struct", "switch"};
                int colors[7] = {5, 5, 5, 4, 4, 4, 4};
                check = colorWord(words, Screen, colors, 7);
                break;
            }
            default:
                attron(COLOR_PAIR(1));
                check = addChar(Screen);
                attroff(COLOR_PAIR(1));
                break;
        }
    }
    free(Screen);
}