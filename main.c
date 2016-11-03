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
#include "lex.h"

int main(int argc, char** argv) {

    FILE *f = fopen("test.txt", "r");
    if(f == NULL){
        fprintf(stderr, "Chyba cteni souboru!");
        return 1;
    }
   
    int type = 0;
    char *content = NULL;
    
    while(type != LEX_EOF){
        getToken( f, &type, &content);  
        if(type == LEX_RUN_ERR || type == LEX_ERR)
            return 1;

        printf("content = \"%s\", ", content);
        printf("type = %d\n", type);
    }    
    
    free(content);
    fclose(f);
    
    return (EXIT_SUCCESS);
}

