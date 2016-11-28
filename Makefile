#Autor:   Michal Slavka
#len pre rozbehanie parsera a pridanie tabulky sybolov

CFLAGS = -std=c99 
#-pedantic -Wall -Wextra -g -fPIC
#MOD = hash_function.o htab_clear.o htab_foreach.o htab_free.o htab_init.o htab_init2.o htab_lookup_add.o htab_remove.o
MOD = htab.o lex.o

all: parser clear
parser: parser

parser: parser.o $(MOD)
	gcc parser.c $(MOD) $(CFLAGS) -o pars

clear:
	rm *[.]o
