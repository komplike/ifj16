/* ****************************************************************************
 * funkcie na pracu s tabulkou symbolov
 * autor: Michal Slavka
 * ***************************************************************************/
#ifndef HTAB_H
#define HTAB_H

#define FUNC		-1		//docasne
#define NOT_IN_TABLE -1		//docasne
#define ESEM		8		//docasne
#define DEF 		1		//docasne
#define NDEF 		0		//docasne
#define LTAB_SIZE	64
#define GTAB_SIZE	4

typedef struct htab {	//tabulka symbolov
	int size;
	struct htab_listitem *list[];
} htab_t;

typedef union Values {
	int val_int;		
	double val_double;
	char *val_string;
} values;

typedef struct arg_list {
	struct arg *first;
	struct arg *act;
	struct arg *last;
	int cnt;
} arg_l;

typedef struct arg {	//zoznam s argumentmy fonkcie
	int type;
	char *class;		
	char *name;
	struct arg *next;	
} arg_t;

typedef struct function {
	int return_type;		//return typ funkcie
	int argc;				//pocet argumentov
	int *args;				//pole typov argumentov funkcie
	int *inst_ptr;			//ukazatel na zaciatok funkcie v zozname instrukcii
	htab_t *local_t;		//ukazatel na lokalnu tabulku symbolov
} function_t;

struct htab_listitem {
  char *class;			//nazov premennej
  char *name;
  int def;				//je funkcia/premenna definovana
  int type;				//typ premennej popripade funkcia
  union {					
  	values val;				//hodnota premennej
  	struct function *func;	//dalsie informacie k funkcii
  };
  struct htab_listitem *next;	//ukazatel na dalsi prvok
} htab_listitem_t;

typedef void (*foreach_func)(struct htab_listitem *);

//hashovacia funkcia
unsigned int hash_function(const char *str, unsigned htab_size);
//inicializuje tabulku
struct htab *htab_init(int size);
//vyhlada ci sa nachadza v tabulke
struct htab_listitem *htab_lookup(htab_t *t, const char *class, const char *name);
//pridava prvok do tabulky
int htab_add(htab_t *htab, const char *class, const char *name, int def, int type, const char *value);
//vytlaci vsetky prvky (popripade prerobim tak aby sa tam dala predat funkcia)
int for_each(htab_t *t, int size, foreach_func func);
//vytlaci prvok
void print(struct htab_listitem *l);
//uvolnuje tabulku
void htab_free(struct htab *t, int size);
//**************************************************
//pridavanie funkcie do ts
int htab_add_func(htab_t *t, const char *class, const char *name, int return_type, arg_l *args);
//uvolnuje pomocne struktury v tabullke
void htab_func_free(struct htab_listitem *ptr, int size);

void print_func(struct htab_listitem *l);

void print2(struct htab_listitem *l);

//**************************************************
//praca s argumentmy
// uvolnenie zoznamu argumentov
void args_free(arg_l *a);
//vytvorenie pola s typmi
int *args_arr(arg_l *a, int argc);
//pridavanie argumentov do zoznamu argumentov
int arg_ctor(arg_l *a, const char *class, const char *name, int type);
//pridavanie argumento do lokalnej tabulky symbolov
int arg_add(htab_t *t, arg_l *a);

//**************************************************
//kopirovanie string retazcov
char *str_cpy(const char *str);
//konverzia string to int
int str_int(const char *i);
//konverzia string to double
double str_double(const char *d);

#endif