/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: QWERT
 *
 * Created on 26. října 2016, 11:22
 */

#include <stdio.h>
#include <stdlib.h>
#include "ial.h"
#include "lex.h"

int main(int argc, char** argv) {

    if (argc != 2)
        return 99;

    arg = arg_init();
    t = htab_init(TAB_SIZE);

    error = parser(argv[1]);

    htab_free(t, TAB_SIZE);
    args_free(arg);
    free(content);

    return (EXIT_SUCCESS);
}

