PROJ	= scheduler
HEADER	= ($PROJ).h
CC	= gcc -std=c99
CFLAGS 	= -Wall
OBJ 	= scheduler.o fileparse.o globals.o auxfns.o

$(PROJ)	: $(OBJ)
	$(CC) -o $(PROJ) $(OBJ)

%.o	: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(PROJECT) $(OBJ)