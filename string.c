#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "string.h"
#include "errors.h"


#define BUFF 16

int strinit(TString *str)
{
	if((str->string = (char*)malloc(BUFF)) == NULL)
	{
		printError(INT_ERR);
	}
	
	str->string[0] = '\0';
	str->length = 0;
	str->allocated = BUFF;
	
	return 0;
}

int strInitDeafault(TString *str, char *s)
{
	int res = strinit(str);

	if(res != 0)
	{
		return res;
	}
	
	for(unsigned i = 0; i < strlen(s); i++)
	{
		res=strCharAppend(str,s[i]);
		
		if(res != 0)
		{
			return res;
		}
	return 0;
}


int strCharAppend(TString *str, char c)
{
	int abs = str->length + 1;
	
	if(abs >= str->allocated)
	{
		str->string = realloc(str->string, abs + BUFF);
		
		if(str->string == NULL)
		{
			printError(INT_ERR);
		}
		
		str->allocated = abs + BUFF;
	}

	str->string[str->length++] = c;//pridava znak;
	str->string[str->length] = '\0';
	
	return 0;
}

void strReset(TString *str)
{
	str->string[0] = '\0';
	str->length = 0;
}

int strTStringCmp(const TString *str1, const TString *str2)
{
	return strcmp(str1->string, str2->string);
}

int strConstTStringCmp(TString *str1, char* str2)
{
	return strcmp(str1->string, str2);
}

int strTStringCpy(TString *destination, const TString *source)
{
	if(destination || source == NULL)
	{
		printError(INT_ERR);
	}
	
	int length = source->length;
	
	if(length + 1 >= destination->allocated)
	{
		if((destination->string = (char*)realloc(destination->string, length+1)) == NULL)
		{
			printError(INT_ERR);
		}
	}
	strcpy(destination->string, source->string);

	destination->length = length;
	destination->allocated = length+1;
	
	return 0;
}

int strTStringLen(TString *str)
{
	return str->length;
}

int strTStringCat(TString **destination, TString *source1, TString *source2)
{
	if(source1 == NULL || source2 == NULL)
	{
		printfError(INT_ERR);
	}
	
	*destination = malloc(sizeof(TString));
	strinit(*destination);

	int length = source1->length + source2->length;

	if(length + 1 >= (*destination)->allocated)
	{
		if(((*destination)->string = (char*)realloc((*destination)->string, length +1)))
		{
			printError(INT_ERR);
		}
	}
	(*destination)->length = length;	
	(*destination)->allocated = length+1;

	strcpy((*destination)->string, source1->string);
	strcat((*destination)->string, source2->string);
	
	return 0;
}
