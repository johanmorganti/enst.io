#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _TYPES_
#include "types.h"
#endif

#ifndef _GC_
#include "gc.h"
#endif

#ifndef _BASE_
#include "base.h"
#endif

#ifndef _SYNTAXE_
#include "syntaxe.h"
#endif

#ifndef _RESOLVE_
#include "resolve.h"
#endif

/* ---------------------------------------------------------------------- */
/*   PROGRAMME PRINCIPAL                                                  */
/* ---------------------------------------------------------------------- */

boolean outofmemory = false;

termeptr terme_lu = NULL;
clauseptr clause_lue = NULL;
progptr prog = NULL;

pileptr pile;
pilerec *butpile;
integer hautbutpile = 0;

instrec *envpile;
integer hautenvpile = 0;

instptr *instpile;
integer hautinstpile = 0;

/* ---------------------------------------------------------------------- */
/*   INITIALISATIONS                                                      */
/* ---------------------------------------------------------------------- */

char *string;

int lit_chaine (void)

{
  if (*string == 0)
    return EOF;
  else {
    ++string;
    return *(string-1);
  }
}

void definit_clause (progptr p,char *clause)

{
  clauseptr c;
  
  string = clause;
  if ((c = analyse_clause(p,&lit_chaine)) != NULL)
    ajoute_clause(prog,c);
}

void init_prog (progptr p)

{
  p->dico = NULL;
  p->paquets = NULL;

  definit_clause(p,"quit.");
  definit_clause(p,"echec."); 
  definit_clause(p,"!.");
  
  definit_clause(p,"atome(X).");
  definit_clause(p,"nombre(X).");
  definit_clause(p,"variable(X).");
  
  definit_clause(p,"plus(X,Y,Z).");
  definit_clause(p,"mult(X,Y,Z).");
  definit_clause(p,"supeg(X,Y).");

  definit_clause(p,"lit(X).");
  definit_clause(p,"ecrit(X).");
  definit_clause(p,"return.");
}

/* ---------------------------------------------------------------------- */
/*   PREDICAT CONSULT                                                     */
/* ---------------------------------------------------------------------- */

FILE *f;
int car;

int lit_fichier (void)

{
  car = getc(f);
  if (car != EOF)
    putchar(car);
  return car;
}

void consult (char *name)

{
  clauseptr c;

  if ((f = fopen(name,"r")) == NULL) {
    printf("fichier introuvable\n");
    return;
  }

  prog->dico = NULL;
  prog->paquets = NULL;

  gc_collect();

  init_prog(prog);

  do
    if ((c = analyse_clause(prog,&lit_fichier)) != NULL)
      resout_prog(prog,c);
  while (c != NULL);

  fclose(f);
}

/* ---------------------------------------------------------------------- */
/*   FONCTION MAIN                                                        */
/* ---------------------------------------------------------------------- */

int main (int argc,char *argv[])

{
  clauseptr c;
  termeptr t;

  if (!gc_init(500000)) {
    printf("memoire insuffisante\n");
    return 0;
  }

  butpile = (pilerec*)malloc(nb_but*sizeof(pilerec));
  envpile = (instrec*)malloc(nb_env*sizeof(instrec));
  instpile = (instptr*)malloc(nb_inst*sizeof(instptr));

  prog = (progptr)gc_malloc(sizeof(progrec));
  
  if ((butpile == NULL) || (envpile == NULL) || (instpile == NULL) || (prog == NULL)) {
    printf("memoire insuffisante\n");
    return 0;
  }
  
  init_prog(prog);

  if (outofmemory) {
    printf("memoire insuffisante\n");
    return 0;
  }

  do {
    printf(">");
    fflush(stdout);
    
    outofmemory = false;
    clause_lue = NULL;
    terme_lu = NULL;
    gc_collect();
   
    c = analyse_clause(prog,&getchar);
    fflush(stdin);

    if (c != NULL)
      if ((c->ctete == NULL) && (c->cqueue != NULL) && (c->cqueue->suiv == NULL)) {
        t = c->cqueue->elem.terme;
        if ((strcmp(t->tcons.mot->mval,"consult") == 0) &&
            (t->targs != NULL) && (t->targs->suiv == NULL) &&
            (t->targs->elem.terme->ttyp == typmot) && 
            (t->targs->elem.terme->targs == NULL))
          consult(t->targs->elem.terme->tcons.mot->mval);
        else
          resout_prog(prog,c);
      } else
        resout_prog(prog,c);
         
    if (outofmemory)
      printf("memoire insuffisante\n");
     
  } while (true);
 
  return 0;
}
