/* ****************************************************************************
 * funkcie na pracu s tabulkou symbolov
 * dole je zakomentovany main
 * autor: Michal Slavka
 * ***************************************************************************/
// #include "htab.h"
#include "parser.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>

#include "ial.h"
#include "errors.h"

#define MAXCHAR 255
//#include <string.h>
/* ****************************************************************************
 * hachovacia funkcia
 * ***************************************************************************/
unsigned int hash_function (const char *str, unsigned htab_size)
{
  unsigned int h = 0;
  const unsigned char *p;
  for (p = (const unsigned char *) str; *p != '\0'; p++)
    h = 65599 * h + *p;

  return h % htab_size;
}

/* ****************************************************************************
 * inicializacia tabulky
 * ***************************************************************************/
struct htab *htab_init (int size)
{

  int i = 0;
  // alokacia pamate pre strukturu
  htab_t *t =
    (htab_t *) malloc (sizeof (htab_t) +
		       sizeof (struct htab_listitem *) * size);
  if (t == NULL)
    return NULL;

  t->size = size;

  for (i = 0; i < size; i++)
    {				// inicializacia ukazatelov
      t->list[i] = NULL;
    }

  return t;
}

/* ****************************************************************************
 * vyhladavanie prvku v tabulke
 * ak sa v tabulke nenachadza vrati NULL
 * inak vracia ukazatel na prvok
 * ***************************************************************************/
struct htab_listitem *htab_lookup (htab_t * t, const char *class, const char *name)
{
  if (name == NULL)
    return NULL;
  unsigned int hash_i = hash_function (name, TAB_SIZE);	//index v hashovacej tabulke
  struct htab_listitem *ptr = t->list[hash_i];
  while (ptr != NULL)
    {
      if (strcmp (name, ptr->name) == 0)
	{
	  if (class != NULL && ptr->class != NULL)
	    {
	      if (strcmp (class, ptr->class) == 0)
		{
		  return ptr;
		}
	    }
	  else

	    return ptr;
	}

      ptr = ptr->next;
    }
  return NULL;
}

/* ****************************************************************************
 * pridava do ts list_item
 * !!!ak sa list_item uz v taulke nachadza tak budeduplikovany	
 * vracia 1 ak bolo pridanie uspesne
 * vracia 0 ak zliha alokacia pamate
 * parametre:
   ts, trieda, meno prvku, je definovany, typ, jeho hodnota
 * ***************************************************************************/
int htab_add (htab_t * htab, const char *class, const char *name, int def, int declr, int type)
{
  if (htab_lookup (htab, class, name) != NULL)
    {
      return E_SEM_PROG;
    }
  if (name == NULL)
    {				//pojde prec po odladeni
      return 99;
    }

  if (type == FUNC && class == NULL)
    {
      return 99;
    }

  struct htab_listitem *new_item = (struct htab_listitem *) malloc (sizeof (htab_listitem_t));
  if (new_item == NULL)
    {
      return 99;
    }

  //inicializacia noveho prvku
  new_item->class = str_cpy (class);;
  new_item->name = str_cpy (name);
  new_item->def = def;
  new_item->declr = declr;
  new_item->type = type;
  new_item->func = NULL;

  //pridanie prvku do tabulky
  unsigned int i = hash_function (name, TAB_SIZE);
  new_item->next = htab->list[i];
  htab->list[i] = new_item;

  return 0;
}


int htab_add_val (htab_t * t, const char *class, const char *name,
	      const char *value)
{
  struct htab_listitem *item;
  if ((item = htab_lookup (t, class, name)) == NULL)
    {
      return E_SEM_OTHERS;
    }
  if (item->func != NULL)
    {
      return 99;
    }

  item->def = DEF;
  if (item->type == FUNC)
    item->func = NULL;
  else if (item->def == DEF)
    {
      if (type == K_INT)
	item->val.val_int = str_int (value);
      else if (item->type == K_DOUBLE)
	item->val.val_double = str_double (value);
      else if (item->type == K_STRING)
	item->val.val_string = str_cpy (value);
    }
  else
    return 99;

  return 0;
}


