/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lex.h
 * Author: QWERT
 *
 * Created on 26. října 2016, 11:23
 */

#ifndef LEX_H
#define LEX_H

#include <stdbool.h>
#include <string.h>

void addChar(char **content, int *type, int *alloc_size, int *content_size, int c);
void getToken(FILE *f, int *type, char **content);

#define K_BOOLEAN   1001    
#define K_BREAK     1002
#define K_CLASS     1003
#define K_CONTINUE  1004
#define K_DO        1005
#define K_DOUBLE    1006
#define K_ELSE      1007
#define K_FALSE     1008
#define K_FOR       1009
#define K_IF        1010
#define K_INT       1011
#define K_RETURN    1012
#define K_STRING    1013
#define K_STATIC    1014
#define K_TRUE      1015
#define K_VOID      1016
#define K_WHILE     1017

#define SIGN_ADD    2001    // +
#define SIGN_SUB    2002    // -
#define SIGN_MUL    2003    // *
#define SIGN_DIV    2004    // /
#define SIGN_ASSIGN 2005    // =
#define SIGN_EQ     2006    // ==
#define SIGN_NEQ    2007    // !=
#define SIGN_LESS   2008    // <
#define SIGN_MORE   2009    // >
#define SIGN_LOE    2010    // <=
#define SIGN_MOE    2011    // >=

#define B_ROUND_LEFT    3001    // (
#define B_ROUND_RIGHT   3002    // )
#define B_WAVE_LEFT     3003    // {
#define B_WAVE_RIGHT    3004    // }

#define L_SIMPLE    4001    // literal
#define L_INT       4002    // literal typu int
#define L_DOUBLE    4003    // literal typu double
#define L_STRING    4004    // literal typu string

#define O_SEMI      5001    // ;
#define O_DOT       5002    // .
#define O_COMMA     5003    // ,

#define LEX_EOF     9001        // konec radku
#define LEX_ERR     9002	// chyba v souboru
#define LEX_RUN_ERR 9003	// chyba v automatu

#endif /* LEX_H */

