#include <stdio.h>
#include <stdlib.h>
#include "instrlist.h"
#include "errors.h"

tInstList *ilist;

void InitL(tInstList *L)
{
	L->first = NULL;
	L->act = NULL;
	L->last = NULL;
}

void FreeL(tInstList *L)
{
	for(L->act = L->first; L->first != NULL; L->first = L-> act)
	{
		L->act = L->first->nItem;
		free(L->first);
	}
	L->last = NULL;
}

void FirstL(tInstList *L)
{
	L->act = L->first;
}

void ILastL(tInstList *L, tInst I)
{
	tItemList *item = malloc(sizeof(tItemList));
	
	if(item == NULL)
	{
		ErrExit(99);	
	}

	item->instr = I;
	item->nItem = NULL;

	if(L->first != NULL)
	{
		L->last->nItem = item;
		L->last = item;
	}
	else
	{
		L->first = l->Last = item;
	}
}

void NextL(tInstList *L)
{
	if(L->active != NULL)
	{
		L->act = L->act->nItem;
	}
}

void GoToL(tInstList *L, tItemList *GoTo)
{
	L->active = GoTo;
}

void *GLPL(tInstList *L)
{
	return (void *)L->last;
}

tInst *getdata(tInstList *L)
{
	if(L->act == NULL)
	{
		ErrExit(99);
	}
	else
	{
		return	&(L->act->instr);
	}
}

void i_add(int typ, void *p1, void *p2, void *p3)
{
	tInst instrukce = { typ, p1, p2, p3};
	
	ILastL(ilist, instrukce);
}

void i_print(tInst *inst)
{
	switch(inst -> TypInst)
	{
//int
		case ADD_I:
			printf("ADD");
			break;
		case SUB_I:
			printf("SUB");
			break;
		case MUL_I:
			printf("MUL");
			break;
		case DIV_I:
			printf("DIV");
			break;
		case LESS_I:
			printf("LESS");
			break;
		case GREAT_I:
			printf("GREAT");
			break;
		case LESEQ_I:
			printf("LESEQ");
			break;
		case GREATEQ_I:
			printf("GREATEQ");
			break;
		case EQ_I:
			printf("EQ");
			break;
		case NEQ_I:
			printf("NEQ");
			break;
//floats
		case FADD_I:
			printf("FADD");
			break;
		case FSUB_I:
			printf("FSUB");
			break;
		case FMUL_I:
			printf("FMUL");
			break;
		case FDIV_I:
			printf("FDIV");
			break;
		case FLESS_I:
			printf("FLESS");
			break;
		case FGREAT_I:
			printf("FGREAT");
			break;
		case FLESEQ_I:
			printf("FLESEQ");
			break;
		case FGREATEQ_I:
			printf("FGREATEQ");
			break;
		case FEQ_I:
			printf("FEQ");
			break;
		case FNEQ_I:
			printf("FNEQ");
			break;
//------
		case GOTO_I:
			printf("GOTO");
			break;
		case RETURN_I:
			printf("RETURN");
			break;
		case CALL_I:
			printf("CALL");
			break;
		case VPUSH_I:
			printf("VPUSH");
			break;
		case CONV_I:
			printf("CONV");
			break;
		case SWAP_I:
			printf("SWAP");
			break;
		case CONCAT_I:
			printf("CONCAT");
			break;
		case TAB_I:
			printf("TAB");
			break;
		case LABEL_I:
			printf("LABEL");
			break;
		default:
			printf("%d", inst->TypInst);
	}
	
	printf("%p %p %p, " inst->adr1, inst->adr2, inst-> adr3);
}

void il_print()
{
	tInst *instruction;
	
	FirstL(ilist);
	
	while(ilist->active != NULL)
	{
		instruction = getdata(ilist);
		
		i_print(instruction);
		printf("\n");
		
		NextL(ilist);
	}
}

		

