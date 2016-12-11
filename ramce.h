#include "instrlist.h"
#include "interpreter.h"
#include "htab.h"

typedef struct tRamec
{	
	tPoRamec *first;
	tListItem *nextInstr;
	struct tRamec *next;
}tRamec;

typedef struct tPoRamec
{
	htab_listitem *var;
	values value;
	struct tPoRamec *next;
}tPoRamec;

values get_val(htab_listitem *var);
void set_val(htab_listitem *var, ival *val);
int add_item(tRamec *ramec, htab_listitem *var);
int set_frame(tRamec *ramec, htab_listitem *var, values val);
int get_frame(tRamec *ramec, htab_listitem *var, values *val);
int free_frame(tRamec **ramec);

int return_i(tListofInstr *In);
int call_i(tListOfInstr, istack *S, htab_listitem *f);



