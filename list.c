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
		l->last = f;
		f->previous = tmp;
		f->next = NULL;
	}
	else
	{
		l->first = f;
		l->last = f;
	}
	++(l->size);
}

int isInList(FRAME_LIST* l, char* pname, int line, FRAME* f)
{
	if(l == NULL || pname == NULL)
	{
		//TODO: Error
		return 0;
	}
	f = l->first;
	while(f != NULL)
	{
		if(!strcmp(pname, f->pname))
			if(f->lineStart == line)
				break;
		f = f->next;
	}

	return f != NULL;
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
		p->next = f->next;
		f->next = l->first;
		l->first->previous = f;
		f->previous = NULL;
		l->first = f;
	}
}

void destroyList(FRAME_LIST* l)
{
	//TODO: cleanup
}
