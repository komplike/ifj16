/* ****************************************************************************
 * funkcie na pracu s tabulkou symbolov
 * dole je zakomentovany main
 * autor: Michal Slavka
 * ***************************************************************************/
#include "htab.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
/* ****************************************************************************
 * hachovacia funkcia
 * ***************************************************************************/
unsigned int hash_function(const char *str, unsigned htab_size) 
{
  unsigned int h=0;
  const unsigned char *p;
  for(p=(const unsigned char*)str; *p!='\0'; p++) 
    h = 65599*h + *p; 
  
  return h % htab_size; 
}

/* ****************************************************************************
 * inicializacia tabulky
 * ***************************************************************************/
struct htab *htab_init(int size) 
{

	int i = 0;
  	// alokacia pamate pre strukturu
	htab_t *t = (htab_t *)malloc(sizeof(htab_t) + sizeof(struct htab_listitem *)*size);
  	if (t == NULL) 
  		return NULL;

  	t->size = size;

  	for (i = 0; i < size; i++) {// inicializacia ukazatelov
    	t->list[i] = NULL;
	}
	
	return t;
}

/* ****************************************************************************
 * vyhladavanie prvku v tabulke
 * ak sa v tabulke nenachadza vrati NULL
 * inak vracia ukazatel na prvok
 * ***************************************************************************/
struct htab_listitem *htab_lookup(htab_t *t, const char *class, const char *name) 
{
	unsigned int hash_i = hash_function(name,4); //index v hashovacej tabulke

	struct htab_listitem *ptr = t->list[hash_i];

	while (ptr != NULL) {
	if (strcmp(name, ptr->name) == 0) {
			if  (class != NULL) {
				if (strcmp(class, ptr->class) == 0)	
					return ptr;
			}
			else 
				return ptr;
		ptr = ptr->next;
		}
	}

	return NULL;
}

/* ****************************************************************************
 * pridava do ts list_item
 * !!!ak sa list_item uz v taulke nachadza tak budeduplikovany	
 * vracia 1 ak bolo pridanie uspesne
 * vracia 0 ak zliha alokacia pamate
 * parametre:
   ts, trieda, meno prvku, je definovany, typ, jeho hodnota
 * ***************************************************************************/
int htab_add(htab_t *htab, const char *class, const char *name, int def, int type, const char *value) 
{
	//printf("%s\n", name);
	if (name == NULL)
	{	//pojde prec po odladeni
		printf("%s\n", "chyba meno");
		return 99;
	}

	if (def == DEF && value == NULL)
	{	//pojde prec po odladeni
		if (type != FUNC) {
		printf("%s\n", "je def ale chyvba value");
		return 99;
		}
	}

	if (type == FUNC && class == NULL) {
		printf("%s\n", "funkicii chyba trieda");
		return 99;

	}

	struct htab_listitem *new_item = (struct htab_listitem *)malloc(sizeof(htab_listitem_t));
	if (new_item == NULL) {
		printf("%s\n", "zlyhala alokacia noveho prvku");
		return 99;
	}
	//inicializacia noveho prvku
	new_item->class = str_cpy(class);;
	new_item->name = str_cpy(name);
	new_item->def = def;
	new_item->type = type;

	if (type == FUNC)
		new_item->func = NULL;
	else if (def == DEF) {
		if (type == K_INT)
			new_item->val.val_int = str_int(value);
		else if (type == K_DOUBLE)
			new_item->val.val_double = str_double(value);
		else if (type == K_STRING)
			new_item->val.val_string = str_cpy(value);
	}
	else if (type == K_STRING)
		new_item->val.val_string = NULL;

	//pridanie prvku do tabulky
	unsigned int i = hash_function(name, 4);
	new_item->next = htab->list[i];
	htab->list[i] = new_item;
	
	return 0;
}

/* ****************************************************************************
 * uvolnuje tabulku symbolov
 * parametre: tabulka symbolov, jej velkost
 * ***************************************************************************/ 
void htab_free(struct htab *t, int size)
{
	int i;
	struct htab_listitem *ptr1 = NULL;
	struct htab_listitem *ptr2 = NULL;


	for (i = 0; i < size; i++) {
		ptr1 = t->list[i];
		while (ptr1 != NULL) {
			ptr2 = ptr1;
			ptr1 = ptr1->next;
			free(ptr2->class);
			free(ptr2->name);
			if (ptr2->type == K_STRING && ptr2->val.val_string != NULL) {
				free(ptr2->val.val_string);
			}
			if (ptr2->type == FUNC  && ptr2->func != NULL) {
				free(ptr2->func->args);
				//TODO	free(ptr->inst_ptr);
				htab_free(ptr2->func->local_t, size);
				free(ptr2->func);
			}

			free(ptr2);
		}
	}
	free(t);	

}

/* ****************************************************************************
 * doplnenenie informaci o funkcii
 * parametre:
   globalna TS, trieda funkcie, meno funkcie, navratovy typ, zoznam argumentov
 * vracia 0 pri uspechu
 * ***************************************************************************/ 
