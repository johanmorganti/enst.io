#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _TYPES_
#include "types.h"
#endif

/* ---------------------------------------------------------------------- */
/*   FONCTION D'ALLOCATION MEMOIRE DE BAS NIVEAU                          */
/* ---------------------------------------------------------------------- */

typedef long base;
typedef base *bloc;

#define get_header(p)     (base*)(p) - 1
#define get_pointer(b)    (ptr)((b)+1)

#define resize_bloc(b,s)  *(b) = 2*(s);
#define mark_bloc(b)      *(b) = 2*(*(b)/2) + 1
#define free_bloc(b)      *(b) = 2*(*(b)/2)
#define next_bloc(b)      (b) + *(b)/2 + 1

#define bloc_free(b)      *(b)%2 == 0
#define bloc_used(b)      *(b)%2 == 1
#define bloc_size(b)      *(b)/2

bloc gc_zone;
bloc last_bloc;
bloc first_free;

/* initialise la zone memoire utilisee pour le GC */

boolean gc_init (longint size)

{
  size = size/sizeof(base);
  gc_zone = (bloc)malloc(size*sizeof(base));
  
  if (gc_zone == NULL)
    return false;
    
  first_free = gc_zone;
  resize_bloc(gc_zone,size - 2);
  
  last_bloc = next_bloc(gc_zone);
  resize_bloc(last_bloc,0); 
  
  return true;
}

/* efface tous les bit de controle des blocs de la zone du GC */

void gc_clean (void)

{
  bloc b = gc_zone;
  
  while (b != last_bloc) {
    free_bloc(b);
    b = next_bloc(b);
  }
}

/* met a un les bits de controle des blocs fils de la racine */

void gc_mark_terme (termeptr t)

{
  listeptr l;
  
  if (bloc_used(get_header(t)))
    return;
    
  mark_bloc(get_header(t));
  
  if (((t->ttyp == typmot) || (t->ttyp > 0)) && (t->tcons.mot != NULL))
    mark_bloc(get_header(t->tcons.mot));
    
  l = t->targs;
  while (l != NULL) {
    mark_bloc(get_header(l));
    if (l->elem.terme != NULL)
      gc_mark_terme(l->elem.terme);    
    l = l->suiv;
  }
}

void gc_mark_clause (clauseptr c)

{
  listeptr l;
  
  mark_bloc(get_header(c));
 
  if (c->ctete != NULL)
    gc_mark_terme(c->ctete);
 
  l = c->cqueue;
  while (l != NULL) {
    mark_bloc(get_header(l));
    if (l->elem.terme != NULL)
      gc_mark_terme(l->elem.terme);
    l = l->suiv;
  }
  
  l = c->cvariables;
  while (l != NULL) {
    mark_bloc(get_header(l));
    if (l->elem.mot != NULL)
      mark_bloc(get_header(l->elem.mot));
    l = l->suiv;
  }
}

void gc_mark_paquet (paquetptr p)

{
  listeptr l;
  
  mark_bloc(get_header(p));
  
  if (p->pnom != NULL)
    mark_bloc(get_header(p->pnom));
  
  l = p->pclauses;
  while (l != NULL) {
    mark_bloc(get_header(l));
    if (l->elem.clause != NULL)
      gc_mark_clause(l->elem.clause);
    l = l->suiv;
  }
}

void gc_mark_prog (progptr p)

{
  listeptr l;
  
  mark_bloc(get_header(p));
  
  l = p->dico;
  while (l != NULL) {
    mark_bloc(get_header(l));    
    if (l->elem.mot != NULL)
      mark_bloc(get_header(l->elem.mot));     
    l = l->suiv;
  }
  
  l = p->paquets;
  while (l != NULL) {
    mark_bloc(get_header(l));    
    if (l->elem.paquet != NULL)
      gc_mark_paquet(l->elem.paquet);     
    l = l->suiv;
  }
}

void gc_mark (void)

{
  integer i;
  
  if (terme_lu != NULL)
    gc_mark_terme(terme_lu);
  if (clause_lue != NULL)
    gc_mark_clause(clause_lue);
  gc_mark_prog(prog);
  
  for (i = 0;i < hautinstpile;++i)
    gc_mark_terme(instpile[i]->tmod);
}

/* reunit les blocs libres contigus en un seul bloc */

void gc_group (void)

