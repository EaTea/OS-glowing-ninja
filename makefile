PROJ	= scheduler
BUILD_DIR = build
HEADER	= $(PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES= scheduler.c fileparse.c globals.c auxfns.c
OBJ= $(SOURCES:%.c=$(BUILD_DIR)/%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o build/$(PROJ) $(OBJ)

$(BUILD_DIR)/%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(BUILD_DIR)/$(PROJ) $(OBJ)
