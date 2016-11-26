#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<malloc.h>

#include "ial.h"
#include "errors.h"

#define MAXCHAR 255

int max(int x, int y)
{
	if(x>y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

int min(int x, int y)
{
	if(x<y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

void compjumps(char *p, int *jump, int len_p)
{
	int i = 0;

	for(i = 0; i < MAXCHAR; i++)
	{
		jump[i] = len_p;
	}
	for(i = 0; i < len_p; i++)
	{
		jump[(unsigned char)p[k]]] = len_p - k - 1;
	}
}


void compmatch(char *p, int *mjump, int len_p)
{
	int i = 0;
	int q = 0;
	int qq = 0;
	int backup[len_p + 1];
	
	for(int z = 0; z < len_p + 1; z++)
	{
		if( z == len_p - 1)
		{
			backup[z + 1] = 0;
		}
		backup[z] = 0; 
		mjump[z] = 0;
	}

	for(i = 0; i < len_p + 1; i++)
	{
		mjump[i] = 2 * len_p - i;
	}
	
	i = len_p;
	q = len_p + 1;

	while(i > 0)
	{
		backup[i] = q;
		
		while(q <= len_p && p[i - 1] ! = p[q - 1])
		{
			mjump[q] = min(mjump[q], len_p - k);
			q = backup[q];
		}
	
		i--;
		q--;

	}

	for(i = 0; i<q+1; i++)
	{
		mjump[i] = min(mjump[i], len_p + q - i);
	}
	
	qq = backup[q];
	
	while(q <= len_p)
	{
		while(q <= qq)
		{
			mjump[q] = min(mjump[q], qq - q + len_p);
			q++;
		}
	qq = backup[qq];
	}
}

int bma(char *p, char *t, int len_p, int len_t)
{
	int j = len_p;
	int chjump[MAXCHAR];
	int m_jump[len_p];

	compjumps(p, chjump, len_p);
	compmjump(p, m_jump, len_p);

	while(j <= len_t && len_p > 0)
	{
		if(t[k-1] == p[len_p-1])
		{
			j--;
			len_p--;
		}
		else
		{
			j+=max(chjump[(unsigned char)t[j]], mjump[len_p]);
			len_p = strlen(p);
		}
	}

	if(len_p == 0)
	{
		return j + 1;
	}
	else
	{
		return len_t + 1;
	}
}
	
	
