
#include <parser.h>
#include <leh.c>
#include <leh.h>

tSymbolTable *table;

tError parser(){
	GetNextToken(FILE *f, int *type, char **content);

	if(*type == LEX_EOF)
		error = E_LEX;
	else
	 	error = program();

	 return error;
}

/*
** <PROG> ->  <CLASS>
*/
tError program(){

	if(error != E_OK)
		return error;

	error = Class()

	if(error != E_OK)
		return error;

	return error;		
}



/*
** <CLASS> ->	class Main { <STATIC> static void run() { <BODY> } <STATIC>} 
** <CLASS> ->	class id { <STATIC> <BODY>} <CLASS_NEXT>
*/	
tError Class(){

	if(error != E_OK)
		return error;

	switch(*type)
	{
		case K_CLASS:

			GetNextToken(FILE *f, int *type, char **content);	
			switch(*type)
			{
				case L_SIMPLE:
					switch(*content)
					{
						case "Main": 

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_LEFT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = Static() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = body() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != K_STATIC)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != K_VOID)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(!strcmp(*content, "run"))
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_ROUND_LEFT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_ROUND_RIGHT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_LEFT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = body() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_RIGHT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = Static() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_RIGHT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = Class_next() != E_OK)
								return error;

							break;	

						default:
							
							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_LEFT)
								return E_SYN;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = Static() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(error = body() != E_OK)
								return error;

							GetNextToken(FILE *f, int *type, char **content);

							if(*type != B_WAVE_RIGHT)
								return E_SYN;

					}
			default: error = E_SYN;
			}
	default: error = E_SYN;		
	}

	return error;
}

/*
** <CLASS_NEXT> -> 		eps
** <CLASS_NEXT>	-> 		<CLASS> 
*/
tError Class_next(){
	if( error != E_OK)
		return error;

	if( error=Class() != E_OK)
		return error;
	
	else
		return E_OK;

	return error;
}

/*
** <STATIC>	->		<STATIC_ID>
** <STATIC>	->		<STATIC_FUNC>
*/
tError Static(){
	if( error != E_OK)
		return error;

	if( error = static_id() != E_OK)
		return error;

	if( error = static_func() != E_OK)
		return error;
	
	return error;
}


/*
** <STATIC_ID> -> 		static id <STATIC_ID_EXP>;
*/
tError static_id(){
	if(error != E_OK)
		return error;

	if( *type == K_STATIC){
		GetNextToken(FILE *f, int *type, char **content);

		if(*type == K_INT || *type == K_DOUBLE || *type == K_STRING){
			int tmp = *type;
			GetNextToken(FILE *f, int *type, char **content);
		}
		else
			return E_SYN;
		
		if(*type != L_SIMPLE)
			return E_SYN;
		//zavolat tabulku a vložit content s typem tmp

		GetNextToken(FILE *f, int *type, char **content);
		if(error = static_id_expr() != E_OK)
			return error;

		GetNextToken(FILE *f, int *type, char **content);
		if(*type != O_SEMI)
			return E_SYN;

	return error;
}

/*
** <STATIC_ID_EXPR> -> 	 	eps
** <STATIC_ID_EXPR> -> 	 	= value
*/
tError static_id_expr(){
	if(error != E_OK)
		return error;

	if( *type != SIGN_ASSIGN)
		return E_SYN;

	GetNextToken(FILE *f, int *type, char **content);
	if(*type !=  L_INT && *type != L_DOUBLE && *type != L_STRING)
		return E_SYN;

	//poslat do tabulky symbolů
	return error;
}


/*
** <STATIC_FUNC> -> 	static id (<LIST_PARAMS>) { <BODY> }
*/
tError static_func(){
	if( error != E_OK)
		return error;

	switch(*type)
	{
		case K_STATIC:

			GetNextToken(FILE *f, int *type, char **content);

			if(*type == K_INT || *type == K_DOUBLE || *type == K_STRING || *type == K_VOID){
				int tmp = *type;
				GetNextToken(FILE *f, int *type, char **content);
			}
			else
				return E_SYN;

			if(*type != L_SIMPLE)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = list_params() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_RIGHT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = body() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_RIGHT)
				return E_SYN;

			break;

	default: return E_SYN; //pokud přišlo cokoliv jiného
	}

	return error;
}


/*
** <LIST_PARAMS> -> 	eps
** <LIST_PARAMS> -> 	id <LIST_PARAMS_NEXT>
*/
tError list_params(){
	if(error != E_OK)
		return error;

	if(*type == B_ROUND_RIGHT) //pokud se token rovná ')' není žádný parametr
		return error;

	if(*type == K_INT || *type == K_DOUBLE || *type == K_STRING){
		int tmp = *type;
		GetNextToken(FILE *f, int *type, char **content);
	}
	else
		return E_SYN;

	if(*type == L_SIMPLE){ 	//pokud se jedná o identifikator
		GetNextToken(FILE *f, int *type, char **content);
		if(error = list_params_next() != E_OK)
			return error;
	}

	return error;
}


/*
** <LIST_PARAMS_NEXT> -> 	eps
** <LIST_PARAMS_NEXT> -> 	, id <LIST_PARAMS_NEXT>
*/
tError list_params_next(){
	if(error != E_OK)
		return error;

	if(*type == B_ROUND_RIGHT) //pokud se token rovná ')' není žádný parametr
		return error;

	if(*content != ',')
		return E_SYN;

	if(*type == K_INT || *type == K_DOUBLE || *type == K_STRING){
		int tmp = *type;
		GetNextToken(FILE *f, int *type, char **content);
	}
	else
		return E_SYN;

	if(*type == L_SIMPLE){ 	//pokud se jedná o identifikator
		GetNextToken(FILE *f, int *type, char **content);
		if(error = list_params_next() != E_OK)
			return error;
	}

	return error;
}

