
#include "parser.h"
// #include "lex.c"	
#include "lex.h"
#include "htab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//struct htab *t;
//FILE *f;

bool prt = true;
bool token_prt = true; 
bool htab_prt = true;

bool sem = false; // musi byt false!!

void GetNextToken(){
	if (error != E_OK){
		return;
	}
	getToken(f, &type, &content);
	if (type == LEX_ERR)
		error = E_LEX;
	char c = '0';
	if (type < 2000 && type != L_SIMPLE) {
		if (token_prt){
			printf(">>typ: %d	cont: %s\n", type, content);
		}
	}
	else {
		switch (type) {
			case SIGN_ADD: c = '+'; break;   // +
			case SIGN_SUB: c = '-'; break;      // -
 			case SIGN_MUL: c = '*'; break;     // *
 			case SIGN_DIV: c = '/'; break;       // /
 			case SIGN_ASSIGN: c= '='; break;    // =
	 		case SIGN_EQ: c = 'c'; break;      // ==
 			case SIGN_NEQ: c = '!'; break;       // !=
 			case SIGN_LESS: c = '<'; break;      // <
 			case SIGN_MORE: c = '>'; break;      // >
 			case SIGN_LOE:  c = 's'; break;     // <=
 			case SIGN_MOE:  c= 'b'; break;      // >=

 			case B_ROUND_LEFT: c = '('; break;       // (
 			case B_ROUND_RIGHT: c = ')'; break;     // )
 			case B_WAVE_LEFT: c= '{';break;        // {
 			case B_WAVE_RIGHT: c = '}'; break;       // }

 			case L_SIMPLE: c = 'L'; break;       // literal
 			case L_INT: c = 'I'; break;          // literal typu int
 			case L_DOUBLE: c = 'D'; break;       // literal typu double
 			case L_STRING: c = 'S'; break;       // literal typu string

 			case O_SEMI: c = ';'; break;         // ;
 			case O_DOT: c = '.'; break;          // .

 			case LEX_EOF: c = '1'; break;           // konec radku
 			case LEX_ERR: c = '2'; break;    	// chyba v souboru
 			case LEX_RUN_ERR: c = '3'; break;	// chyba v automatu
		}
		if (token_prt){	
			printf(">>typ: %c	cont: %s\n", c, content);
		}
	}
}

tError parser(){

	f = fopen("test.txt", "r");
	arg = arg_init();
	t = htab_init(TAB_SIZE);
	GetNextToken(); 
	if (error != E_OK) 
		return error;

	if(type == LEX_EOF)
		error = E_LEX;
	else
	 	error = program();

	if (error != E_OK){
		printf("\n\n");
		for_each(t, TAB_SIZE, print);
		return error;
	}
	if (htab_prt) {
		printf("\n\n");
		for_each(t, TAB_SIZE, print);
	}
	fclose(f);
	for_each(t, TAB_SIZE, check1);
	if (error != E_OK)
		return error;
	if (prt) printf("***********************************semantika************************************\n");

	f = fopen("test.txt", "r");
	sem = true;
	GetNextToken(); 
	if (error != E_OK) 
		return error;

	if(type == LEX_EOF)
		error = E_LEX;
	else
	 	error = program();
	if (htab_prt) {
		printf("\n\n");
		for_each(t, TAB_SIZE, print);
	}
	htab_free(t, TAB_SIZE);
	args_free(arg);
	free(content);
	content = NULL;

	return error;
}

/*
** <PROG> ->  <CLASS>
*/
tError program(){
if (prt) printf("__program__\n");
	if(error != E_OK)
		return error;

	error = Class();

	if(error != E_OK)
		return error;

	return error;		
}


/*
** <CLASS>	->  class id { <STATIC> <STATIC_NEXT>}
*/
tError Class(){
if (prt) printf("__Class__\n");
	if(error != E_OK)
		return error;	

	if(type != K_CLASS) 
		return E_SYN;

	{GetNextToken(); if (error != E_OK) return error;}
	if (type != L_SIMPLE)
		return E_SYN;

	class = str_cpy(content); 

	if (!sem && search_for_class(t, class, TAB_SIZE) == 1){
		printf("trieda %s je v programe duplicitne\n", class);
		free(class);
		class = NULL;
		return E_SEM_PROG;
	}

	{GetNextToken(); if (error != E_OK) return error;}
	if (type != B_WAVE_LEFT) {
		free(class);
		class = NULL;
		return E_SYN;
	}

	{GetNextToken(); if (error != E_OK) return error;}
	if((error = Static()) != E_OK){
		free(class);
		class = NULL;		
		return error;
	}

	{GetNextToken(); if (error != E_OK) return error;}
	if((error = static_next()) != E_OK){
		free(class);
		class = NULL;		
		return error;
	}

	//{GetNextToken(); if (error != E_OK) return error;}
	if (type != B_WAVE_RIGHT)
		return E_SYN;
	free(class);
	class = NULL;

	{GetNextToken(); if (error != E_OK) return error;}

	if((error = Class_next()) != E_OK)
		return error;

}


