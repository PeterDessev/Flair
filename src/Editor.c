#include <ctype.h> //char stuff
#include <ncurses.h>
#include <stdbool.h> //booleans and stuff
#include <stdint.h>  //special ints
#include <stdio.h>   //file io and stuff
#include <string.h>
#include <unistd.h> //POSIX api

#include "languages.h"
// #include <getopt.h> // FUTURE: add me when adding long arguments

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

void getFullPath(char Dir[], openedFile file) {
  strcpy(Dir, file.fileDir);
  strcat(Dir, "\\");
  strcat(Dir, file.name);
}

uint8_t handleArguments(char *fileName, uint32_t argc, char *argv, int opt) {
  while (opt != -1) {
    switch (opt) {
    case 'h':
      printf("To start, input the command 'editor -f filename' into "
             "the console where filename is the file you want to "
             "edit\n");
      exit(0);
    case 'f':
      if (strlen(optarg) <= FILENAME_MAX) {
        strcpy(fileName, optarg);
        break;
      } else {
        fprintf(stderr, "Filename %s is too long. \n", optarg);
      }
    case '?':
      if (optopt == 'f') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      exit(0);
    default:
      exit(0);
    }
    opt = getopt(argc, argv, "hf:");
  }
}

int main(int argc, char **argv) {
  opterr = 0; // does not print errors, but returns ? on error when checking
              // arguments
  int opt;
  FILE *fp;
  char path[PATH_MAX + 1];

  printf("Flair version: 1.0.0\n");

  openedFile currentFile;

  // Get input options when executing Editor
  opt = getopt(argc, argv, "hf:");

  if (opt == -1) {
    char input[PATH_MAX];
    printf("Enter name of file to open: \n");
    if (!fgets(input, PATH_MAX - 1, stdin)) {
      printf("Error getting user input\n");
      return 1;
    }

    // Remove newline from end of string
    uint8_t inputLen = strlen(input) - 1;

    input[inputLen] = 0;
    strncpy(currentFile.name, input, inputLen);
  } else {
    handleArguments(currentFile.name, argc, argv, opt);
  }

  // set file properties
  getcwd(currentFile.fileDir, PATH_MAX);

  getFullPath(path, currentFile);
  strcpy(currentFile.fullPath, path);

  // logic for file creation or opening
  fp = fopen(currentFile.fullPath, "r+");
  if (fp == NULL) {
    fp = fopen(currentFile.fullPath, "r");
    if (fp == NULL) {
      int input;
      fprintf(stderr,
              "\nFile does not exist, create new file with name "
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
