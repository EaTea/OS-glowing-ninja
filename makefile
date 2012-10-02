PROJ	= scheduler
BUILD_DIR = build
HEADER	= $(PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES= scheduler.c fileparse.c globals.c auxfns.c logger.c fcfs.c
OBJ= $(SOURCES:%.c=%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o $(BUILD_DIR)/$(PROJ) $(OBJ)

#TODO: Get rid of this target later
qtest: queue.c queue.h qtest.c
	$(CC) -o $(BUILD_DIR)/queue queue.c queue.h qtest.c

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

#TODO: Remove the queue target
clean:	
	echo "Removing all object files and the project executable in build directory"
	rm -f $(BUILD_DIR)/$(PROJ) $(BUILD_DIR)/qtest $(OBJ)

cleanLog:
	echo "Removing all log files in ./logs with extension .log"
	rm -f logs/*.log
