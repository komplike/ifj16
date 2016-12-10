#include "instrlist.h"
#include "interpreter.h"
#include "htab.h"

typedef struct tRamec
{	
	tPoRamec *first;//prvni instrukce
	tListItem *nextInstr;
	struct tRamec *next;
}tRamec;

typedef struct tPoRamec
{
	htab_listitem *var;
	values value;
	struct tPoRamec *next;
}tPoRamec;

values get_val(htab_listitem *var);//ziska hodnotu
void set_val(htab_listitem *var, ival *val);//nastavi hodnotu
void call_builtin_func(istack *stack, htab_listitem *built_in);//zavolat vestavene funkce
int add_item(tRamec *ramec, htab_listitem *var);//prida polozku
int set_frame(tRamec *ramec, htab_listitem *var, values val);//nastavi ramec
int get_frame(tRamec *ramec, htab_listitem *var, values *val);//ziska ramec
int free_frame(tRamec **ramec);//uvolni ramec

int return_i(tListofInstr *In);//instrukce return
int call_i(tListOfInstr, istack *S, htab_listitem *f);//volani funkce



