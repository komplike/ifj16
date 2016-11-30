#ifndef ILIST_H
#define ILIST_H

/*
	PROSTOR PRO SPOUSTU definu
*/

typedef struct 
{
	//typ instrukce
	int TypInst;
	// 1 adresa
	void *adr1;
	// 2 adresa
	void *adr2;
	//3 adresa
	void *adr3;
}tInst;

typedef struct itemList
{
	tInst instr;
	struct itemList *nItem;
}tItemList;

typedef struct
{
	struct itemList *first;//prvni
	struct itemList *last;//posledni
	struct itemList *act;//aktivni
}tInstList;


//funkce inicializuici instrukcni seznam
void InitL(tInstList *L);

//dealokuje seznam 
void FreeL(tInstList *L);

//aktivace prvni instrukci
void FirstL(tInstList *L);

//nova instrukce na konci seznamu
void ILastL(tInstList *L, tList I);

//nasledujici = aktivni
void NextL(tInstList *L);

//nastavuje aktivni instrukci podle ukazatele
void GoToL(tInstList *L, tItemList *GoTo);

// vraci ukazatel na posledni instrukci
void *GLPL(tInstList *L);


tInst *getdata(tInstList *L);

void i_add(int typ, void *p1, void *p2, void *p3);
void i_print(tInst *inst);
void il_print();

#endif

