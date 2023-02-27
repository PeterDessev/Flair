#include "LinkedList.h"
#include <stdbool.h>

void appendNode(linkedList *current, char input, bool eof){ //only to be used when contstructing the linked list from the FP
    while(current->next != NULL){
        current = current->next;
    }

    current->next = malloc(sizeof(linkedList));
    if(eof){
        current->next->eof = true;
        current->next->letter = input; 
    }else{
        current->next->eof = false;
        current->next->letter = input;
    }
    current->next->next = NULL;
    current->next->prev = current;
}

linkedList *navigateNodes(linkedList *current, int input){
    if(input == 0){
        fprintf (stderr, "\nattempted to navigate ammount 0, illegal distance");
    }else if(input > 0){
        for(int itter = 0; itter < input; itter++){
            if (current->next->eof == true){
                break;
            }
            current = current->next;
        }
    }else if(input < 0){        
        for(int itter = 0; itter < (-input); itter++){
            if(current->prev == NULL){
                break;
            }
            current = current->prev;
        }
    }
    return current;
}

linkedList *removeNode(linkedList *current){ //deletes the current node
    if (current->prev != NULL){
        linkedList *temp = current;
        current = current->prev;
        current->next = temp->next;
        
        current = current->next;
        current->prev = temp->prev;
        free(temp);
    }else if(current->prev == NULL){
        current = current->next;
        free(current->prev);
        current->prev = NULL;
        if(current->prev != NULL){
            fprintf(stderr, "ERROR while removing node where prev was null: prev is not null");
        }
    }
    return current;
}

void addNode(linkedList *current, char input){ //adds a node previous to the one given
    if(current->prev == NULL){
        current->prev = malloc(sizeof(linkedList));
        
        current->prev->next = current;
        current->prev->prev = NULL;
        current->prev->letter = input;
        current->prev->eof = false;
    }else{
        linkedList *addedNode = malloc(sizeof(linkedList));
        addedNode->letter = input;
        addedNode->eof = false;
        addedNode->prev = current->prev;
        addedNode->next = current;

        current->prev = addedNode;
        current = current->prev->prev;
        current->next = addedNode;
    }
}

int getLineLength(linkedList *current){
    int x = 0;
    while(current->prev != NULL && current->prev->letter != '\n'){
        current = current->prev;
    }
    while(current->eof == false && current->letter != '\n'){
        current = current->next; 
        x++;
    }
    return x;
}

linkedList *getEOL(linkedList *current){
    while(current->eof == false && current->letter != '\n'){
        current = current->next;
    }
    if(current->eof){
        return current->prev;
    }else{
        return current;    
    } 
}

linkedList *getPrevEOL(linkedList *current){
    if(current->prev != NULL && current->letter == '\n'){
        current = current->prev;
    }
    while(current->prev != NULL && current->letter != '\n'){
            current = current->prev;
    }
    return current;
}

void printAll(linkedList *current){
    while(current->prev != NULL){
        current = current->prev;
    }
    while(current->eof == false){
        printf("%c", current->letter);
        current = current->next;
    }
}

const char *stringFrom(linkedList *current, int length){
    char word[length+1];
    for(int i = 0; i < length; i++){
        if(current->eof == true){
            break;
        }
        word[i] = current->letter;
        current = current->next;
    }
    return word;
}