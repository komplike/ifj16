#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

void ErrExit(int err)
{
	switch(err)
	{
		case(ERR_LEX):
			fprintf(stderr, "IFJ16: lexical error!\n");
			break;
		case(ERR_SYNTAX):
			fprintf(stderr, "IFJ16: syntax error!\n");
			break;
		case(ERR_SEMANTIC_DEF):
			fprintf(stderr, "IFJ16: semantic error!\n");
			break;
		case(ERR_SEMANTIC_TYPES):
			fprintf(stderr, "IFJ16: semantic type error!\n");
			break;
		case(ERR_AUTO_TYPE):
			fprintf(stderr, "IFJ16: type error!\n");
			break;
		case(ERR_SEMANTIC_OTHERS):
			fprintf(stderr, "IFJ16: semantic error!\n");
			break;
		case(ERR_READ_NUMBER):
			fprintf(stderr, "IFJ16: failed to read number!\n");
			break;
		case(ERR_UNINIT):
			fprintf(stderr, "IFJ16: var missing !\n");
			break;
		case(ERR_ZERO_DIV):
			fprintf(stderr, "IFJ16: Math error: divison by zero!\n");
			break;
		case(ERR_RUNTIME):
			fprintf(stderr, "IFJ16: runtime error !\n");
			break;
		case(ERR_INTERNAL):
			fprintf(stderr, "IFJ16: smthng went wrong!\n");
			break;
		case(ERR_ALLOC):
			fprintf(stderr, "IFJ16: memory allocation failed!\n");