/*
** <BODY> -> 	eps
** <BODY> -> 	<STAT>; <BODY>
*/
tError body(){
	if(error != E_OK)
		return error;

	if(*type == B_WAVE_RIGHT)	// ukončující znak pro BODY
		return error;

	if(error = stat() != E_OK)
		return error;

	if(*type != O_SEMI)
		return E_SYN;

	if(error = body() != E_OK)
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
	if(error != E_OK)
		return error;

	if(*type == K_INT || *type == K_DOUBLE || *type == K_STRING){
		int tmp = *type;
		GetNextToken(FILE *f, int *type, char **content);
	}
	else
		return E_SYN;

	switch(*type)
	{
		case K_IF: // <STAT> ->	if (<EXPR>) {<STAT>; <BODY>} else {<BODY>}

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = expresion() != E_OK) // provede se analýza výrazů
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_RIGHT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = stat() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != O_SEMI)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = body() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_RIGHT)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != K_ELSE)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);


			if(error = body() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_RIGHT)
				return error;

			break;

		case K_WHILE: // <STAT> ->	while (<EXPR>) {<STAT>; <BODY>}

			GetNextToken(FILE *f, int *type, char **content);

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = expresion() != E_OK) // provede se analýza výrazů
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_ROUND_RIGHT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_LEFT)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = stat() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != O_SEMI)
				return E_SYN;

			GetNextToken(FILE *f, int *type, char **content);

			if(error = body() != E_OK)
				return error;

			GetNextToken(FILE *f, int *type, char **content);

			if(*type != B_WAVE_RIGHT)
				return error;

			break;

		case K_RETURN:  //<STAT>	return <EXPR>

			GetNextToken(FILE *f, int *type, char **content);

			if(error = expresion() != E_OK)
				return error;

			break;

		case L_SIMPLE: //<STAT>		id <PRIRAZENI>

			GetNextToken(FILE *f, int *type, char **content);

			if(error = prirazeni() != E_OK)
				return error;

			break;

	default: return E_SYN;
	}

	// 	<STAT> -> <CALL_FUNC>
	if(error = call_func() != E_OK)
		return error;

	if(error != E_OK)
		return error;

	return error;
}

/*
** <PRIRAZENI> ->	eps
** <PRIRAZENI> ->	= <EXPR>
** <PRIRAZENI> ->  	= <CALL_FUNC>
*/
tError prirazeni(){
	if(error != E_OK)
		return error;

	if(*type == O_SEMI) // <PRIRAZENI> ->EPS
		return error;

	if(*type == SIGN_ASSIGN){

		GetNextToken(FILE *f, int *type, char **content);

		if(error = expresion() != E_OK)
			return error;
		else if(error = call_func() != E_OK)
			return error;
	}

	if(error != E_OK)
		return error;

	return error;
}

/*
** <CALL_FUNC> -> 	id (<ARGS>)
*/
tError call_func(){
	if(error != E_OK)
		return error;

	int clas_name = *type; //uložení id pro případ kdyby se jednalo o třídu nebo vestavěnou funkci
	if(strcmp(*content, "ifj16")){ //jedná se o vestavěnou funkci
		GetNextToken(FILE *f, int *type, char **content);
		if(*type == O_DOT) { //očekává se '.'
			GetNextToken(FILE *f, int *type, char **content);
			if(*type == L_SIMPLE)
				//uložení id do tabulky symbolů
			else
				return E_SYN;
		}
		else
			return E_SYN;
	}
	else if(*type == L_SIMPLE){
		//kouknout se do tabulky jestli se nejedná o třídu
		GetNextToken(FILE *f, int *type, char **content);
		if(*type == O_DOT) { //očekává se '.'
			GetNextToken(FILE *f, int *type, char **content);
			if(*type == L_SIMPLE)
				//uložení id do TS
			else
				return E_SYN;
		}
		else if(*type == B_ROUND_LEFT) //jedná se o rekurzivní funkci
			//uložení funkce do TS;
		else
			return E_SYN;
	}

	GetNextToken(FILE *f, int *type, char **content);

	if(*type != B_ROUND_LEFT)
		return E_SYN;

	GetNextToken(FILE *f, int *type, char **content);

	if(error = args() != E_OK)
		return error;

	GetNextToken(FILE *f, int *type, char **content);

	if(*type != B_ROUND_RIGHT)
		return E_SYN;

	if(error != E_OK)
		return error;

	return error;	
}

/*
** <ARGS> ->	<LIT> <ARGS_NEXT>
** <ARGS> ->	eps
*/
tError args(){
	if(error != E_OK)
		return error;

	if(*type == B_ROUND_RIGHT) //pokud se token rovná ')' není žádný parametr
		return error;

	GetNextToken(FILE *f, int *type, char **content);

	if(error = lit() != E_OK)
		return error;

	GetNextToken(FILE *f, int *type, char **content);

	if(error = args_next() != E_OK)
		return error;

	return error;
}

/*
** <LIT> ->		string
** <LIT> ->		id
** <LIT> ->		number
*/
tError lit(){
	if(error != E_OK)
		return error;

	//..
}

/*
** <ARGS_NEXT> -> eps
** <ARGS_NEXT> -> , <LIT> <ARGS_NEXT>
*/
tError args_next(){
	if(error != E_OK)
		return error;

	if(*type == B_ROUND_RIGHT) //pokud se token rovná ')' není žádný parametr
		return error;

	if(*content != ',')
		return E_SYN;

	GetNextToken(FILE *f, int *type, char **content);

	if(error = lit() != E_OK)
		return error;

	GetNextToken(FILE *f, int *type, char **content);

	if(error = args_next() != E_OK)
		return error;

	return error;
}
