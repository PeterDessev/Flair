#cat -e -t makefile

.RECIPEPREFIX := >

LIBS = -lncurses

INCLUDE = linkedlist/linkedlist.c

a.exe: editor.c
>gcc $< $(LIBS) $(INCLUDE) -o $@

clean:
>rm $(TARGETS)