#include "os-project.h"

FRAME_LIST* newList()
{
	FRAME_LIST* l = malloc(sizeof(FRAME_LIST));
	l->first = NULL;
	l->last = l->first;
	l->size = 0;
	return l;
}

//assumes frame is well-formed
void appendToList(FRAME_LIST* l, FRAME* f)
{
	if(l == NULL || f == NULL)
	{
		//TODO: Error
		return;
	}
	FRAME* tmp = l->last;
	if(tmp != NULL)
	{
		tmp->next = f;
		f->previous = tmp;
		f->next = NULL;
	}
	else
	{
		l->first = f;
	}
	l->last = f;
	++(l->size);
}

int isInList(FRAME_LIST* l, char* pname, int line, FRAME** f)
{
	//notice pass by reference to retrieve the frame
	if(l == NULL || pname == NULL)
	{
		//TODO: Error
		return 0;
	}
	*f = l->first;
	//look through the FRAMEs and compare the name
	while(*f != NULL)
	{
		//FRAME equality condition; same process and start line is the same as the looked for line
		if(!strcmp(pname, (*f)->pname))
			if((*f)->lineStart == line)
				break;
		*f = (*f)->next;
	}
	return *f != NULL;
}

//assumes that this f is already in l
void bringElementToFront(FRAME_LIST* l, FRAME* f)
{
	if(l == NULL || f == NULL)
	{
		//TODO: Error
		return;
	}
	FRAME* p = f->previous;
	//otherwise f is the very front of the list anyway
	if(p != NULL)
	{
		//moving the element at the very end to the very beginning
		if(f == l->last)
		{
			l->last = p;
		}
		if(f->next != NULL)
			(f->next)->previous = p;
		//do all the swaps to ensure the doubly linked referentiality is maintained
		p->next = f->next;
		f->next = l->first;
		l->first->previous = f;
		f->previous = NULL;
		l->first = f;
	}
}

void destroyList(FRAME_LIST* l)
{
	recursiveDestroyFrame(l->first);
	free(l);
}
