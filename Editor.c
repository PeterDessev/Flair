#include <ctype.h>  //char stuff
#include <ncurses/curses.h>
#include <stdbool.h>  //booleans and stuff
#include <stdint.h>   //special ints
#include <stdio.h>    //file io and stuff
#include <unistd.h>   //POSIX api

#include "types/FileText.h"
#include "types/languageC.h"
//#include <getopt.h> // FUTURE: add me when adding long arguments

linkedList *getFileContents(FILE *fp) {
    int c = fgetc(fp);
    linkedList *head = malloc(sizeof(linkedList));
    if (c != -1) {
        head->next = NULL;
        head->prev = NULL;
        head->eof = false;
        head->letter = c;

        while (c != -1) {
            c = fgetc(fp);
            if (c != -1) {
                appendNode(head, c, false);
            } else {
                break;
            }
        }
        appendNode(head, 0, true);
    } else {
        head->next = NULL;
        head->prev = NULL;
        head->eof = true;
        head->letter = 0;
    }
    fclose(fp);
    return head;
}

void drawFile(openedFile File) {
    if (strcmp(File.extension, "c") == 0) {
        initCLang(File);
    } else if (strcmp(File.extension, "txt") == 0) {
        initFileTxt(File);
    } else {
        printf("Filetype %s does not have coloring in Flair\n", File.extension);
    }
}

void updateFile(openedFile File) {
    if (strcmp(File.extension, "c") == 0) {
        updateCLang(File);
    } else if (strcmp(File.extension, "txt") == 0) {
        updateFileTxt(File);
    } else {
        printf("Filetype %s does not have coloring in Flair\n", File.extension);
        updateFileTxt(File);
    }
}

void saveFile(openedFile File) {
    linkedList *current = File.content;
    FILE *fp = fopen(File.fullPath, "w");

    while (current->eof == false) {
        fputc(current->letter, fp);
        current = current->next;
    }

    fclose(fp);
}

