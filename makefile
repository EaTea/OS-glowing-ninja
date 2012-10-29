PROJ	= os-project
HEADER	= $(PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall -pedantic -g -O2

SOURCES = file_parse.c globals.c aux_fcns.c logger.c fcfs.c rr.c queue.c list.c line_processing.c cache_manager.c main_memory_manager.c main.c
OBJ= $(SOURCES:%.c=%.o)

$(PROJ)	: $(OBJ)
	$(CC) -o $(PROJ) $(OBJ)

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:	
	echo "Removing all object files and the project executable in build directory"
	rm -f $(PROJ)
	rm -f *.o