int htab_add_func(htab_t *t, const char *class, const char *name, int return_type, arg_l *args){
	//struct function *fun;
	struct htab_listitem *item = htab_lookup(t, class, name);
	//int i;			//pomocne pocitadlo
	//int argc = 0;	//pocet argumentov funkcie

	if (item == NULL) {
		printf("funkcia: %s.%s() sa nenasla\n", class, name);
		return 100;
	}
	//ak v tabulke funkcia pridana ale este nema doplnene argumenty, navratovy typ etc.
	if (item->func == NULL) {
		struct function *f = (function_t *)malloc(sizeof(function_t));
		if (f == NULL) {
			printf("%s\n", "htab_add_func: zlyhala alokacia");
			return 99;
		}
		//dodatocne informacie do ts
		f->return_type = return_type;
		f->argc = args->cnt;
		f->args = args_arr(args, args->cnt);		//funkcii sa preda zoznam obsahujuci argumenty
		f->local_t = htab_init(LTAB_SIZE);			//inicializacia lokalnej tabulky symbolov
		item->func = f;								//pridanie do tabulky
		if (item->func->local_t == NULL || item->func->args == NULL) {		//kontrola ci sa podarila alokacia
			return 99;		//po navrate treba vsetko uvolnit!!!
		}
		arg_add(item->func->local_t, args);		//pridanie argumentov do lokalnej tabulky symbolov
		//uvedenie zoznamu parametrov do inicializovaneho stavu
		args->cnt = 0;						
		args->act = args->first;
	}

	return 0;
}


/* ****************************************************************************
 * inicializacia zoznamu argumentov
 * ***************************************************************************/ 
arg_l *arg_init(){
	struct arg_list *a = (struct arg_list *)malloc(sizeof(struct arg_list));
	if (a == NULL){
		//error(99)
		return NULL;
	}

	a->first = NULL;
	a->act = NULL;
	a->last = NULL;
	a->cnt = 0;

		//alokacia prveho prvku
	a->first = (struct arg *)malloc(sizeof(arg_t));
	if (a->first == NULL) {
		return NULL;
	}
	else {
		a->first->class = NULL;
		a->first->name = NULL;
		a->first->next = NULL;
		a->act = a->first;
		a->last = a->first;
	}

	return a;
}


/* ****************************************************************************
 * pridavanie argumentov do lokalnej tabulky symbolov
 * musi ist ako posledna funkcia ktora pracuje s argumentmi
 * ***************************************************************************/ 
int arg_add(htab_t *t, arg_l *a){
	int i, ret;
	a->act = a->first;

	for (i = 0; i < a->cnt; i++) {
		if ((ret = htab_add(t, a->act->class, a->act->name, NDEF, a->act->type, NULL)) != 0)
			return ret;
		if (a->act->class != NULL)
			free(a->act->class);
		if (a->act->name != NULL){
			free(a->act->name);
		}
		a->act->class = NULL;
		a->act->name = NULL;
		a->act = a->act->next;
	}
	return 0;
}

/* ****************************************************************************
 * indexovat sa zacina od 1
 * ak sa nebudu davat parametre po rade nebude to fungovat
 * parametre:
   inicializovany zoznam, trieda, meno argumentu, typ
 * ***************************************************************************/

int arg_ctor(arg_l *a, const char *class, const char *name, int type){	
	if (a == NULL){
		return 99;
	}
	a->act->class = str_cpy(class);
	a->act->name  = str_cpy(name);
	a->act->type  = type;
	a->cnt++;
	if (a->act->next == NULL) {
		a->act->next = (struct arg *)malloc(sizeof(struct arg));
	}
	if (a->act->next == NULL) {
		return 99;
	}
	else {
		a->act = a->act->next;
		a->act->class = NULL;
		a->act->name = NULL;
		a->act->next = NULL;
		a->last = a->act;
	}
	return 0;
}

/* ****************************************************************************
 * pridava argumenty definicie a volania funkcie do ts 
 * ***************************************************************************/
int *args_arr(arg_l *a, int argc){
	int *type_arr = (int *)malloc(sizeof(int)*argc);
	if (type_arr == NULL)
		return NULL;
	int c = 0; //pocitadlo pre index
	a->act = a->first;

	for (c = 0; c < argc; c++) {
		type_arr[c] = a->act->type;
		a->act = a->act->next;
	}
	return type_arr;
}

/* ****************************************************************************
 * uvolnuje zoznam argumentov
 * ***************************************************************************/
void args_free(arg_l *a) 
{
	a->act = a->first;
	while (a->last != NULL) {
		//free(a->act->name);
		//free(a->act->class);
		if (a->act != a->last) {
			a->act = a->act->next;
			free(a->first);
			a->first = a->act;
		}
		else {
			free(a->act);
			a->first = NULL;
			a->act   = NULL;
			a->last  = NULL;
		}
	}
	free(a);
}
/* ****************************************************************************
 * kopia stringov
 * ***************************************************************************/
