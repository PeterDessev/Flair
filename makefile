CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lncurses
SOURCES=editor.c\
     linkedlist/linkedlist.c\
     openedFile/openedFile.c\
     types/languageC.c\
     types/FileText.c
     
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=flair


all: $(SOURCES) $(EXECUTABLE) rm

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
rm: 
	rm $(OBJECTS)