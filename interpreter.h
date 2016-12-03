#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "htab.h"
#include "instrlist.h"

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

typedef bitem
{
	bool value;
	struct bitem *next;
}bitem;

typedef struct bistack
{
	bitem *top;
}bistack;

void POP(istack *Stack);
void PUSHt(value val, istack *Stack);
void PUSHv(void *val, istack *Stack);
void TOP(ival *val, istack *Stack);
void bPOP(bistack *BStack);
void bPUSH(bool val, bistack *BStack);
void bTOP(bool *val, bistack *BStack);

int interpreter(htab_t *Table, tListOfInstr *L);

#endif

