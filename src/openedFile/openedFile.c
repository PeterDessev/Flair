#include "openedFile.h"
#include "languages.h"
#include <ncurses.h>

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