void keyboardListener(openedFile File) {
    linkedList *current = File.content;
    linkedList *holder;
    keypad(stdscr, true);
    int userInput, lineLength;
    uint16_t row, col, y, x;
    bool isSave = true;

    cbreak();
    wmove(stdscr, 1, 0);
    getmaxyx(stdscr, row, col);

    while (1) {
        noecho();
        userInput = wgetch(stdscr);
        getyx(stdscr, y, x);

        if ((userInput > 31 && userInput < 127) || (userInput == 10)) {
            isSave = false;
            addNode(current, userInput);
            if (current->prev != NULL && current->prev->prev == NULL) {
                File.content = current->prev;
            }

            if (userInput != 10) {
                updateFile(File);
                wmove(stdscr, y,
                      x + 1);  // TODO: add support for line extension if the
                               // user types a line longer than screen
            } else {
                if (y >= (row - 4)) {
                    File.line++;
                    printf("%d", File.line);
                    updateFile(File);
                } else {
                    updateFile(File);
                    wmove(stdscr, y + 1, 0);
                }
            }
        } else {
            switch (userInput) {
                case 8:  // backspace
                    if (current->prev != NULL) {
                        if (current->prev->letter ==
                            '\n') {  // TODO: line/scroll nav
                            lineLength = getLineLength(current->prev);
                        }
                        current = removeNode(current->prev);
                        if (current->prev ==
                            NULL) {  // update the File object if the first char
                                     // of file is deleted, since Content is
                                     // defined as the first char of the file
                            File.content = current;
                        }
                        updateFile(File);
                        if (x == 0 && current->prev != NULL) {
                            wmove(stdscr, y - 1, lineLength);
                        } else {
                            wmove(stdscr, y, x - 1);
                        }
                    }
                    break;
                case 16:  // ctrl+p
                    printAll(current);
                    break;
                case 19:  // ctrl+s
                    isSave = true;
                    saveFile(File);
                    break;
                case 27:  // esc
                    if (isSave == false) {
                        for (int i = 0; i < col; i++) {
                            mvaddch(0, i, '-');
                        }
                        mvwprintw(stdscr, 0, col / 2 - 13,
                                  "Exit without saving? (y/n)");
                        userInput = wgetch(stdscr);
                        if (userInput == 'y') {
                            printf("exit without save command given");
                            exit(0);
                        } else {
                            wmove(stdscr, y, x);
                            break;
                        }
                    }
                    printf("exit command given");
                    exit(0);
                case 258:  // down
                    if (y >= (row - 5)) {
                        int oldEOF = current->eof;
                        current = getEOL(current)->next;
                        if (oldEOF == 1) {
                            break;
                        }
                        if (current->eof == true) {
                            if (current->prev->letter == '\n') {
                                wmove(stdscr, y, 0);
                            } else {
                                int length = getLineLength(current);
                                wmove(stdscr, y, length);
                                break;
                            }
                        }
                        File.line++;
                        updateFile(File);
                        wmove(stdscr, y, 0);

                        for (int i = 0; i < x;
                             i++) {  // moves across either to the end of the
                                     // line to to the row it was at before
                            if (current->letter == '\n' ||
                                current->eof == true) {
                                wmove(stdscr, (y), i);
                                break;
                            }
                            current = current->next;
                            wmove(stdscr, (y), i + 1);
                        }
                    } else {
                        int oldEOF = current->eof;
                        current = getEOL(current)->next;
                        if (oldEOF == 1) {
                            break;
                        }
                        if (current->eof == true) {
                            if (current->prev->letter == '\n') {
                                wmove(stdscr, y + 1, 0);
                            } else {
                                int length = getLineLength(current);
                                wmove(stdscr, y, length);
                                break;
                            }
                        }
                        wmove(stdscr, y + 1, 0);
                        for (int i = 0; i < x;
                             i++) {  // moves across either to the end of the
                                     // line to to the row it was at before
                            if (current->letter == '\n' ||
                                current->eof == true) {
                                wmove(stdscr, (y + 1), i);
                                break;
                            }
                            current = current->next;
                            wmove(stdscr, (y + 1), i + 1);
                        }
                    }
                    break;
                case 259:  // up
                    if (y > 1) {
                        if (current->prev->letter == '\n') {
                            if (current->prev->prev == NULL) {
                                current = current->prev;
                                wmove(stdscr, 1, 0);
                            } else {
                                current = getPrevEOL(getPrevEOL(current));
                                if (current->prev != NULL) {
                                    current = current->next;
                                }
                                wmove(stdscr, y - 1, 0);
                            }
                        } else {
                            current = getPrevEOL(getPrevEOL(current));
                            if (current->prev == NULL) {
                                wmove(stdscr, y - 1, 0);
                                break;
                            } else {
                                current = current->next;
                            }
                            for (int i = 0;; i++) {
                                if (current->letter == '\n') {
                                    wmove(stdscr, y - 1, i);
                                    break;
                                } else if (i < x) {
                                    wmove(stdscr, y - 1, i + 1);
                                    current = current->next;
                                    // TODO: add refresh of top bar here when
                                    // removing debugger tool
                                    break;
                                }
                            }
                        }
                    } else {
                        if (File.line > 0) {
                            File.line--;
                            updateFile(File);
                            if (current->prev->letter == '\n') {
                                if (current->prev->prev == NULL) {
                                    current = current->prev;
                                    wmove(stdscr, 1, 0);
                                } else {
                                    current =
                                        getPrevEOL(getPrevEOL(current))->next;
                                    wmove(stdscr, y, 0);
                                }
                            } else {
                                current = getPrevEOL(getPrevEOL(current));
                                if (current->prev == NULL) {
                                    wmove(stdscr, y, 0);
                                    break;
                                } else {
                                    current = current->next;
                                }
                                for (int i = 0;; i++) {
                                    if (current->letter == '\n') {
                                        wmove(stdscr, y, i);
                                        break;
                                    } else if (i < x) {
                                        wmove(stdscr, y, i + 1);
                                        current = current->next;
                                        // TODO: add refresh of top bar here
                                        // when removing debugger tool
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case 260:  // left
                    if (current->prev !=
                        NULL) {  // cursor is not at the first char of the file
                        if (current->prev->letter ==
                            '\n') {  // cursor is at the begining of a new line
                            current = current->prev;
                            holder = current;
                            for (int i = 0;; i++) {
                                holder = holder->prev;
                                if (holder->prev == NULL ||
                                    holder->prev->letter == 10) {
                                    x = ++i;
                                    break;
                                }
                            }
                            if (y == 1) {
                                File.line--;
                                updateFile(File);
                                wmove(stdscr, y, x);
                            } else {
                                wmove(stdscr, (y - 1), x);
                            }
                        } else {  // cursor is in some line with text and not on
                                  // either end
                            wmove(stdscr, y, (x - 1));
                            current = current->prev;
                        }
                    }
                    break;
                case 261:  // right
                    if (current->eof == false) {
                        if (current->letter == '\n') {
                            current = current->next;
                            if (y >= (row - 5)) {
                                File.line++;
                                updateFile(File);
                                wmove(stdscr, y, 0);
                            } else {
                                wmove(stdscr, (y + 1), 0);
                            }
                        } else {
                            wmove(stdscr, y, (x + 1));
                            current = current->next;
                        }
                    }
                    break;
                case 330:  // delete (????)
                    if (current->eof == false) {
                        current = current->next;
                        current = removeNode(current->prev);
                        if (current->prev ==
                            NULL) {  // update the File object if the first char
                                     // of file is deleted, since Content is
                                     // defined as the first char of the file
                            File.content = current;
                        }

                        updateFile(File);
                        wmove(stdscr, y, x);
                    }
                    break;
                default:
                    printf("%d\n", userInput);
            }
        }
        // temp debugger tool
        getyx(stdscr, y, x);
        for (int i = 0; i < col; i++) {
            mvaddch(0, i, '-');
        }
        if (current->prev != NULL) {
            if (current->letter != '\n' && current->prev->letter != '\n') {
                mvwprintw(stdscr, 0, col / 2 - 12,
                          "Current/Previous Letter: %c, %c", current->letter,
                          current->prev->letter);
            } else if (current->letter == '\n' &&
                       current->prev->letter != '\n') {
                mvwprintw(stdscr, 0, col / 2 - 12,
                          "Current/Previous Letter: \\n, %c",
                          current->prev->letter);
            } else if (current->letter != '\n' &&
                       current->prev->letter == '\n') {
                mvwprintw(stdscr, 0, col / 2 - 12,
                          "Current/Previous Letter: %c, \\n", current->letter);
            } else {
                mvwprintw(stdscr, 0, col / 2 - 12,
                          "Current/Previous Letter: \\n, \\n");
            }
        } else {
            mvwprintw(stdscr, 0, col / 2 - 12,
                      "Current/Previous Letter: %d, NULL-", current->letter);
        }
        wmove(stdscr, y, x);

        refresh();
    }
}

char *getFileExtension(openedFile file) {
    char *extension = strtok(file.name, ".");

    while (extension != NULL) {
        extension = strtok(NULL, ".");
        if (strtok(NULL, ".") == NULL) {
            break;
        }
    }

    return extension;
}

void getFullPath(char Dir[], openedFile file) {
    strcpy(Dir, file.fileDir);
    strcat(Dir, "\\");
    strcat(Dir, file.name);
}

int main(int argc, char **argv) {
    opterr = 0;  // does not print errors, but returns ? on error when checking
                 // arguments
    int opt;
    FILE *fp;
    char path[PATH_MAX + 1];

    printf("Flair version: 1.0.0\n");

    openedFile currentFile;

    // Check input options when executing Editor
    opt = getopt(argc, argv, "hf:");
    if (opt == -1) {
        char input[PATH_MAX];
        printf("Enter name of file to open: \n");
        scanf("%[^\n]s", input);
        strcpy(currentFile.name, input);
    } else {
        while (opt != -1) {
            switch (opt) {
                case 'h':
                    printf(
                        "To start, input the command 'editor -f filename' into "
                        "the console where filename is the file you want to "
                        "edit\n");
                    exit(0);
                case 'f':
                    if (strlen(optarg) <= FILENAME_MAX) {
                        strcpy(currentFile.name, optarg);
                        break;
                    } else {
                        fprintf(stderr, "Filename %s is too long. \n", optarg);
                    }
                case '?':
                    if (optopt == 'f') {
                        fprintf(stderr, "Option -%c requires an argument.\n",
                                optopt);
                    } else if (isprint(optopt)) {
                        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                    } else {
                        fprintf(stderr, "Unknown option character `\\x%x'.\n",
                                optopt);
                    }
                    exit(0);
                default:
                    exit(0);
            }
            opt = getopt(argc, argv, "hf:");
        }
    }

    // set file properties
    _getcwd(currentFile.fileDir, PATH_MAX);

    getFullPath(path, currentFile);
    strcpy(currentFile.fullPath, path);

    // logic for file creation or opening
    fp = fopen(currentFile.fullPath, "r+");
    if (fp == NULL) {
        fp = fopen(currentFile.fullPath, "r");
        if (fp == NULL) {
            int input;
            fprintf(stderr,
                    "ver 0.1.2\nFile does not exist, create new file with name "
                    "%s? (y/n)\n",
                    currentFile.name);
            input = fgetc(stdin);
            input = tolower(input);
            if (input == 'y') {
                fp = fopen(currentFile.fullPath, "w+");
            } else {
                exit(0);
            }
        } else {
            printf("File can only be read, opening in read-only mode\n");
        }
    }

    // function to initiate editor and finish setting file properties
    currentFile.content = getFileContents(fp);
    fclose(fp);
    currentFile.extension = getFileExtension(currentFile);
    currentFile.line = 0;

    // draw the file
    drawFile(currentFile);

    keyboardListener(currentFile);
    getch();

    endwin();
    return 0;
}
