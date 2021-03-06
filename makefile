PROJ	= os-project
BUILD_DIR = build
HEADER	= $(PROJ).h queue.h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES = file_parse.c globals.c aux_fcns.c logger.c fcfs.c rr.c queue.c list.c line_processing.c cache_manager.c main_memory_manager.c main.c
OBJ= $(SOURCES:%.c=%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o $(BUILD_DIR)/$(PROJ) $(OBJ)

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

#TODO: Remove the queue target
clean:	
	echo "Removing all object files and the project executable in build directory"
	rm -f $(BUILD_DIR)/$(PROJ) $(BUILD_DIR)/qtest $(OBJ)
	rm -f *.*~

cleanLog:
	echo "Removing all log files in ./logs with extension .log"
	rm -f logs/*.log
