#ifndef STACK_H_
#define STACK_H_

#include "htab.h"

typedef union
{
	values uval;
	void *vval;
}ival;

typedef struct iitem
{
	ival value;
	struct iitem *next;
}iitem;

typedef struct istack
{
	iitem *top;
}istack;

typedef struct bitem
{
	bool value;
	struct bitem *next;
}bitem;

typedef struct bistack
{
	bitem *top;
}bistack;

void POP(istack *Stack);//odstrani prvek z vrcholu zasobniku a vrati jeho hodnotu
void PUSHt(values val, istack *Stack);//vlozi hodnotu na vrchol stacku
void PUSHv(void *val, istack *Stack);//
void TOP(ival *val, istack *Stack);
void bPOP(bistack *BStack);
void bPUSH(bool val, bistack *BStack);
void bTOP(bool *val, bistack *BStack);
void PRINT(istack *Stack);


#endif

