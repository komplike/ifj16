#ifndef ERRORS_H_
#define ERRORS_H_

extern const char *err_table[];

enum msgs
{
	SUCCESS = 0,
	LEX_ERR = 1,
	SYN_ERR = 2,
	UNDEF_ID = 3,
	INC_TYP = 4,
	AUT_T_ERR = 5,
	SEM_ERR = 6 ,
	RT_NUM_IN = 7,
	RT_UNINIT = 8,
	RT_DIV_BZ = 9,
	RT_OTHER = 10,
	INT_ERR = 99,
};

unsigned int printError(unsigned int ret_msg_id);

#endif