/*
** <CLASS_NEXT> -> 		eps
** <CLASS_NEXT>	-> 		<CLASS> 
*/

tError Class_next(){
	if( error != E_OK)
		return error;

	if(type == K_CLASS){
		if((error = Class()) != E_OK)
			return error;
	}
	else
		return error;
	
	return error;
}

/*
** <STATIC_NEXT> -> EPS
** <STATIC_NEXT> -> <STATIC>
*/
tError static_next(){
if (prt) printf("__static_next__\n");
	if(error != E_OK)
		return error;
	if(type == B_WAVE_RIGHT){
		return error;
	}
	if((error = Static()) != E_OK)
		return error;

	{GetNextToken(); if (error != E_OK) return error;}

	if(type == K_STATIC){
		if((error = static_next()) != E_OK)
			return error;
	}

	return error;
}

/*
** <STATIC>	->		id <STATIC_ID>
** <STATIC>	->		<STATIC_FUNC>
*/
tError Static(){
if (prt) printf("__Static__\n");
	if( error != E_OK)
		return error;

	if(type != K_STATIC)
		return E_SYN;

	{GetNextToken(); if (error != E_OK) return error;}

	if (type != K_VOID  && type != K_INT && type != K_DOUBLE && type != K_DOUBLE) {
		return E_SYN;
	}

	tmp_type = type;
	
	{GetNextToken(); if (error != E_OK) return error;}

	if (type != L_SIMPLE) {
		return E_SYN;
	}

	func_name = str_cpy(content);

	{GetNextToken(); if (error != E_OK) return error;}

	if (type == B_ROUND_LEFT) {
		if( (error = static_func()) != E_OK) {
			free(func_name);
			func_name = NULL;
			return error;
		}
	}
	else if (tmp_type != K_VOID) {
		if( (error = static_id()) != E_OK){
			free(func_name);
			func_name = NULL;
			return error;
		}
	}
	else {
		error = E_SYN;
	}
	free(func_name);
	func_name = NULL;
	return error;
}


/*
** <STATIC_ID> -> 	 <STATIC_ID_EXP>
*/
tError static_id(){
if (prt) printf("__static_id__\n");
	if(error != E_OK)
		return error;
	//zavolat tabulku a vložit content s typem tmp
	if (!sem){
		if (htab_lookup(t,class,func_name) == NULL) {
			if ((error = htab_add(t, class, func_name, NDEF, DECLR, tmp_type)) != E_OK)
				return error;
		}
		else { 
			if ((error = htab_comp(t, class, func_name, tmp_type)) != E_OK) {
				return error;
		}	}
	}

	if((error = static_id_expr()) != E_OK)
		return error;

	return error;
}

/*
** <STATIC_ID_EXPR> -> 	 	eps
** <STATIC_ID_EXPR> -> 	 	= value;
*/
tError static_id_expr(){
if (prt) printf("__static_id_expr__\n");
	if(error != E_OK)
		return error;

	if(type == O_SEMI)
		return error;

	if( type != SIGN_ASSIGN)
		return E_SYN;

	{GetNextToken(); if (error != E_OK) return error;}
	//if(type !=  L_INT && type != L_DOUBLE && type != L_STRING)
	//zmena na expressions(); 	
	if( type< L_INT && type > L_STRING)
		return E_SYN;
	
	if (!sem)
		if ((error = htab_add_val(t, class, func_name, content)) != E_OK)
			return error;
	//todo co ak sa static id = func?
	GetNextToken(); 
	if (error != E_OK) 
		return error;


	if(type != O_SEMI)
		return E_SYN;

	//poslat do tabulky symbolů
	return error;
}