int htab_comp (htab_t * htab, const char *class, const char *name, int type)
{
  struct htab_listitem *item;
  if ((item = htab_lookup (htab, class, name)) == NULL)
    {
      return E_SEM_PROG;
    }
  if (name == NULL)
    {				//pojde prec po odladeni
      return 99;
    }

  if (class == NULL)
    {
      return 99;
    }

  //inicializacia noveho prvku
  item->declr = DECLR;
  item->type = type;
  item->func = NULL;

  return 0;
}




/* ****************************************************************************
 * uvolnuje tabulku symbolov
 * parametre: tabulka symbolov, jej velkost
 * ***************************************************************************/
void htab_free (struct htab *t, int size)
{
  int i;
  struct htab_listitem *ptr1 = NULL;
  struct htab_listitem *ptr2 = NULL;


  for (i = 0; i < size; i++)
    {
      ptr1 = t->list[i];
      while (ptr1 != NULL)
	{
	  ptr2 = ptr1;
	  ptr1 = ptr1->next;
	  free (ptr2->class);
	  free (ptr2->name);
	  ptr2->class = NULL;
	  ptr2->name = NULL;
	  if (ptr2->type == K_STRING && ptr2->val.val_string != NULL)
	    {
	      free (ptr2->val.val_string);
	      ptr2->val.val_string = NULL;
	    }
	  if (ptr2->type == FUNC && ptr2->func != NULL)
	    {
	      free (ptr2->func->args);
	      ptr2->func->args = NULL;
	      //TODO  free(ptr->inst_ptr);
	      htab_free (ptr2->func->local_t, size);
	      free (ptr2->func);
	      ptr2->func = NULL;
	    }

	  free (ptr2);
	  ptr2 = NULL;
	}
    }
  free (t);
  t = NULL;

}

/* ****************************************************************************
 * doplnenenie informaci o funkcii
 * parametre:
   globalna TS, trieda funkcie, meno funkcie, navratovy typ, zoznam argumentov
 * vracia 0 pri uspechu
 * ***************************************************************************/
int htab_add_func (htab_t * t, const char *class, const char *name,
	       int return_type, arg_l * args)
{
  struct htab_listitem *item = htab_lookup (t, class, name);

  if (item == NULL)
    {
      return 100;
    }
  //ak v tabulke funkcia pridana ale este nema doplnene argumenty, navratovy typ etc.
  if (item->func == NULL)
    {
      struct function *f = (function_t *) malloc (sizeof (function_t));
      if (f == NULL)
	{
	  return 99;
	}
      //dodatocne informacie do ts
      f->return_type = return_type;
      f->argc = args->cnt;
      f->args = args_arr (args, args->cnt);	//funkcii sa preda zoznam obsahujuci argumenty
      f->local_t = htab_init (TAB_SIZE);	//inicializacia lokalnej tabulky symbolov
      item->func = f;		//pridanie do tabulky
      if (item->func->local_t == NULL || item->func->args == NULL)
	{			//kontrola ci sa podarila alokacia
	  return 99;		//po navrate treba vsetko uvolnit!!!
	}
      arg_add (item->func->local_t, args);	//pridanie argumentov do lokalnej tabulky symbolov
      //uvedenie zoznamu parametrov do inicializovaneho stavu
      args->cnt = 0;
      args->act = args->first;
    }

  return 0;
}


/* ****************************************************************************
 * inicializacia zoznamu argumentov
 * ***************************************************************************/
arg_l *arg_init ()
{
  struct arg_list *a = (struct arg_list *) malloc (sizeof (struct arg_list));
  if (a == NULL)
    {
      //error(99)
      return NULL;
    }

  a->first = NULL;
  a->act = NULL;
  a->last = NULL;
  a->cnt = 0;

  //alokacia prveho prvku
  a->first = (struct arg *) malloc (sizeof (arg_t));
  if (a->first == NULL)
    {
      return NULL;
    }
  else
    {
      a->first->class = NULL;
      a->first->name = NULL;
      a->first->next = NULL;
      a->act = a->first;
      a->last = a->first;
    }

  return a;
}


