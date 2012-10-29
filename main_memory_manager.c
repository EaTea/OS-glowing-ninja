#include "os-project.h"

static FRAME_LIST * mainMemoryList;
static const int cacheSize = 8;

void initialiseMainMemory()
{
	//make the list contaiing all the frames
	if (lf) fprintf(logger,"Initialising main memory\n");
	mainMemoryList = newList();
	for(int i = 0; i < cacheSize; i++)
	{
		appendToList(mainMemoryList, newFrame());
	}
}

void tearDownMainMemory()
{
	destroyList(mainMemoryList);
}

FRAME* newFrame()
{
	FRAME* f = malloc(sizeof(FRAME));
	f->pname[0] = '\0';
	f->lineStart = -2;
	f->page = NULL;
	f->page = calloc(sizeof(char*),2);
	f->page[0] = calloc(sizeof(char),BUFSIZ);
	f->page[1] = calloc(sizeof(char),BUFSIZ);
	f->next = NULL;
	f->previous = NULL;
	return f;
}

void recursiveDestroyFrame(FRAME* f)
{
	//tail recursively destroy the frame f
	FRAME* tmp = f->next;
	free(f->page[0]);
	free(f->page[1]);
	free(f->page);
	free(f);
	if(tmp != NULL)
		recursiveDestroyFrame(tmp);
}

static void recursiveDumpToStream(FILE* stream, FRAME* f, int fnumber)
{
	//base case
	if(f == NULL) return;
	fprintf(stream,"Main Memory: Frame %d\nProcess Stored: %s\n", fnumber, f->pname);
	fprintf(stream,"%s\n%s\n",f->page[0],f->page[1]);
	recursiveDumpToStream(stream,f->next,fnumber+1);
} 
void dumpMainMemoryToStream(FILE* stream)
{
	fprintf(stream, "Time: %d\n", timesToTakeDumps[nextTimeToDumpIndex]);
	recursiveDumpToStream(stream,mainMemoryList->first,1);
}

//loads PROCESS p's currentLine and currentLine+1 into a frame
void loadIntoMainMemory(PROCESS* p, int currentLine)
{
	if(p == NULL)
	{
		//TODO:throw an error!
		return;
	}
	if(currentLine <= 0 || currentLine > p->nlines)
	{
		//throw an error
		puts("Tried to load a block entirely outside of process into cache\n");
		return;
	}
	//load into the last FRAME; that's the FRAME that's been used the least
	FRAME* f = mainMemoryList->last;
	strcpy(f->pname, p->pname);
	f->lineStart = currentLine;
	strcpy(f->page[0],p->lines[currentLine-1]);
	strcpy(f->page[1], currentLine+1 <= p->nlines ? p->lines[currentLine] : NO_VALUE);
	//move the frame back to the beginning since it's the most recently used
	updateMainMemory(f);
}

void updateMainMemory(FRAME* f)
{
	bringElementToFront(mainMemoryList,f);
}

int inMainMemory(PROCESS* p, int currentLine, FRAME** f)
{
	if(p == NULL || currentLine <= 0 || currentLine > p->nlines)
	{
		//TODO: error
		return -1;
	}
	//inMainMemory checks that there exists two frames f1, and f2 such that 
	//f1.page[0] = currentLine
	//f1.page[1] = currentLine+1
	//f2.page[0] = currentLine+2
	//f2.page[1] = currentLine+3
	return isInList(mainMemoryList,p->pname,currentLine,f);
}
