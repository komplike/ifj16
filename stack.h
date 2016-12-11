#ifndef STACK_H_
#define STACK_H_

#include "instrlist.h"
#include "interpreter.h"
#include "htab.h"
//ADT pro FCE pro praci se zasobnikem
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
//ramci
typedef struct bistack
{
	bitem *top;
}bistack;

typedef struct tRamec
{	
	struct tPoRamec *first;
	tListItem *nextInstr;
	struct tRamec *next;
}tRamec;

typedef struct tPoRamec
{
        struct htab_listitem_t *var;
	values value;
	struct tPoRamec *next;
}tPoRamec;

values get_val(struct htab_listitem * var);
void set_val( struct htab_listitem * var, ival *value);


int return_i(tListOfInstr *In);
int call_func(tListOfInstr *In, istack *stack, struct htab_listitem *f);



void POP(istack *Stack);//odstrani prvek z vrcholu zasobniku a vrati jeho hodnotu
void PUSHt(values val, istack *Stack);//vlozi hodnotu na vrchol stacku
void PUSHv(void *val, istack *Stack);//
void TOP(ival *val, istack *Stack);
void bPOP(bistack *BStack);
void bPUSH(bool val, bistack *BStack);
void bTOP(bool *val, bistack *BStack);
void PRINT(istack *Stack);


#endif