/*
** <STATIC_FUNC> -> 	static id (<LIST_PARAMS>) { <BODY> }
*/
tError static_func(){
if (prt) printf("__static_func__\n");
	if( error != E_OK)
		return error;

	{GetNextToken(); if (error != E_OK) return error;}

	if (!sem)
		if ((error = htab_add(t,class,func_name, DEF, DECLR, FUNC)) != E_OK)
			return error;

	if((error = list_params()) != E_OK)
		return error;

	if (!sem)
		if ((error = htab_add_func(t, class, func_name, tmp_type, arg)) != E_OK)
			return error;

	//if(type != B_ROUND_RIGHT)
	//	return E_SYN
	{GetNextToken(); if (error != E_OK) return error;}

	if(type != B_WAVE_LEFT) {
		return E_SYN;
	}
	GetNextToken(); 
	if (error != E_OK) 
		return error;

	if((error = body()) != E_OK) {
		return error;
	}
	//{GetNextToken(); if (error != E_OK) return error;}
	if(type != B_WAVE_RIGHT)
		return E_SYN;

	return error;
}


/*
** <LIST_PARAMS> -> 	eps
** <LIST_PARAMS> -> 	id <LIST_PARAMS_NEXT>
*/
tError list_params(){
if (prt) printf("__list_params__\n");
	if(error != E_OK)
		return error;
	if(type == B_ROUND_RIGHT){ //pokud se token rovná ')' není žádný parametr

		return error;
	}


	if(type != K_INT && type != K_DOUBLE && type != K_STRING){
		return E_SYN;
	}
	int tmp = type;
	{GetNextToken(); if (error != E_OK) return error;}

	if(type == L_SIMPLE){ 	//pokud se jedná o identifikator
		param_ctor(arg, NULL, content, tmp);	
		{GetNextToken(); if (error != E_OK) return error;}
		if((error = list_params_next()) != E_OK)
			return error;
	}
	return error;
}


/*
** <LIST_PARAMS_NEXT> -> 	eps
** <LIST_PARAMS_NEXT> -> 	, id <LIST_PARAMS_NEXT>
*/
tError list_params_next(){
if (prt) printf("__list_params_next__\n");
	if(error != E_OK)
		return error;

	if(type == B_ROUND_RIGHT) //pokud se token rovná ')' není žádný parametr
		return error;

	if(type != O_COMMA)
		return E_SYN;

	{GetNextToken(); if (error != E_OK) return error;}

	if(type != K_INT && type != K_DOUBLE && type != K_STRING){
		return E_SYN;
	}

	int tmp = type;
	{GetNextToken(); if (error != E_OK) return error;}

	if(type == L_SIMPLE){ 	//pokud se jedná o identifikator
		param_ctor(arg, NULL, content, tmp);	
		{GetNextToken(); if (error != E_OK) return error;}
		if((error = list_params_next()) != E_OK)
			return error;
	}
	else 
		return E_SYN;

	return error;
}

/*
** <BODY> -> 	eps
** <BODY> -> 	<STAT>; <BODY>
*/
tError body(){
if (prt) printf("__body__\n");
	if(error != E_OK)
		return error;

	if(type == B_WAVE_RIGHT) { // ukončující znak pro BODY
		return error;
	}

	if((error = stat()) != E_OK){
		return error;
	}

	if(type == B_ROUND_RIGHT) {
		{GetNextToken(); if (error != E_OK) return error;} 
		if (error != E_OK) return error;
	}

	if(type != O_SEMI && type != B_WAVE_RIGHT) {
		return E_SYN;
	}

	{GetNextToken(); if (error != E_OK) return error;}

	if(type == K_CLASS){
		if((error = Class_next()) == E_OK)
			return error;
	}
	
	if((error = body()) != E_OK)
		return error;

	return error;
}

