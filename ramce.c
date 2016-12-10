#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "htab.h"
#include "instrlist.h"
#include "string.h"
#include "errors.h"
#include "ramce.h"
#include "builtins.h"

tRamec *new = NULL;
tRamec *act = NULL;
//nastavi hodnotu
void set_val(htab_listitem *var, ival *value)
{
	set_frame(active, var, val->uval);
}
//ziskani hodnotu
values get_val(htab_listitem *var)
{
	values val1;
	get_frame(active, var, &val1);
	return val1;
}

//ziskani polozky
tPoRamec *get_item(tRamec *ramec, htab_listitem *var)
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
int add_item(tRamec *ramec, htab_listitem *var)
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
int set_frame(tRamec *ramec, htab_listitem *var, values val)
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
int get_frame(tRamec *ramec, htab_listitem *var, values *val)
{
	if(frame == NULL)
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

	*val = item->value;
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
	if(active == NULL)
	{
		printError(INT_ERR);
	}

	listGoto(In, active->next);

	tRamec *aux = active;
	active = active -> next;

	free_frame(&aux);

	
	if(active == NULL)
	{
		printError(INT_ERR);
	}
	

	return 0;
}
//volani vestavenych funkci
void call_builtin_func(istack *stack, htab_listitem *built_in)
{
	if(strcmp(built_in->name, "ifj16.readInt") == 0)
	{
		int result  = readInt();
		PUSHv((void *)(unsigned)result, stack);
	}
	else if(strcmp(built_in->name, "ifj16.readDouble") == 0)
	{
		double result = readDouble();
		PUSHv(void *)result, stack);
	}
	else if(strcmp(built_it->name, "ifj16.readString") == 0)
	{
		char *result = readString();
		PUSHv((void *)result, stack);
	}
}

int call_i(tListOfInstr *In, istack *stack, htab_listitem *f)
{
	if(strcmp(f->class, "CALL_FUNC") == 0)
	{
	
		new = malloc(sizeof(tRamec));
	
		if(new == NULL)
		{
			printError(INT_ERR);
		}
	
		new->first = NULL;
	
		htab_listitem *p = f->func;
	
			while(p != NULL)
			{
				add_item(new, p);
	
				ival p_val;
				TOP(&p_val, stack);
				POP(stack);
		
				set_frame(new, p, p_val.uval);
				p = p->next;
			}

		new->next = act;
		act = new;
		new = NULL;
	
		if(strcmp(f->name,"Nevim co")== 0)
		{
			act->next = NULL;
		}
		else
		{
			act->next = instrlist->act->nextItem;
		}
	
}	
		return 0;

}