/* ****************************************************************************
 * pridavanie argumentov do lokalnej tabulky symbolov
 * musi ist ako posledna funkcia ktora pracuje s argumentmi
 * ***************************************************************************/
int arg_add (htab_t * t, arg_l * a)
{
  int i, ret;
  a->act = a->first;

  for (i = 0; i < a->cnt; i++)
  {
    if ((ret = htab_add (t, a->act->class, a->act->name, NDEF, DECLR, a->act->type)) != 0)
	return ret;
    if (a->act->class != NULL)
	{
	  free (a->act->class);
	}
    if (a->act->name != NULL)
	{
	  free (a->act->name);
	}
	  a->act->class = NULL;
	  a->act->name = NULL;
	  a->act = a->act->next;
  }
  return 0;
}

/* ****************************************************************************
 * indexovat sa zacina od 1
 * ak sa nebudu davat parametre po rade nebude to fungovat
 * parametre:
   inicializovany zoznam, trieda, meno argumentu, typ
 * ***************************************************************************/

int param_ctor (arg_l * a, const char *class, const char *name, int type)
{
  if (a == NULL){
      return 99;
    }

  a->act->class = str_cpy (class);
  a->act->name = str_cpy (name);
  a->act->type = type;
  a->cnt++;

  if (a->act->next == NULL){
      a->act->next = (struct arg *) malloc (sizeof (struct arg));

      if (a->act->next == NULL){
	  return 99;
	}
      else{
	  a->act = a->act->next;
	  a->act->class = NULL;
	  a->act->name = NULL;
	  a->act->next = NULL;
	  a->last = a->act;
	}
  }
  else
    a->act = a->act->next;


  return 0;
}

int arg_ctor (htab_t *t, htab_t *lt, arg_l *a, const char *cl, const char *name, int type)
{
  	if (a == NULL)
  	{
      	return 99;
  	}
  	if (type != L_SIMPLE) {
  		//name je hodnota predavana do funkcie
  		a->act->class = NULL;
  		a->act->name = NULL;
  		a->cnt++;
  		switch (type){
  			case L_INT: 	a->act->type = K_INT; break;
  			case L_STRING:	a->act->type = K_STRING; break;
  			case L_DOUBLE: 	a->act->type = K_DOUBLE; break;
  			default: return 99;
  		}
  	
  	} 
  	else {
	  	a->act->class = str_cpy (cl);
	  	a->act->name = str_cpy (name);
	  	a->cnt++;
	  	struct htab_listitem *item;
	  	if (strcmp(cl, class) == 0){
			item = htab_lookup (lt, NULL, name);
			if (item == NULL){
				item = htab_lookup (t, class, name);
			}
		}
	    else{	    	
			item = htab_lookup (t, cl, name);
	    }

	    if (item == NULL)
		{
		  	return E_SEM_PROG;
		}

	    if (item->def == NDEF)
		{
		  	return E_SEM_PROG;
		}

	    a->act->type = item->type;
	}

  	if (a->act->next == NULL)
  	{
      	a->act->next = (struct arg *) malloc (sizeof (struct arg));

      	if (a->act->next == NULL)
		{
	  		return 99;
		}
     	else
		{
			a->act = a->act->next;
			a->act->class = NULL;
			a->act->name = NULL;
			a->act->next = NULL;
			a->last = a->act;
		}
  	}
  	else
    	a->act = a->act->next;


  return 0;

}

/* ****************************************************************************
 * pridava argumenty definicie a volania funkcie do ts 
 * ***************************************************************************/
int *args_arr (arg_l * a, int argc)
{
  int *type_arr = (int *) malloc (sizeof (int) * argc);
  if (type_arr == NULL)
    return NULL;
  int c = 0;			//pocitadlo pre index
  a->act = a->first;
  for (c = 0; c < argc; c++)
    {
      type_arr[c] = a->act->type;
      a->act = a->act->next;
    }
  return type_arr;
}

/* ****************************************************************************
 * uvolnuje zoznam argumentov
 * ***************************************************************************/
