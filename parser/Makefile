CC=gcc
CFLAGS= -std=c11

#UNAME Shell Variable
UNAME_S := $(shell uname -s)

# OS X Dependant Variables
ifeq ($(UNAME_S), Darwin)
    LIB := sharedLib.dylib
# GNU/Linux Depedant Variables
else ifeq ($(UNAME_S), Linux)
    LIB := sharedLib.so
endif

$(LIB): LinkedListAPI.o parsingUtilities.o VCardParser.o
	$(CC) -shared -o $(LIB) LinkedListAPI.o parsingUtilities.o VCardParser.o

sharedLib.o: src/LinkedListAPI.c src/parsingUtilities.c src/VCardParser.c
	$(CC) $(CFLAGS) src/LinkedListAPI.c src/parsingUtilities.c src/VCardParser.c -Iinclude -c -fpic

clean:
	rm $(LIB) *.o