/*
** <STAT> ->	if (<EXPR>) {<STAT>; <BODY>} else {<BODY>}
** <STAT> ->	while (<EXPR>) {<STAT>; <BODY>}
** <STAT> ->	return <EXPR>
** <STAT> ->	<CALL_FUNC>
** <STAT> ->	id <PRIRAZENI>
*/
tError stat(){
if (prt) printf("__stat__\n");
	if(error != E_OK)
		return error;
	char *str1 = NULL;
	char *str2 = NULL;
	struct htab_listitem *item;
	struct htab_listitem *item2;
	int tmp = -1; int i = -1;
	if(type == K_INT || type == K_DOUBLE || type == K_STRING){
		tmp = type;
		{GetNextToken(); if (error != E_OK) return error;}
	}
	else if(strcmp(content, "ifj16") == 0){
		{GetNextToken(); if (error != E_OK) return error;}
		if((error = call_func("ifj16", content)) == E_OK){
			GetNextToken(); 
			if (error != E_OK) 
				return error;
			return error;
		}
		else
			return error;
	}

	switch(type)
	{
		case K_IF: // <STAT> ->	if (<EXPR>) {<STAT>; <BODY>} else {<BODY>}
			if (tmp != -1)
				return E_SYN;
			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_ROUND_LEFT)
				return E_SYN;

			if((error = expression(K_BOOLEAN, false)) != E_OK) // provede se analýza výrazů
				return error;

			if(type != B_ROUND_RIGHT)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_WAVE_LEFT)	//??? co ak ide iba jeden prikaz za if
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = stat()) != E_OK)
				return error;

			//{GetNextToken(); if (error != E_OK) return error;}

			if(type != O_SEMI)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = body()) != E_OK)
				return error;
			//{GetNextToken(); if (error != E_OK) return error;}
			// kontrolovane 2 krat (aj v body)
			if(type != B_WAVE_RIGHT)
				return error;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != K_ELSE)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_WAVE_LEFT)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = body()) != E_OK)
				return error;
			// kontrolovane uz v body!!
			if(type != B_WAVE_RIGHT)
				return error;
			break;

		case K_WHILE: // <STAT> ->	while (<EXPR>) {<STAT>; <BODY>}
			if (tmp != -1)
				return E_SYN;
			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_ROUND_LEFT)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = expression(K_BOOLEAN, false)) != E_OK) // provede se analýza výrazů
				return error;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_ROUND_RIGHT)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_WAVE_LEFT)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = stat()) != E_OK)
				return error;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != O_SEMI)
				return E_SYN;

			{GetNextToken(); if (error != E_OK) return error;}

			if((error = body()) != E_OK)
				return error;

			{GetNextToken(); if (error != E_OK) return error;}

			if(type != B_WAVE_RIGHT)
				return error;

			break;

		case K_RETURN:  //<STAT>	return <EXPR>
			if (tmp != -1)
				return E_SYN;

			GetNextToken(); 
			if (error != E_OK) 
				return error;
			item = htab_lookup(t, class, func_name);
			if((error = expression(item->func->return_type, true)) != E_OK)
				return error;

			break;

		case L_SIMPLE: //<STAT>		id <PRIRAZENI>
			item = htab_lookup(t, class, func_name);


			str1 = str_cpy(content); 
			
			GetNextToken(); 
			if (error != E_OK) 
				return error;

			if (tmp == -1) {
				if (type == B_ROUND_LEFT) {
					error = call_func(class , str1);
					if (error != E_OK) {
						return error;
					}

					GetNextToken(); 
					free(str1);
					str1 = NULL;
					return error;
				}
				else if (type == O_DOT) {
					GetNextToken(); 
					if (error != E_OK) 
						return error;
					

					if (type != L_SIMPLE){
						free(str1);
						str1 = NULL;
						return E_SYN;
					}
					str2 = str_cpy(content);	//str1.str2 trieda.meno

					GetNextToken(); 
					if (error != E_OK) {
						free(str1);
						free(str2);
						str1 = NULL;
						str2 = NULL;					
						return error;
					}

					if (type == B_ROUND_LEFT) {
						error = call_func(str2, str1);
						if (error != E_OK)
							return error;
						GetNextToken();
						free(str1);
						free(str2);
						str1 = NULL;
						str2 = NULL;
						return error;
					}
				}

				if (str2 == NULL){
					if (!sem){
						if ((item2 = htab_lookup(item->func->local_t, NULL, str1)) == NULL)
							if ((item2 = htab_lookup(t, class, str1)) == NULL)
								error = htab_add(t, class, str1, NDEF, NDECLR, tmp);
						if (error != E_OK)
							return error;
					}
					if (sem) {
						if ((htab_lookup(item->func->local_t, NULL, str1) == NULL) && (htab_lookup(t, class, str1) == NULL)) {
							printf("premenna sa nenachadza v ts\n");
							return error = E_SEM_PROG;
						}
					}
				}
				else if (sem && (item2 = htab_lookup(t, str1, str2)) == NULL){
					printf("premenna sa nenachadza v ts\n");
					return error = E_SEM_PROG;
				}

				if (type == SIGN_ASSIGN) {
					if (!sem){
						item2 = htab_lookup(t, class, str1);
						if (item2 != NULL){
							item2->def = DEF;
						}
					}
					if (sem){
						if (str2 == NULL) {
							item2 = htab_lookup(t, class, func_name);
							if (item2 == NULL){
								item2 = htab_lookup(item2->func->local_t, NULL, str1);
							}
							// 	return 99;
							if (item2 == NULL)
								return E_SEM_PROG;
							i = item2->type;
						}
					}

					if((error = prirazeni(i)) != E_OK) {
						free(str1);
						str1 = NULL;
						if (str2 != NULL){
							free(str2);
							str2 = NULL;
						}
						return error;
					}
				}
				if (type == O_SEMI){
					free(str1);
					free(str2);
					str1 = NULL;
					str2 = NULL;
				}
				else 
					error = E_SYN;
			}
			else {	//typ id..
				if (type == SIGN_ASSIGN) {
					if (sem) {
						item2 = htab_lookup(item->func->local_t, NULL, str1);
						if (item2 != NULL && item2->declr == NDECLR){
							printf("nedeklarovana premenna\n");
							return error = E_SEM_PROG;
						}
						item2->def = DEF;
					}

					if (!sem) {
						error = htab_add(item->func->local_t, NULL, str1, DEF, DECLR, tmp);
					}
					if (error != E_OK){
						free(str1);
						str1 = NULL;
						return error;
					}
					if ((error = prirazeni(tmp)) != E_OK){
						free(str1);
						str1 = NULL;
						return error;
					}
					return error;
				}
				if (type == O_SEMI){
					if (!sem) {
						item2 = htab_lookup(t, class, str1);
						if (item2 != NULL && item2->declr == NDECLR){
							printf("nedeklarovana premenna\n");
							return error = E_SEM_PROG;
						}
						if (item2 != NULL)
							item2->def = DEF;
					}					
					if (!sem) error = htab_add(item->func->local_t, NULL, str1, DEF, DECLR, tmp);
					free(str1);
					str1 = NULL;
					return error;
				}
			}

			
			break;

	default: return E_SYN;
	}

	return error;
}

