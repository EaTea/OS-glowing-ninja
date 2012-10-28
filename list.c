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
	}
	else
	{
		l->first = f;
		l->last = f;
	}
}

int isInList(FRAME_LIST* l, char* pname, int line, FRAME* f, FRAME* p)
{
	if(l == NULL || f == NULL || p == NULL)
	{
		//TODO: Error
		return 0;
	}
	p = NULL;
	f = l->first;
	if(f != NULL)
	{
		while(f->next != NULL && strcmp(pname, f->pname) && (line < f->lineStart || line > f->lineStart+1))
		{
			p = f;
			f = (l->first)->next;
		}
		return f != NULL;
	}
	return 0;
}

//assumes that this f is already in l
void bringElementToFront(FRAME_LIST* l, FRAME* f, FRAME* p)
{
	if(l == NULL || f == NULL || p == NULL)
	{
		//TODO: Error
		return;
	}
	p->next = f->next;
	f->next = l->first;
}

void destroyList(FRAME_LIST* l)
{
	//TODO: cleanup
}
