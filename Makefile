#IFJ, 2016-12-2
#Autor: Michal Slavka

CFLAGS = -std=c99 -g
#-pedantic -Wall -Wextra -g -fPIC
MOD = ial.o lex.o parser.o

all: compiler clear
comp: compiler

compiler: main.o $(MOD)
	gcc main.c $(MOD) $(CFLAGS) -o c

clear:
	rm *[.]o