/*
** <PRIRAZENI> ->	eps
** <PRIRAZENI> ->	= <EXPR>
** <PRIRAZENI> ->  	= <CALL_FUNC>
*/
tError prirazeni(int i){
if (prt) printf("__prirazeni__\n");
	if(error != E_OK)
		return error;

	if(type == O_SEMI){ // <PRIRAZENI> ->EPS
		return error;
	}

	if(type == SIGN_ASSIGN){

		{GetNextToken(); if (error != E_OK) return error;}
		error = expression(i, true);
			return error;
	}

	// return error;
}

/*
** <CALL_FUNC> -> 	id (<ARGS>)
*/
tError call_func(const char *str1, const char *str2){
if (prt) printf("__call_func__\n");
	if(error != E_OK)
		return error;
	struct htab_listitem *item = NULL;
	
	if (sem){
		if (str1 == NULL){
			item = htab_lookup(t, class, str2);
			if (item == NULL){
				printf("volana funkcia neexistuje\n");
				return E_SEM_PROG;
			}
		}
		else {
			item = htab_lookup(t, str1, str2);
			if (item == NULL){
				printf("volana funkcia neexistuje\n");
				return E_SEM_PROG;
			}
		}			

		if (item == NULL){
			printf("volana funkcia %s.%s nebola deklarovana\n", str1, str2);
			return error = E_SEM_PROG;
		}

		if (item->type != FUNC && item->def != DEF && item->declr != DECLR){
			printf("volana funkcia %s.%s nebola deklarovana\n", str1, str2);
			return error = E_SEM_PROG;
		}
	}

	if(type == B_ROUND_LEFT){//vím že se jedná o volání rekurzivní funkce
		{GetNextToken(); if (error != E_OK) return error;}
		if((error = args(htab_lookup(t, class, func_name))) != E_OK) {
			return error;
		}
	}

	if (sem) {
		error = arg_check(arg, item);
	}

	return error;	
}

