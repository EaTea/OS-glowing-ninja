PROJ	= scheduler
BUILD_DIR = build
HEADER	= $(PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES= scheduler.c fileparse.c globals.c auxfns.c logger.c
OBJ= $(SOURCES:%.c=%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o build/$(PROJ) $(OBJ)

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(BUILD_DIR)/$(PROJ) $(OBJ)

cleanLog:
	rm -f logs/*