void args_free (arg_l * a)
{
  a->act = a->first;
  while (a->last != NULL)
    {
      //free(a->act->name);
      //free(a->act->class);
    if (a->act != a->last)
	{
	  a->act = a->act->next;
	  free (a->first);
	  a->first = a->act;
	}
    else
	{
	  free (a->act);
	  a->first = NULL;
	  a->act = NULL;
	  a->last = NULL;
	}
  }
  free (a);
  a = NULL;
}

/* ****************************************************************************
 * kopia stringov
 * ***************************************************************************/
char *str_cpy (const char *str)
{
  if (str == NULL)
    return NULL;
  char *new_str = (char *) malloc (strlen (str) + 1);
  if (new_str == NULL)
    return NULL;
  /*unsigned int i;
     for (i = 0; i < strlen(str); ++i)
     {
     new_str[i] = str[i];
     } */
  strcpy (new_str, str);
  return new_str;
}

int arg_check(struct arg_list *a, struct htab_listitem *item){
	if (a->cnt != item->func->argc)
		return error = E_SEM_PROG;
	int i;
	a->act = a->first;
	for (i = 0; i < a->cnt; ++i){
		if (a->act->type != item->func->args[i]){
			return E_SEM_TYPE;
		}

		free(a->act->class);
		free(a->act->name);
		a->act->class = NULL;
		a->act->name = NULL;

		a->act = a->act->next;
		if (a->act == NULL)
			return 99;
	}
	a->cnt = 0;
	return E_OK;
}

/* ****************************************************************************
 * zmeni string na int
 * ***************************************************************************/
int str_int (const char *i)
{

  return atoi (i);
}

/* ****************************************************************************
 * TODO
 * convertuje string na double
 * ***************************************************************************/
double str_double (const char *d)
{
  if (d[0] > '0' + 8)
    printf ("%s\n", d);
  return 1.0;
}


//*****************************************************************
/* ****************************************************************************
 * vytlaci vsetky prvky tabulky
 * ***************************************************************************/
int for_each (htab_t * t, int size, foreach_func func)
{
  int i;
  struct htab_listitem *ptr;

  for (i = 0; i < size; i++)
    {
      ptr = t->list[i];
      while (ptr != NULL)
	{
	  func (ptr);
	  ptr = ptr->next;
	}
    }

  return 1;
}
void check1 (struct htab_listitem *l){
	if (l->declr == NDECLR){
		error = E_SEM_PROG;
	}
}
/* ****************************************************************************
 * vytlaci prvok
 * ***************************************************************************/
void print (struct htab_listitem *l)
{
  if (l == NULL)
    {
      printf ("print: bol predany NULL ptr\n");
      return;
    }


  if (l->class != NULL || l->name != NULL)
    {
      printf ("name: %s.%s\n", l->class, l->name);
    }
  else if (l->name == NULL)
    {
      printf ("Vo funkci print: chyba meno\n");
    }
  else
    {
      printf ("name: (NULL).%s\n", l->name);
    }
  printf ("init: %d\ndeclr: %d\ntype: %d\n", l->def, l->declr, l->type);
  if (l->def == DEF)
    switch (l->type)
      {
      case K_INT:
	printf ("int: %d\n", l->val.val_int);
	break;
      case K_DOUBLE:
	printf ("double: %f\n", l->val.val_double);
	break;
      case K_STRING:
	printf ("string: %s\n", l->val.val_string);
	break;
      case FUNC:
	print_func (l);
	break;
      }
  else
    switch (l->type)
      {
      case K_INT:
	printf ("int: NULL\n");
	break;
      case K_DOUBLE:
	printf ("double: NULL\n");
	break;
      case K_STRING:
	printf ("string: NULL\n");
	break;
	//case FUNC:            print_func(l);
      }
  printf ("- - - - - - - - - - - - - - -\n");
}

void print2 (struct htab_listitem *l)
{
  printf ("\t\tname: %s\n", l->name);
  printf ("\t\tinit: %d\n\t\tdeclr: %d\n\t\ttype: %d\n", l->def, l->declr, l->type);
  if (l->def == DEF)
    switch (l->type)
      {
      case K_INT:
	printf ("		int: %d\n", l->val.val_int);
	break;
      case K_DOUBLE:
	printf ("		double: %f\n", l->val.val_double);
	break;
      case K_STRING:
	printf ("		string: %s\n", l->val.val_string);
	break;
      case FUNC:
	print_func (l);
	break;
      }
  else
    switch (l->type)
      {
      case K_INT:
	printf ("		int: NULL\n");
	break;
      case K_DOUBLE:
	printf ("		double: NULL\n");
	break;
      case K_STRING:
	printf ("		string: NULL\n");
	break;
      }
  printf ("\n");
}

