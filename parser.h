

typedef enum Error
{
    E_OK,        	// 0 - přeloženo bez chyby
    E_LEX,       	// 1 - chyba v lexikální analýze
    E_SYN,       	// 2 - chyba v syntaktické analýze
    E_SEM_PROG,  	// 3 - chyba v sémantické analýze v programu
    E_SEM_TYPE,  	// 4 - chyba v sémantické analýze v typové kompabilitě 
    E_SEM_OTHERS,	// 6 - ostatní sémantické chyby
    E_RUN_INT,		// 7 - běhová chyba při načítaní číselné hodnoty ze vstupu	
    E_RUN_NO_ID,	// 8 - běhová chyba při práci s neinicializovanou proměnnou 
    E_RUN_ZERO_DIVIDE,//9 - běhová chyba dělení nulou
    E_RUN_OTHERS,	// 10 - ostatní běhové chyby
    E_INT,			// 99 - interní chyba interpretu
} tError;

int prog() 
int Class() 
int Class_next()
int Static()
int static_id()
int static_id_expr()
int static_func()
int list_params()
int list_params_next()
int body()
int stat()
int call_func()
int expr()

