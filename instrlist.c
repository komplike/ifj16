#include <stdio.h>
#include <stdlib.h>
#include "instrlist.h"
#include "errors.h"

tListOfInstr *instr_list;

void listInit(tListOfInstr *L)
{

	L->first = NULL;
	L->active = NULL;
	L->last = NULL;
	
}

void listFree(tListOfInstr *L){

	for(L->active = L->first;L->first != NULL;L->first = L->active)
		{
			L->active = L->first->nextItem;		//zachovavame pointer na nastupny prvek
			free(L->first);
		}
	L->last = NULL;

}

void listInsertLast(tListOfInstr *L, tInstr I){

	tListItem * item;
	item = malloc(sizeof(tListItem));

	if(item == NULL)
	{
		printError(INT_ERR);
	}

	item->instruction = I;
	item->nextItem = NULL;

	if(L->first != NULL)
	{
	    L->last->nextItem = item;
	    L->last = item;
	}    
	else
{
		L->first = L->last = item;
	}

}

void printInstruction(tInstr *I) 
{
    printf("%p ", (void *)I);

    switch (I->instType) {
        case ADD_I: printf("ADD"); break;
        case SUB_I: printf("SUB"); break;
        case MUL_I: printf("MUL"); break;
        case DIV_I: printf("DIV"); break;
        case FADD_I: printf("FADD"); break;
        case FSUB_I: printf("FSUB"); break;
        case FMUL_I: printf("FMUL"); break;
        case FDIV_I:printf("FDIV"); break;
        case VALPUSH_I: printf("VAL_PUSH"); break;
        case TABPUSH_I: printf("TAB_PUSH"); break;
        case CONV_I: printf("CONV"); break;
        case SWAP_I: printf("SWAP"); break;
        case CONCAT_I: printf("CONCAT"); break;
        case LESS_I: printf("LESS"); break;
        case GREAT_I: printf("GREAT"); break;
        case LESSEQ_I: printf("LESSEQ"); break;
        case GREATEQ_I: printf("GREATEQ"); break;
        case EQ_I: printf("EQ"); break;
        case NEQ_I: printf("NEQ"); break;
        case FLESS_I: printf("FLESS"); break;
        case FGREAT_I: printf("FGREAT"); break;
        case FLESSEQ_I: printf("FLESSEQ"); break;
        case FGREATEQ_I: printf("FGREATEQ"); break;
        case FEQ_I: printf("FEQ"); break;
        case FNEQ_I: printf("FNEQ"); break;
        case GOTO_I: printf("GOTO"); break;
        default: printf("%d", I->instType);
    }

    printf(" %p %p %p, ", I->addr1, I->addr2, I->addr3);
}



void listFirst(tListOfInstr *L){

	 L->active = L->first;

}

void listNext(tListOfInstr *L){

	if (L->active != NULL)
  		L->active = L->active->nextItem;

}

tInstr *listGetData(tListOfInstr *L)
{

  if (L->active == NULL)
  {
    printError(INT_ERR);
  }
  else return &(L->active->instruction);

}

void listGoto(tListOfInstr *L, tListItem *gotoInstr)
{
  L->active =  gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L)
{
	return (void *)L->last;
}