void print_func (struct htab_listitem *l)
{
  if (l->type == FUNC)
    {
      printf ("	name: %s.%s\n", l->class, l->name);
      if (l->def == DEF && l->func != NULL)
	{
	  printf ("	return_type: %d\n", l->func->return_type);
	  printf ("	argc: %d\n", l->func->argc);
	  printf ("	lokalna ts:\n\n");
	  for_each (l->func->local_t, TAB_SIZE, print2);
	}
      //print args
    }
}

int search_for_class (htab_t * t, const char *class, int size)
{
  int i;
  struct htab_listitem *ptr;

  for (i = 0; i < size; i++)
    {
      ptr = t->list[i];
      while (ptr != NULL)
	{
	  if (strcmp (ptr->class, class) == 0)
	    {
	      return 1;
	    }
	  ptr = ptr->next;
	}
    }
  return 0;
}


void shellSort(char *array, int n)
{
  int step, i, j;
  char tmp;
    step = n / 2; 

      while (step > 0) 
          {
          for (i = step; i < n - 1; i++)
          {
             j = i - step + 1;

              while ((j >= 1) && (array[j] > array[j + step]))
            {
                 tmp = array[j];
                 array[j] = array[j + step];
                 array[j + step] = tmp;
                 j = j - step;
              }
          }
          step = step / 2; 
      }
}   

int max(int x, int y)
{
  if(x>y)
  {
    return x;
  }
  else
  {
    return y;
  }
}

int min(int x, int y)
{
  if(x<y)
  {
    return x;
  }
  else
  {
    return y;
  }
}

void compjumps(char *p, int *jump, int len_p)
{
  int i = 0;

  for(i = 0; i < MAXCHAR; i++)
  {
    jump[i] = len_p;
  }
  for(i = 0; i < len_p; i++)
  {
    jump[(unsigned char)p[k]]] = len_p - k - 1;
  }
}


void compmatch(char *p, int *mjump, int len_p)
{
  int i = 0;
  int q = 0;
  int qq = 0;
  int backup[len_p + 1];
  
  for(int z = 0; z < len_p + 1; z++)
  {
    if( z == len_p - 1)
    {
      backup[z + 1] = 0;
    }
    backup[z] = 0; 
    mjump[z] = 0;
  }

  for(i = 0; i < len_p + 1; i++)
  {
    mjump[i] = 2 * len_p - i;
  }
  
  i = len_p;
  q = len_p + 1;

  while(i > 0)
  {
    backup[i] = q;
    
    while(q <= len_p && p[i - 1] ! = p[q - 1])
    {
      mjump[q] = min(mjump[q], len_p - k);
      q = backup[q];
    }
  
    i--;
    q--;

  }

  for(i = 0; i<q+1; i++)
  {
    mjump[i] = min(mjump[i], len_p + q - i);
  }
  
  qq = backup[q];
  
  while(q <= len_p)
  {
    while(q <= qq)
    {
      mjump[q] = min(mjump[q], qq - q + len_p);
      q++;
    }
  qq = backup[qq];
  }
}

int bma(char *p, char *t, int len_p, int len_t)
{
  int j = len_p;
  int chjump[MAXCHAR];
  int m_jump[len_p];

  compjumps(p, chjump, len_p);
  compmjump(p, m_jump, len_p);

  while(j <= len_t && len_p > 0)
  {
    if(t[k-1] == p[len_p-1])
    {
      j--;
      len_p--;
    }
    else
    {
      j+=max(chjump[(unsigned char)t[j]], mjump[len_p]);
      len_p = strlen(p);
    }
  }

  if(len_p == 0)
  {
    return j + 1;
  }
  else
  {
    return len_t + 1;
  }
}/