/*
** <ARGS> ->	<LIT> <ARGS_NEXT>
** <ARGS> ->	eps
*/
tError args(struct htab_listitem *func){
if (prt) printf("__args__\n");
	if(error != E_OK)	
		return error;
	char *str = NULL;

	if(type == B_ROUND_RIGHT){ //pokud se token rovná ')' není žádný parametr
		return error;
	}

	if((error = lit()) != E_OK){
		return error;
	}

	if (type == L_SIMPLE){
		if (sem) str = str_cpy(content);

		GetNextToken(); 
		if (error != E_OK) 
			return error;

		if (type == O_DOT){
			GetNextToken(); 
			if (error != E_OK) 
				return error;

			if ((error = lit()) != E_OK){
				free(str);
				str = NULL;
				return error;
			}
			if (type != L_SIMPLE){
				free(str);
				str = NULL;
				return E_SYN;
			}

			if (sem) {
				error = arg_ctor(t, func->func->local_t, arg, str, content, L_SIMPLE);
			}
			if (error != E_OK){
				free(str);
				return error;
			}
		}
		else {
			if (sem) {
				error = arg_ctor(t, func->func->local_t, arg, class, str, L_SIMPLE);
			}
			if (error != E_OK){
				free(str);
				return error;
			}
		}	
	}
	else {
		if (sem) error = arg_ctor(t, func->func->local_t, arg, "#", content, type);
		if (error != E_OK)
			return error;
		GetNextToken(); 
		if (error != E_OK) 
			return error;
	}
	free(str);
	str = NULL;

	if (type == B_ROUND_RIGHT){
		free(str);
		return error;
	}

	if (type != O_COMMA)
		return E_SYN;

	GetNextToken(); 
	if (error != E_OK) 
		return error;


	if (type == B_ROUND_RIGHT)
		return E_SYN;
	error = args_next(func);
	return error;
}

/*
** <LIT> ->		string
** <LIT> ->		id
** <LIT> ->		number
*/
tError lit(){
 if (prt) printf("__lit__\n");

	if(error != E_OK)
		return error;

	error = E_SYN;
	if(type == L_STRING){ //pokud se jedná o řetězec
		error = E_OK;
	}

	if(type == L_DOUBLE){ //pokud se jedná o double
		error = E_OK;
	}

	if(type == L_INT){ // pokud se jedná o int
		error = E_OK;
	}

	if(type == L_SIMPLE){ // pokud se jedná o id
		error = E_OK;
	}

	return error;
}

/*
** <ARGS_NEXT> -> eps
** <ARGS_NEXT> -> , <LIT> <ARGS_NEXT>
*/
tError args_next(struct htab_listitem *func){
if (prt) printf("__args_next__\n");
	if(error != E_OK)	
		return error;
	char *str = NULL;

	if((error = lit()) != E_OK){
		return error;
	}

	if (type == L_SIMPLE){
		if (sem) str = str_cpy(content);
		GetNextToken();
		if (error != E_OK) 
			return error;

		if (type == O_DOT){
			GetNextToken(); 
			if (error != E_OK) 
				return error;

			if ((error = lit()) != E_OK){
				free(str);
				str = NULL;
				return error;
			}
			if (type != L_SIMPLE){
				free(str);
				str = NULL;
				return E_SYN;
			}

			if (sem) {
			 	error = arg_ctor(t, func->func->local_t, arg, str, content, L_SIMPLE);
			}
			if (error != E_OK){
				free(str);
				return error;
			}
		}
		else {
			if (sem) {
				error = arg_ctor(t, func->func->local_t, arg, class, str, L_SIMPLE);
			}
			if (error != E_OK){
				free(str);
				return error;
			}
		}	
	}
	else {
		if (sem) error = arg_ctor(t, func->func->local_t, arg, NULL, content, type);
		if (error != E_OK)
			return error;
		GetNextToken(); 
		if (error != E_OK) 
			return error;
	}
	free(str);
	str = NULL;

	if (type == B_ROUND_RIGHT){
		free(str);
		return error;
	}

	if (type != O_COMMA)
		return E_SYN;

	GetNextToken(); 
	if (error != E_OK) 
		return error;


	if (type == B_ROUND_RIGHT)
		return E_SYN;

	error = args_next(func);
	return error;
}


tError expression(int i, bool semi){
	if (prt) printf("___expression___\n");
	if (!true) {;//(sem)
		//funkcia pre kontrolu vyrazov
	}
	else {
	    int br_cnt = 0;
	    if (type == B_ROUND_LEFT){
	        br_cnt++;
	        {GetNextToken(); if (error != E_OK) return error;}
	    }

	    while (type != O_SEMI){
	        if (type == B_ROUND_LEFT)
	            br_cnt++;
	        else if (type == B_ROUND_RIGHT) {
	            br_cnt--;
	            if (br_cnt == 0 && !semi)
	        		break;
	        }
	        else if (type < 2000 || type > 6000) {
	            return E_SYN;
	        }

	        {GetNextToken(); if (error != E_OK) return error;}
	    }
	}
    return E_OK;
 }


int main(){
	printf("\n%d\n\n",parser());

	return 0;
}