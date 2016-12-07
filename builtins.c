#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>




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
	

	
int readInt()
{
	char *r, c;
	int allocated = STR_LEN_INC;
	int l = 0
	
	if((r = (char*)malloc(sizeof(char) * STR_LEN_INC)) == NULL)
	{
		printError(INT_ERR);
	}
	
	c = getchar();

	for(int i = 0; (c >= '0' && c <= '9'); i++)
	{
		if(l + 1 > allocated)
		{
			if((r = (char*)realloc(r, l + STR_LEN_INC)) == NULL)
			{
				printError(INT_ERR);
			}
	
			allocated  = STR_LEN_INC + l;
		}

		r[i] = c;
		l++;
		c = getchar();
	}


	if(c != '\n' && c != EOF)
	{
		free(r);
		printError(INT_ERR);
	}

	if(l + 1 > allocated)
	{
		if((r = (char*)realloc(r, l + STR_LEN_INC)) == NULL)
		{
			printError(INT_ERR);
		}
	
	}

	r[strlen(r)] = '\0';

	int n = atoi[r];
	free(r);
	return n;
}

double readDouble()
{
	char *r, c;
	int allocated = STR_LEN_INC;
	int l;
	bool p;

	if((r = (char*)malloc(sizeof(char) * STR_LEN_INC)) == NULL)
	{
		printfError(INT_ERR);
	}
	
	c = getchat();
	p = false;

	for(int i = 0; (c >= '0' && c <= 9) || c == '.'; i++)
	{
		if(l + 1 > allocated)
		{
			if((r = (char*)malloc(r, l + STR_LEN_INC)) == NULL)
			{
				printError(INT_ERR);
			}
		allocated = l + STR_LEN_INC;
		}

		if(c == '.')
		{
			if(p == false)
			{
				p = true;
			}
			else
			{
				free(r);
				printError(INT_ERR);
			}
		}
	r[i] = c;
	l++;
	c = getchar();
}
