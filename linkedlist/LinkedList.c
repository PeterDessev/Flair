#include "LinkedList.h"

void appendNode(linkedList *current, char input){
    //linkedList *current = list;
    
    while(current->next != NULL){
        current = current->next;
    }

    current->next = (linkedList *) malloc(sizeof(linkedList));
    current->next->letter = input;
    current->next->next = NULL;
    current->next->prev = current;
}

linkedList *navigateNodes(linkedList *current, int input){
    if(input = 0){
        fprintf (stderr, "attempted to navigate ammount 0, illegal distance");
    }else if(input > 0){
        for(int i = 0; i < input; i++){
            if (current->next == NULL){
                break;
            }
            current = current->next;
        }
    }else if(input < 0){
        for(int i = 0; i < (-input); i++){
            if(current->prev->prev == NULL){
                break;
            }
            current = current->prev;
        }
    }
    return current;
}

linkedList *navigateCNodes(linkedList *current, char input){
    if(input == 'e'){
        while(current->next != NULL){
            current = current->next;
        }
    }else if(input == 'b'){
        while(current->prev->prev != NULL){
            current = current->prev;
        }
    }else{
        fprintf(stderr, "Unknown navigation command: %c", input);
    }
    return current;
}

linkedList *removeNode(linkedList *current){
    linkedList *temp = current;
    if (current->prev->prev == NULL){
        //printf("prev is null\n");
        temp = current;
        current->prev->next = current->next;
        current->next->prev = current->prev;
        current = temp->next;
    }else if(current->next == NULL){
        //printf("next is null\n");
        temp = current->prev;
        current->prev->next = NULL;
    }else{
        //printf("nothing is null\n");
        temp = current->next;
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    return current;
}

void addNode(linkedList *current, char input){
    linkedList *temp = current->next;
    current->next = (linkedList *) malloc(sizeof(linkedList));
    current->next->next = temp;
    current->next->prev = current;
    current->next->letter = input;
    temp = NULL;
}

linkedList *nextNode(linkedList *current){
    return current->next ? current->next : NULL;
}