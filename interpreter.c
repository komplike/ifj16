#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "htab.h"
#include "stack.h"
#include "instrlist.h"
#include "interpreter.h"
#include "string.h"
#include "errors.h"


int interpreter(tListOfInstr *List)
{
	istack Stack;// pomocny stack pro hodnoty
	bistack BStack;//stack stack pro provedeni relacnich operaci

	Stack.top = NULL;
	BStack.top = NULL;
	
	iitem first, second, result;//polozky pro ukladani operandu a vysledku
	values value;
	
	bool Next = false;//nasledujici operace(predem nedefinovna)
	bool bval = true;

	int res;

	listFirst(List);//nacteme prvni instrukci
	tInstr *Instruction = NULL;

	Instruction = listGetData(List);//ziskame data


		while(List->active != NULL)//pokud seznam je aktivni 
		{
			if(Next)//pokud zadna instrukce neni nacte nasledujici ze seznamu
			{
				listNext(List);
			}

			Instruction = listGetData(List);//ziskani data
		
			Next = true;//nasledujici instrukce

			
			switch(Instruction -> instType)//typy jsednotlivych instrukci
			{
//*******************************************(MATEMATICKE OPERACE)(int))***************************************************************
//"+"
				case ADD_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_int) = (first.value.uval.val_int) + (second.value.uval.val_int);
					PUSHt(result.value.uval, &Stack);
				break;
//"-"		
				case SUB_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_int) = (first.value.uval.val_int) - (second.value.uval.val_int);
					PUSHt(result.value.uval, &Stack);
				break;
//"*"
				case MUL_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_int) = (first.value.uval.val_int) * (second.value.uval.val_int);
					PUSHt(result.value.uval, &Stack);
				break;
//"/"
				case DIV_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_int) = (second.value.uval.val_int) / (first.value.uval.val_int);
					PUSHt(result.value.uval, &Stack);
				break;
//******************************************(MATEMATICKE OPERACE(double))***************************************************************
				case FADD_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_double) = (first.value.uval.val_double) + (second.value.uval.val_double);
					PUSHt(result.value.uval, &Stack);
				break;

				case FSUB_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_double) = (first.value.uval.val_double) + (second.value.uval.val_double);
					PUSHt(result.value.uval, &Stack);
				break;

				case FMUL_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_double) = (first.value.uval.val_double) + (second.value.uval.val_double);
					PUSHt(result.value.uval, &Stack);
				break;

				case FDIV_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
		
					(result.value.uval.val_double) = (first.value.uval.val_double) + (second.value.uval.val_double);
					PUSHt(result.value.uval, &Stack);
				break;
//**********************************************(POSUNITI HODNOTY)************************************************************************
				case VALPUSH_I:
					PUSHv(Instruction->addr1, &Stack);
				break;

				case CONV_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
					
					(first.value.uval.val_double) = (double)(second.value.uval.val_int);
					PUSHt(result.value.uval, &Stack);
				break;

				case SWAP_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
					TOP(&(second.value), &Stack);
					POP(&Stack);
					
					PUSHt(first.value.uval, &Stack);
					PUSHt(second.value.uval, &Stack);
				break;
				
				case CONCAT_I:
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					int len = strlen(first.value.uval.val_string) + strlen(second.value.uval.val_string);
					
					result.value.uval.val_string = malloc((len+1)*sizeof(char));
					
					if(result.value.uval.val_string == NULL)
					{
						printError(INT_ERR);
					}
					
					strcpy((result.value.uval.val_string), (second.value.uval.val_string));
					strcat((result.value.uval.val_string), (first.value.uval.val_string));
				
					PUSHt(result.value.uval, &Stack);
					
				break;
//******************************************(RELACNI OPERACE(int))***********************************************************
//"<"
				case LESS_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int)< (second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//">"
				case GREAT_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int)>(second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"<="
				case LESSEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int) <= (second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//">="
				case GREATEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int) >= (second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"=="		
				case EQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int) == (second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"!="
				case NEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_int) != (second.value.uval.val_int))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//*******************************************(RELACNI OPERACE(double)*****************************************
//"<"
				case FLESS_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) < (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//">"
				case FGREAT_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) > (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"<="
				case FLESSEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) <= (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//">="
				case FGREATEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) >= (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"!="
				case FNEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) != (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//"=="
				case FEQ_I:
				
					TOP(&(first.value), &Stack);
					POP(&Stack);
				
					TOP(&(second.value), &Stack);
					POP(&Stack);
				
					if((first.value.uval.val_double) == (second.value.uval.val_double))
					{
						PUSHb(true, &BStack);
					}
					else
					{
						PUSHb(false, &BStack);
					}
				break;
//******************************************(INSTRUKCE SKOKU)**********************************************
//"goto"
				case GOTO_I:
					listGoto(List, Instruction->addr3);
					Next = false;
				break;
//"while"				
				case WHILE_I:	
					
					TOPb(&bval, &BStack);	
					if(bval)
					{
						listGoto(List, Instruction->addr3);
						Next = false;
					}
				break;
//"if"
				case IF_I:
					TOPb(&bval, &BStack);	
					POPb(&BStack);

					if(bval)
					{
						listGoto(List, Instruction->addr3);
						Next = false;
					}
				break;
//"label"
				case LABEL_I:
				break;
//"return"
				case RETURN_I:

					res = return_i(List);
					Next = false;
				break;
//volani instrukci
				case CALL_I:
			 
					res = call_func(List, &Stack,(struct htab_listitem*)Instruction->addr3);
					Next = false;
				break;
					
			}
		}
	
	
	return 0;
}
