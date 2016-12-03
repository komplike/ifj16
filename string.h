#ifndef _MY_STRING_H
#define _MY_STRING_H

// prevzaty z ukazkoveho projektu "Zjednodušená implementace interpretu jednoduchého jazyka", zatim beze zmeny

typedef struct string_struct
{
  char* string;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocated;	// velikost alokovane pameti
}TString;

int strInit(TString *str);
int strInitDefault(TString *str, char *s);
int strCharAppend(TString *str, char c);
void strReset(TString *str);
int strStringCmp(const TString *str1, const TString *str2);
int strConstTStringCmp(TString *str1, char *str2);
int strTStringCpy(TString *destination, const TString *source);
int strStringLen(TString *str);
int strTStringCat(TString **destionation, TString *source1, TString *source2);

#endif
