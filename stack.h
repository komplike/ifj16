#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "htab.h"
//stack pro hodnoty
typedef union
{
	values uval;
	void *vval;
}ival;
//stack pro polozky v stacku
typedef struct iitem
{
	ival value;
	struct iitem *next;
}iitem;
//stack, ktery potrebujeme pro praci s interpretem
typedef struct istack
{
	iitem *top;
}istack;
//polozka v stacku 
typedef struct bitem
{
	bool value;
	struct bitem *next;
}bitem;
//stack pro zpracovani logickych operaci
typedef struct bistack
{
	bitem *top;
}bistack;

void POP(istack *Stack);
void PUSHt(values val, istack *Stack);
void PUSHv(void *val, istack *Stack);
void TOP(ival *val, istack *Stack);
void bPOP(bistack *BStack);
void bPUSH(bool val, bistack *BStack);
void bTOP(bool *val, bistack *BStack);
void PRINT(istack *Stack);


#endif