{
  bloc p = NULL,b = gc_zone;
  
  first_free = last_bloc;
  
  while (b != last_bloc)
    if (bloc_free(b)) {
      if (first_free == last_bloc)
        first_free = b;
      if ((p != NULL) && (bloc_free(p))) {
        *p = *p + *b + 2;
        b = next_bloc(b);        
      } else {
        p = b;
        b = next_bloc(b);
      }
    } else {
      p = b;
      b = next_bloc(b);
    }
}

/* fonction de debogage */

void gc_test (char *s)

{
  bloc b = gc_zone;
  integer nb = 0;
    
  printf("%s\n",s);
  
  while (b != last_bloc) {
    if (bloc_used(b)) {
      printf("*");
      ++nb;
    } else
      printf("-");
      
    b = next_bloc(b);
  }
  
  printf("\n%d\n",nb);
}

/* tente d'allouer un bloc de taille size (exprimee en base) */

ptr gc_simple_malloc (longint size)

{
  boolean ok = false;
  bloc b = first_free;
  base b_size;  
        
  while ((b != last_bloc) && !ok) {
    b_size = bloc_size(b);   
    if ((bloc_free(b)) && ((b_size == size) || (b_size > size + 1)))
      ok = true;
    else
      b = next_bloc(b);
  }
      
  if (ok) {
    if (b_size > size) {
      resize_bloc(b,size);
      first_free = next_bloc(b);
      resize_bloc(first_free,b_size - size - 1);
    } else {
      first_free = next_bloc(b);
      while (bloc_used(first_free))
        first_free = next_bloc(first_free);
    }
    mark_bloc(b);
    return get_pointer(b);
  } else
    return NULL;
}

/* tente d'allouer un bloc de taille size (exprimee en octet) */

ptr gc_malloc (longint size)

{
  bloc b;
  
  if (size%sizeof(base) == 0)
    size = size/sizeof(base);
  else
    size = size/sizeof(base) + 1;

  b = gc_simple_malloc(size);
  
  if (b == NULL) {
    #ifdef _DEBUG_
    printf("ramasse miettes...\n");
    gc_test("avant");
    #endif
    
    gc_clean();
    gc_mark();
    gc_group();
    
    #ifdef _DEBUG_
    gc_test("apres");
    #endif
    
    b = gc_simple_malloc(size);
  }
  
  if (b == NULL)
    outofmemory = true;

  return b;
}

/* reconstruit le dictionnaire en eliminant les mots inutilises */

void gc_init_dico (progptr p)

{
  listeptr l;
  
  l = p->dico;
  while (l != NULL) {
    l->elem.mot->mutile = false;
    l = l->suiv;
  }
}

void gc_dico_terme (termeptr t)

{
  listeptr l;
    
  if (((t->ttyp == typmot) || (t->ttyp > 0)) && (t->tcons.mot != NULL))
    t->tcons.mot->mutile = true;

  l = t->targs;
  while (l != NULL) {
    gc_dico_terme(l->elem.terme);    
    l = l->suiv;
  }
}

void gc_dico_clause (clauseptr c)

{
  listeptr l;
  
  if (c->ctete != NULL)
    gc_dico_terme(c->ctete);
 
  l = c->cqueue;
  while (l != NULL) {
    gc_dico_terme(l->elem.terme);
    l = l->suiv;
  }
}

void gc_dico_paquet (paquetptr p)

{
  listeptr l;
  
  l = p->pclauses;
  while (l != NULL) {
    gc_dico_clause(l->elem.clause);
    l = l->suiv;
  }
}

void gc_dico_prog (progptr p)

{
  listeptr l;
  
  l = p->paquets;
  while (l != NULL) {
    gc_dico_paquet(l->elem.paquet);     
    l = l->suiv;
  }
  
  l = p->dico;
  while (l != NULL) {
    if (!l->elem.mot->mutile) {
      if (l->suiv != NULL)
        l->suiv->prec = l->prec;    
      if (l == p->dico)
        p->dico = l->suiv;
      else {
        l->prec->suiv = l->suiv;
        if (l->suiv == NULL)
          p->dico->prec = l->prec;
      }
    }
    l = l->suiv;
  }
}

void gc_collect (void)

{
  gc_init_dico(prog);
  gc_dico_prog(prog);
  
  #ifdef _DEBUG_
  printf("recalcul dico...\n");
  gc_test("avant");
  #endif
  
  gc_clean();
  gc_mark();
  gc_group();

  #ifdef _DEBUG_
  gc_test("apres");
  #endif
} 
