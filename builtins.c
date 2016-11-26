#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>



#include "ial.h"

int lenght(const char *text)
{
	int i = 0;
	
	while(text[i] != '\0')
	{
		i++;
	}
	
	return i;
}

char *substr(char *str, int i, int n)
{
	if(i + n >lenght(str))
	{
		n = lenght(str) - i;
	}

	if(i>=0 && n>= 0)
	{
		char *text = malloc(n+1);
		if(text == NULL)
		{
			return NULL;
		}
		else
		{
			memcpy(text, str+i,n);
			text[n] = '\0';
			return text;
		}

	}
	
	else
	{
		return NULL;
	}
}

char *sort(char text[])
{
	int l = strlen(text);
	char *newstring;
	
	if((newstring = malloc(l + 1)))
	{
		memcpy(newstring, text, l + 1);
		
		if(l > 1)
		{
			shellsort(newstring, 0, l - 1);
		}
	}
	else
	{
		newstring = NULL;
	}
	
	return newstring;
}
	

	
