PROJ	= os-project
BUILD_DIR = build
HEADER	= $(PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES= main.c file_parse.c globals.c aux_fcns.c logger.c
OBJ= $(SOURCES:%.c=%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o build/$(PROJ) $(OBJ)

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:	
	echo "Removing all object files and the project executable in build directory"
	rm -f $(BUILD_DIR)/$(PROJ) $(OBJ)

cleanLog:
	echo "Removing all log files in ./logs with extension .log"
	rm -f logs/*.log
