#ifndef RAMCE_H_
#define RAMCE_H_

#include "instrlist.h"
#include "interpreter.h"
#include "htab.h"

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


#endif
