#include "os-project.h"

static FRAME_LIST * mainMemoryList;
static const int cacheSize = 8;
static const char* NO_VALUE = "<<NO LINE STORED IN FRAME>>";

void initialiseMainMemory()
{
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
	f->page = malloc(sizeof(char*)*2);
	f->page[0] = malloc(sizeof(char)*BUFSIZ);
	f->page[1] = malloc(sizeof(char)*BUFSIZ);
	f->next = NULL;
	return f;
}

void recursiveDestroyFrame(FRAME* f)
{
	FRAME* tmp = f->next;
	free(f->page[0]);
	free(f->page[1]);
	free(f->page);
	free(f);
	if(tmp != NULL)
		recursiveDestroyFrame(tmp);
}

void loadIntoMainMemory(PROCESS* p, int currentLine)
{
	if(p == NULL)
	if(currentLine < 0 || currentLine > p->nlines)
	{
		//throw an error
		puts("Tried to load a block entirely outside of process into cache\n");
		return;
	}
	FRAME* f = mainMemoryList->last;
	strcpy(f->pname, p->pname);
	f->lineStart = currentLine;
	strcpy(f->page[0],p->lines[currentLine-1]);
	strcpy(f->page[1], currentLine+1 <= p->nlines ? p->lines[currentLine] : NO_VALUE);
}

int inMainMemory(PROCESS* p, int currentLine)
{
	//leverage knowledge that cache is always of size 8
	return 1;
}