char *str_cpy(const char *str) 
{
	if (str == NULL)
		return NULL;
	char *new_str = (char*)malloc(strlen(str)+1);
	if (new_str == NULL)
		return NULL;
	/*unsigned int i;
	for (i = 0; i < strlen(str); ++i)
	{
		new_str[i] = str[i];
	}*/
	strcpy(new_str,str);
	return new_str;
}


/* ****************************************************************************
 * zmeni string na int
 * ***************************************************************************/
int str_int(const char* i)
{

	return atoi(i);
}

/* ****************************************************************************
 * TODO
 * convertuje string na double
 * ***************************************************************************/
double str_double(const char *d)
{
	if (d[0] > '0'+8)
	printf("%s\n", d);
	return 1.0;
}


//*****************************************************************
/* ****************************************************************************
 * vytlaci vsetky prvky tabulky
 * ***************************************************************************/
int for_each(htab_t *t, int size, foreach_func func)
{
	int i;
	struct htab_listitem *ptr;

	for (i = 0; i < size; i++) {
		ptr = t->list[i];
		while (ptr != NULL) {
			func(ptr);
			ptr = ptr->next;
		}
	}

	return 1;
}

/* ****************************************************************************
 * vytlaci prvok
 * ***************************************************************************/
void print(struct htab_listitem *l) 
{	
	if (l == NULL) {
		printf("print: bol predany NULL ptr\n");
		return;
	}

	
	if (l->class != NULL|| l->name != NULL) {
		printf("name: %s.%s\n", l->class, l->name);
	}
	else if (l->name == NULL) {
		printf("Vo funkci print: chyba meno\n");
	}
	else {
		printf("name: (NULL).%s\n", l->name);
	}
	printf("init: %d\ntype: %d\n", l->def, l->type);
	if (l->def == DEF)
		switch (l->type) {
			case K_INT:		printf("int: %d\n", l->val.val_int); break;
			case K_DOUBLE: 	printf("double: %f\n", l->val.val_double); break;
			case K_STRING:	printf("string: %s\n", l->val.val_string); break;
			case FUNC:		print_func(l); break;
		}
	else
		switch (l->type) {
			case K_INT:		printf("int: NULL\n"); break;
			case K_DOUBLE: 	printf("double: NULL\n"); break;
			case K_STRING:	printf("string: NULL\n"); break;
			//case FUNC:		print_func(l);
		}
		printf("- - - - - - - - - - - - - - -\n");
}

void print2(struct htab_listitem *l) 
{
	printf("		name: %s.%s\n", l->class, l->name);
	printf("		init: %d\n 		type: %d\n", l->def, l->type);
	if (l->def == DEF)
		switch (l->type) {
			case K_INT:		printf("		int: %d\n", l->val.val_int); break;
			case K_DOUBLE: 	printf("		double: %f\n", l->val.val_double); break;
			case K_STRING:	printf("		string: %s\n", l->val.val_string); break;
			case FUNC:		print_func(l); break;
		}
	else
		switch (l->type) {
			case K_INT:		printf("		int: NULL\n"); break;
			case K_DOUBLE: 	printf("		double: NULL\n"); break;
			case K_STRING:	printf("		string: NULL\n"); break;
		}
		printf("\n");
}

void print_func(struct htab_listitem *l)
{
	if (l->type == FUNC) {
		printf("	name: %s.%s\n", l->class, l->name);
		if (l->def == DEF && l->func != NULL) {
			printf("	return_type: %d\n", l->func->return_type);
			printf("	argc: %d\n", l->func->argc);
			printf("	lokalna ts:\n\n");
			for_each(l->func->local_t, LTAB_SIZE, print2);
		}
		//print args
	}
}
/*
int main() {
	htab_t *t = htab_init(4);
	struct arg_list *a = arg_init();
	char *i = "3";
	char *d = "4.5";
	htab_add(t, "Main",	"str", 	NDEF, K_STRING, NULL);
	htab_add(t, "b",	"dbl",	DEF, K_DOUBLE, d);
	htab_add(t, "b", 	"str",	DEF, K_STRING, "Hello World!");
	htab_add(t, "c", 	"int", 	DEF, K_INT, i);
	htab_add(t, "A", 	"F",	NDEF,K_INT, NULL);
	htab_add(t, "hmm", 	"g",	DEF, K_INT, "7");
	htab_add(t, "c", 	"func",	DEF, FUNC, NULL);
	htab_add(t, "c", 	"fnc", 	DEF, FUNC, NULL);

	arg_ctor(a,"Main","str",K_STRING);
	htab_add_func(t, "c", "func", K_INT, a);

	arg_ctor(a,"hulahop","xD",K_INT);
	//printf("%s\n", t->list[0]->name);
	arg_ctor(a, NULL,"go\0",K_STRING);
	arg_ctor(a,"hulahop","U",K_DOUBLE);
	htab_add_func(t, "c", "fnc", K_INT, a);



	for_each(t,4, print);
	htab_free(t, 4);
	args_free(a);
	return 0;
} */