#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "stack.h"

int interpreter(htab_t *Table, tListOfInstr *List);
void get_instr(int type, void *op1, void *op2, void *op3);
#endif
