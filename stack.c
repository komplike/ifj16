#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

#include "stack.h"
#include "instrlist.h"

void POP(istack *Stack)
{
	iitem *aux;
	aux = (*Stack).top;
	(*Stack).top = (*Stack).top->next;
	free(aux);
}

void PUSHt(values val, istack *Stack)
{
	iitem *aux;
	aux = (iitem*)malloc(sizeof(iitem));
	
	aux->next = Stack->top;
	aux->value.uval = val;
	
	Stack->top = aux;
}

void PUSHv(void *val, istack *Stack)
{
	iitem *aux;
	aux = (iitem *)malloc(sizeof(iitem));

	if(aux == NULL)
	{
		return;
	}

	aux->next = Stack->top;
	aux->value.vval = val;
	
	Stack->top = aux;
}

void TOP(ival *val, istack *Stack)
{
	*val = Stack->top->value;
}

void bPOP(bistack *BStack)
{
	bitem *aux;
	aux = BStack->top;
	
	BStack->top = aux->next;
	free(aux);
}

void bPUSH(bool val, bistack *BStack)
{
	bitem *aux;
	aux = (bitem *)malloc(sizeof(bitem));

	if(aux == NULL)
	{
		return;
	}
	
	aux->value = val;
	aux->next = BStack->top;
	BStack->top = aux;
}

void bTOP(bool *val, bistack *BStack)
{
	*val = BStack->top->value;
}

void PRINT(istack *Stack)
{
	iitem *aux;
	aux = Stack->top;

	while(aux != NULL)
	{
		printf("%p ", aux->value.vval);
	
		aux = aux->next;
	}
}
