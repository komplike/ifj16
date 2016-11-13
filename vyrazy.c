/**
 * Predmet: IFJ / IAL
 * Projekt: Implementace interpretu imperativniho jazyka
 * Varianta:  b/3/2
 * Soubor:   Vyrazy.c
 * Datum:   10.10.2011
 * Autori:  Novak Lukas <xnovak1t@stud.fit.vutbr.cz>
 *         
 */

/*
	< -> L = Larger;
	> -> S = Smaller;
	= -> R = Equals;
	Chybný vstup = E;
*/

const int Ptable [16][16] =
{
    //	          +  -  *  /  == != <= >= >  <  (  ) ID  F  ,  $
    /*1  +  */  { L, L, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*2  -  */  { L, L, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*3  *  */  { L, L, L, L, L, L, L, L, L, L, S, L, S, S, L, L },
    /*4  /  */  { L, L, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*5  == */  { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*6  != */  { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*7  <= */  { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*8 >= */   { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*9 >  */   { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*10 <  */  { S, S, S, S, L, L, L, L, L, L, S, L, S, S, L, L },
    /*11 (  */  { S, S, S, S, S, S, S, S, S, S, S, R, S, S, R, E },
    /*12 )  */  { L, L, L, L, L, L, L, L, L, L, E, L, E, E, L, L },
    /*13 ID */  { L, L, L, L, L, L, L, L, L, L, E, L, E, E, L, L },
    /*14 F  */  { E, E, E, E, E, E, E, E, E, E, R, E, E, E, E, E },
    /*15 ,  */  { S, S, S, S, S, S, S, S, S, S, S, R, S, S, R, E },
    /*16 $  */  { S, S, S, S, S, S, S, S, S, S, S, E, S, S, E, E },
};
