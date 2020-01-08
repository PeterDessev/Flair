#include "linkedlist/LinkedList.h" //linked lists
#include <ncurses/curses.h>

#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
//#include <getopt.h> //add me when adding long arguments
  
int row, col;

linkedList *getFileContents(FILE *fp){
    int c;
    
    linkedList *head;
    head = (linkedList *) malloc(sizeof(linkedList));
    head -> letter = 0;
    head -> next = NULL;
    head -> prev = NULL;
    
    for(int i = 0; i == i; i++){
        c = fgetc(fp);
        if(feof(fp)){
            break;
        }
        appendNode(head, c);
    }
    return head;
}
 

int drawScreen(char *filename, FILE *fp){
    linkedList *FileText;
    linkedList *current;
    char *extension = filename;
    const char extDelim[2] = ".";
    int c;
    int line = 1;    
    initscr(); 
    getmaxyx(stdscr,row,col);
    
    while(strchr(extension, '.') != extension){
        extension = strchr(extension, '.');
    }
    
    if(has_colors() == FALSE){	
        endwin();
		fprintf(stderr, "Your terminal does not support color\n");
		exit(1);
	}
    
    //Text Editor header
    for(int i = 0; i<col; i++){
        mvaddch(0, i, '-');
    } 
    mvprintw(0, (col-strlen(filename))/2, " %s ", filename);
    move(1, 0);

    //File text holds the head and thus by proxy the file, current is the current character of the file the program is on.
    FileText = getFileContents(fp);
    current = FileText->next;
    
    
    //addNode(current, 'f');
    for(int i = 0; i == i; i++){
        
        if(line < (row - 4)){
            //printf("%c", current->letter);
            mvaddch(line, i, current->letter);
        }
        if(nextNode(current) == NULL){
            break;
        }    
        if(i >= col || current->letter == '\n'){
            i = -1;
            line++;
        }
        current = nextNode(current);
    }

    
    return 0;
}



int main(int argc, char **argv){
    opterr = 0; //does not print errors, but returns ? on error when checking arguments
    int opt;
    char *filename = NULL;
    char workingDir[PATH_MAX];
    FILE *fp; //new file variable
    
    //Check input options when executing Editor
    while((opt = getopt(argc, argv, "hf:")) != -1){
        switch(opt){
            case 'h':
                printf("To start, input the command 'editor -f filename' into the console where filename is the file you want to edit\n");
                abort();
            case 'f':
                if(strlen(optarg) <= FILENAME_MAX){
                    filename = optarg;
                    break;
                }else{
                    fprintf (stderr, "Filename %s is too long. \n", optarg);
                }
            case '?':
                if (optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
        } 
    }
    
    //get the current dir
    _getcwd(workingDir, FILENAME_MAX);
    //format the working directory charstring so that we get the absolute path of the file
    strcat(workingDir, "\\"); 
    strcat(workingDir, filename);
    	
    //check if file exists, if false asks to create one
    if ((fp = fopen(workingDir, "r+")) == NULL){
        int input;
        fprintf(stderr, "File does not exist, create new file with name %s? (y/n)", filename);
        input = getchar();
        input = tolower(input);
        if(input == 'y'){
            fp = fopen(workingDir, "w+");
        }else{
            abort();
        }
    }
    
    //function to initiate editor and add all the 
    drawScreen(filename, fp);
    
    endwin();
    fclose(fp);
    return 0;
}
