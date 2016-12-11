#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "htab.h"
#include "instrlist.h"
#include "string.h"
#include "errors.h"
#include "ramce.h"
#include "stack.h"


tRamec *new = NULL;
tRamec *act = NULL;
//nastavi hodnotu
void set_val(struct htab_listitem *var, ival *value)
{
	set_frame(act, var, value->uval);
}
//ziskani hodnoty
values get_val(struct htab_listitem *var)
{
	values val1;
	get_frame(act, var, &val1);
	return val1;
}


//ziskani polozky
tPoRamec *get_item(tRamec *ramec, struct htab_listitem *var)
{
	tPoRamec *tmp = ramec->first;
	
	while( tmp != NULL)
	{
		if(tmp->var == var)
		{
			return tmp;
		}
	
		tmp = tmp->next;
	}
	
	return NULL;
}
//pridat item
int add_item(tRamec *ramec, struct htab_listitem *var)
{
	tPoRamec *tmp = malloc(sizeof(tPoRamec));

	if(tmp == NULL)
	{
		printError(INT_ERR);
	}

	tmp->var = var;
	
	values zero;
	zero.val_int  = 0;
	tmp->value = zero;

	tmp->next = ramec->first;
	ramec->first = tmp;
	return 0;
}
//nastavit ramec
int set_frame(tRamec *ramec,struct  htab_listitem *var, values val)
{
	if(ramec == NULL)
	{
		printError(INT_ERR);
	}
	
	tPoRamec *aux = get_item(ramec, var);
	
	if(aux == NULL)
	{
		if(add_item(ramec, var) == 99)
		{
			printError(INT_ERR);
		}

		aux = get_item(ramec, var);
		
		if(aux == NULL)
		{
			printError(INT_ERR);
		}

	}
	
	aux->value = val;

	return 0;
}
//ziskat ramec
int get_frame(tRamec *ramec, struct htab_listitem *var, values *val)
{
	if(ramec == NULL)
	{
		printError(INT_ERR);
	}

	tPoRamec *aux = get_item(ramec, var);
	
	if(aux  == NULL)
	{
		if(add_item(ramec, var) == INT_ERR)
		{
			printError(INT_ERR);
		}

		aux = get_item(ramec, var);

		if(aux == NULL)
		{
			printError(INT_ERR);
		}

	}

	*val = aux->value;
	return 0;
}
// uvolnit ramec
void free_frame(tRamec **ramec)
{
	tPoRamec *aux = (*ramec)->first;
	
	while(aux != NULL)
	{
		tPoRamec *tmp = aux;
		aux = aux->next;
		free(tmp);
	}

	free(*ramec);
	*ramec = NULL;
}
//instrukce return
int return_i(tListOfInstr *In)
{
	if(act == NULL)
	{
		printError(INT_ERR);
	}

	listGoto(In, act->nextInstr);

	tRamec *aux = act;
	act = act -> next;

	free_frame(&aux);

	
	if(act == NULL)
	{
		printError(INT_ERR);
	}
	

	return 0;
}


int call_func(tListOfInstr *In, istack *stack, struct htab_listitem *f)
{
	if(strcmp((f->type), "CALL_FUNC") == 0 )//jestli je funkce
	{
	
		new = malloc(sizeof(tRamec));//vytvorime novej ramec
		
		if(new == NULL)
		{
			printError(INT_ERR);
		}
	
		new->first = NULL;//nastavime prvni jako prazdnej
	
		struct htab_listitem *p;//prvni parametr
		
		p = f->fp;
	
		while(p != NULL)//pokud neni prazdny
		{
			ival p_val;
	
			add_item(new, p);
			TOP(&p_val, stack);
			POP(stack);
		
			set_frame(new, p, p_val.uval);
			p = p->np;
		}
		
		new->next = act;
		act = new;
		new = NULL;
	
		if(strcmp(f->name, "run") == 0)
		{
			act->nextInstr = NULL;
		}
		else
		{
			act->nextInstr = In->active->nextItem;
		}
	
		listGoto(In, f->fi);
	}
	
	return 0;
}


