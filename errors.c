#include <stdio.h>

#include "error.h"

const char *err_table[] = {

	[LEX_ERR] = "Chyba v rogramu v ramci lexicalni analyzy.\n",
	[SYN_ERR] = "Chyba v ramci syntakticke analyzy\n",
	[UNDEF_ID] = "Semanticka chyba v programu\n",
	[INC_TYP] = "Semanticka chyba typove kompatibility\n",
	[SUCCESS] = "",
	[AUT_T_ERR] = "----\n",
	[SEM_ERR] = "Ostatnii semanticke chyby.\n",
	[RT_NUM_IN] = "Behova chyba pri nacitani ciselne hodnoty ze vstupu.\n",
	[RT_UNINIT] = "Behova chyba pri praci s neinicializovanou promennou.\n",
	[RT_DIV_BZ] =  "Behova chyba pri deleni nulou.\n",
	[RT_OTHER] = "Ostatni behove chyby.\n",
	[INT_ERR] = "Interni chyba interpretu.\n",
};


inline unsigned int printError(unsigned int ret_msg_id)
{
	fprintf(stderr, "%s", err_table[ret_msg_id]);
	return ret_msg_id;
}
