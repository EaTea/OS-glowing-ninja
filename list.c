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
	//TODO: Ensure that when a FRAME is created next is always NULL
	if(tmp != NULL)
	{
		tmp->next = f;
		++(l->size);
		l->last = f;
		f->previous = tmp;
		f->next = NULL;
	}
	else
	{
		l->first = f;
		l->last = f;
	}
}

int isInList(FRAME_LIST* l, char* pname, int line, FRAME* f)
{
	if(l == NULL || f == NULL || pname == NULL)
	{
		//TODO: Error
		return 0;
	}
	f = l->first;
	if(f != NULL)
	{
		while(f != NULL && strcmp(pname, f->pname) && line != f->lineStart)
			f = f->next;
		return f != NULL;
	}
	return 0;
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
		p->next = f->next;
		f->next = l->first;
		f->previous = NULL;
	}
}

void destroyList(FRAME_LIST* l)
{
	//TODO: cleanup
}
