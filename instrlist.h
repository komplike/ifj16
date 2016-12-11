#ifndef INSTRLIST_H_
#define INSTRLIST_H_

#define ADD_I		    0
#define SUB_I		    1
#define MUL_I		    2
#define DIV_I		    3
#define FADD_I		  4
#define FSUB_I		  5
#define FDIV_I		  6
#define VALPUSH_I	  7
#define TABPUSH_I	  8
#define CONV_I		  9
#define SWAP_I		  10
#define CONCAT_I	  11
#define LESS_I		  12
#define GREAT_I		  13
#define LESSEQ_I	  14
#define GREATEQ_I	  15
#define EQ_I		    16
#define NEQ_I		    17
#define FLESS_I		  18
#define FGREAT_I	  19
#define FLESSEQ_I	  20
#define FGREATEQ_I	21
#define FEQ_I		    22
#define FNEQ_I		  23
#define GOTO_I		  24
#define FMUL_I      25
#define WHILE_I     26
#define IF_I        27
#define LABEL_I     28

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr instruction;
  struct listItem *nextItem;
} tListItem;


typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;


void listInit(tListOfInstr *L); // funkce inicializuje seznam instrukci
void listFree(tListOfInstr *L); // funkce dealokuje seznam instrukci
void listInsertLast(tListOfInstr *L, tInstr I); // vlozi novou instruci na konec seznamu
void listFirst(tListOfInstr *L); // zaktivuje prvni instrukci
void listNext(tListOfInstr *L); // aktivni instrukci se stane nasledujici instrukce
tInstr *listGetData(tListOfInstr *L); // vrati aktivni instrukci
void listGoto(tListOfInstr *L, tListItem *gotoInstr); // nastavime aktivni instrukci podle zadaneho ukazatele
void *listGetPointerLast(tListOfInstr *L); // vrati ukazatel na posledni instrukci
void printInstruction(tInstr *I) //vypisuje instrukce


#endif
