/*
 *  Projekt IFJ16 - VUTBR FIT
 *  Tým 062, varianta b/3/II
 *  autor: Skryja Ondřej (xskryj02)  
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lex.h"

#define LIT_LEN 1   // po kolika se bude alokovat pamet

void getToken(FILE *f, int *type, char **content){
    
    bool print = true;  // pro testovaci vypisy
    
    // Uvolni pamet at tam bylo cokoli
    free(*content);
    *content = NULL;
    
    int content_size = 0;   // nastenych znaku
    int alloc_size = 1;     // kolikrat je LIT_LEN znaku alokovano
    int c;
    while(1){
        c = fgetc(f);
        
        // Vynecha bile znaky
        while(c == ' ' || c == '\n' || c == '\t'){
            c = fgetc(f);
            if(c == EOF){
                *type = LEX_EOF;
                return;
            }
        }
        
        // Prvni znak
        switch(c){
            // Komentare
            case '/':   c = fgetc(f);
                        if(c == '*'){
                            // blokovy komentar
                            if(print)printf("zacatek komentare\n");   
                            bool stop = false;
                            while(!stop && c != EOF){
                                c = fgetc(f);
                                if(c == '*'){
                                    c = fgetc(f);
                                    if(c == '/'){
                                        stop = true;
                                        if(print)printf("konec komentare\n");
                                    }                                        
                                }                                
                            }
                            
                            if(c == EOF){
                                *type = LEX_EOF;
                                return;
                            }
                            
                            break;  // na zacatek celeho while
                        }
                        else{
                            if(c == '/'){
                                // radkovy komentar
                                if(print)printf("radkovy komentar\n");    
                                while(c != '\n'){
                                    c = fgetc(f);  
                                    if(c == EOF){
                                        *type = LEX_EOF;
                                        return;
                                    }
                                }
                                break;  // na zacatek celeho while
                            }
                            else{
                                // deleni
                                if(print)printf("deleni\n"); 
                                *type = SIGN_DIV;
                                ungetc(c, f);
                                return;
                            }
                        }
                //return;
            case '+': if(print)printf("scitani\n"); *type = SIGN_ADD; return;
            case '-': if(print)printf("odcitani\n"); *type = SIGN_SUB; return;
            case '*': if(print)printf("nasobeni\n"); *type = SIGN_MUL; return;
            case ';': if(print)printf("strednik\n"); *type = O_SEMI; return;            
            case '.': if(print)printf("tecka\n"); *type = O_DOT;return;
            case '(': if(print)printf("kulata leva\n"); *type = B_ROUND_LEFT;return;
            case ')': if(print)printf("kulata prava\n"); *type = B_ROUND_RIGHT;return;
            case '{': if(print)printf("slozena leva\n"); *type = B_WAVE_LEFT;return;
            case '}': if(print)printf("slozena prava\n"); *type = B_WAVE_RIGHT;return;
            case EOF: if(print)printf("eof\n"); *type = LEX_EOF;return;
            case '=':   
                c = fgetc(f);    
                if(c == '='){
                    if(print)printf("rovnase\n");
                    *type = SIGN_EQ;
                    return;
                }
                else{
                    if(print)printf("prirazeni\n");
                    ungetc(c,f);    // vrati posledni nactene
                    *type = SIGN_ASSIGN;
                    return;
                }
            
            case '<':
                c = fgetc(f);    
                if(c == '='){
                    if(print)printf("mensi_rovno\n");
                    *type = SIGN_LOE;
                    return;
                }
                else{
                    if(print)printf("mensi\n");
                    ungetc(c,f);    // vrati posledni nactene
                    *type = SIGN_LESS;
                    return;
                }
                
            case '>':
                c = fgetc(f);    
                if(c == '='){
                    if(print)printf("vetsi_rovno\n");
                    *type = SIGN_MOE;
                    return;
                }
                else{
                    if(print)printf("vetsi\n");
                    ungetc(c,f);    // vrati posledni nactene
                    *type = SIGN_MORE;
                    return;
                }    
            
                
            case '!':
                c = fgetc(f);    
                if(c == '='){
                    if(print)printf("nerovnase\n");
                    *type = SIGN_NEQ;
                    return;
                }
                else{
                    if(print)printf("vykricnik a neco jinyho nez rovnase\n");
                    ungetc(c,f);    // vrati posledni nactene
                    *type = LEX_ERR;
                    return;
                }
                
            case '"':   // textovy retezec
                        if(print)printf("retezec\n");                        
                        while(1){
                            c = fgetc(f);
                            
                            // pokud je na vice radcich (neni povoleno)
                            if(c == '\n'){
                                *type = LEX_ERR;
                                return;
                            }
                            
                            // nasleduje esc. seq.
                            if(c == '\\'){            
                                c = fgetc(f);
                                // povolene: \" \n \t \\ 
                                // nebo \ddd (ddd oktalove cislo 001 - 377)

                                if(c == '\"')
                                    addChar(content,type,&alloc_size,&content_size,'\"');
                                else
                                    if(c == 'n')
                                        addChar(content,type,&alloc_size,&content_size,'\n');
                                    else
                                        if(c == 't')
                                            addChar(content,type,&alloc_size,&content_size,'\t');
                                        else
                                            if(c == '\\')
                                                addChar(content,type,&alloc_size,&content_size,'\\');
                                            else{
                                                // nacteni oktaloveho
                                                int idec = 0;
                                                for(int i = 0; i < 3; i++){
                                                    // neni cislice
                                                    if(c < '0' || c > '9'){
                                                        if(print)printf("spatne oct cislo\n");
                                                        *type = LEX_ERR;
                                                        return;
                                                    }
                                                    
                                                    // oct na dec
                                                    char cc[2];
                                                    cc[0] = c;
                                                    cc[1] = '\0';
                                                    idec += atoi(cc)*((i == 0)?64:((i == 1)?8:1));

                                                    c = fgetc(f);
                                                }
                                                                                              
                                                ungetc(c,f);    // vrati posledni nactene
                                                
                                                if(idec < 1 || idec > 255){
                                                    if(print)printf("Nepovoleny znak\n");
                                                    *type = LEX_ERR; return;                                                        
                                                }
                                                
                                                addChar(content,type,&alloc_size,&content_size,idec);
                                            }                                                
                                continue;
                            }       
                            
                            // konec
                            if(c == '"')
                                break;
                            
                            // EOF - zatim neni uzavreno
                            if(c == EOF){
                                *type = LEX_ERR;
                                return;
                            }
            
                            // pridani znaku
                            addChar(content,type,&alloc_size,&content_size,c);
                        } 
                        *type = L_STRING;return;
            default:    
                        // cte jeden retezec cehokoliv
                        // dokud neni mezera                                                                                 
                                   
                        // Identifikator
                        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '$'){                            
                            while((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '$'){
                                addChar(content,type,&alloc_size,&content_size,c);
                                c = fgetc(f);                                
                            }
                            ungetc(c,f);
                            *type = L_SIMPLE;
                            
                            // klicova slova?                            
                            if(strcmp(*content,"boolean") == 0)
                                *type = K_BOOLEAN;
                            if(strcmp(*content,"break") == 0)
                                *type = K_BREAK;
                            if(strcmp(*content,"class") == 0)
                                *type = K_CLASS;
                            if(strcmp(*content,"continue") == 0)
                                *type = K_CONTINUE;
                            if(strcmp(*content,"do") == 0)
                                *type = K_DO;
                            if(strcmp(*content,"double") == 0)
                                *type = K_DOUBLE;
                            if(strcmp(*content,"else") == 0)
                                *type = K_ELSE;
                            if(strcmp(*content,"false") == 0)
                                *type = K_FALSE;
                            if(strcmp(*content,"for") == 0)
                                *type = K_FOR;
                            if(strcmp(*content,"if") == 0)
                                *type = K_IF;
                            if(strcmp(*content,"int") == 0)
                                *type = K_INT;
                            if(strcmp(*content,"return") == 0)
                                *type = K_RETURN;
                            if(strcmp(*content,"String") == 0)
                                *type = K_STRING;
                            if(strcmp(*content,"static") == 0)
                                *type = K_STATIC;
                            if(strcmp(*content,"true") == 0)
                                *type = K_TRUE;
                            if(strcmp(*content,"void") == 0)
                                *type = K_VOID;
                            if(strcmp(*content,"while") == 0)
                                *type = K_WHILE;
                            
                            return;
                        }
                        else{
                            // Cislo nejake
                            if(c >= '0' && c <= '9'){
                                while(c >= '0' && c <= '9'){
                                    addChar(content,type,&alloc_size,&content_size,c);
                                    c = fgetc(f);  
                                }
                                // zatim celecislo a nejaky znak
                                
                                // Cele cislo
                                if(c != '.' && c != 'e' && c != 'E'){
                                    ungetc(c,f);
                                    *type = L_INT;
                                    return;
                                }                                
                                                              
                                // muze byt jen desetinne cislo
                                while(1){
                                    switch(c){
                                        case 'e':
                                        case 'E':   // exponent
                                            addChar(content,type,&alloc_size,&content_size,c);
                                            c = fgetc(f);
                                            
                                            // nepovinne
                                            if(c == '+' || c == '-'){
                                               addChar(content,type,&alloc_size,&content_size,c); 
                                               c = fgetc(f);
                                            }
                                                // bude neprazdny
                                                if(c >= '0' && c <= '9'){
                                                   addChar(content,type,&alloc_size,&content_size,c); 
                                                   c = fgetc(f);
                                                }
                                                else{
                                                    // je prazdny
                                                    *type = LEX_ERR;
                                                    return;
                                                }
                                            
                                            
                                            while(c >= '0' && c <= '9'){
                                                addChar(content,type,&alloc_size,&content_size,c); 
                                                c = fgetc(f);
                                            }
                                            ungetc(c,f);
                                            *type = L_DOUBLE;
                                            return;
                                            
                                        case '.':   // desetina cast
                                            addChar(content,type,&alloc_size,&content_size,c);
                                            c = fgetc(f);
                                            // prvni znak
                                            if(c >= '0' && c <= '9'){
                                                addChar(content,type,&alloc_size,&content_size,c);
                                                c = fgetc(f);  
                                            }
                                            else{
                                                // po . nei cislo
                                                *type = LEX_ERR;
                                                return;
                                            }
                                            
                                            // dalsi znaky
                                            while(c >= '0' && c <= '9'){
                                                addChar(content,type,&alloc_size,&content_size,c);
                                                c = fgetc(f);  
                                            }
                                            break;
                                    }
                                    
                                    if(c != 'e' && c != 'E'){
                                        ungetc(c,f);
                                        *type = L_DOUBLE;
                                        return;
                                    }                                        
                                }                                
                            }                                
                            else{
                                if(c == EOF){
                                   *type = LEX_EOF;
                                    return;
                                }
                                    // chyba
                                    *type = LEX_ERR;
                                    return;
                                }
                            }
                        }      
        
                        return;
                        
    }

}

void addChar(char **content, int *type, int *alloc_size, int *content_size, int c) {
    //alokace
    if ((*content) == NULL) {
        (*content) = malloc((*alloc_size)* LIT_LEN * sizeof (char));
        if ((*content) == NULL) {
            fprintf(stderr, "Malloc ERR\n");
            *type = LEX_RUN_ERR;
            return;
        }
    }

    (*content)[(*content_size)++] = c;

    // realokace
    if ((*content_size) + 1 > (*alloc_size) * LIT_LEN) {
        (*content) = realloc((*content), (++(*alloc_size)) * LIT_LEN * sizeof (char));
        if ((*content) == NULL) {
            fprintf(stderr, "ERROR: REALLOC\n");
            *type = LEX_RUN_ERR;
            return;
        }
    }
    
    (*content)[*content_size] = '\0';
    
}
