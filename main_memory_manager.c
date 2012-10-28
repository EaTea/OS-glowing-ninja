#include "os-project.h"

static FRAME_LIST * mainMemoryList;

void initialiseMainMemory()
{
	mainMemoryList = newList();
}

int inMainMemory(PROCESS* p, int currentLine)
{
	return 1;
}
